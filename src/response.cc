#include "response.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "network.h"
#include "defs.h"

namespace ice {

extern std::string content_directory;
const std::string kHttpVersion = "HTTP/1.1";
const std::string kServerInformation = "Server: ice";

std::unordered_map<size_t, std::string> http_error_map({
  {400, "Bad Request"},
  {404, "Not Found"},
  {500, "Internal Server Error"},
});

std::unordered_map<std::string, ContentHandler> content_handler_map;

void InitContentMapping() {
  content_handler_map.insert({"/", 
         ContentHandler(content_directory + "home.html", "text/html")});
  content_handler_map.insert({"/favicon.ico",
         ContentHandler(content_directory + "ow.ico", "image/apng")});
}

void RequestHandler::GetValidResponse() {
  const ContentHandler &content_handler = content_handler_map[http_request.Get("Url")];
  std::string content = content_handler.GetContent(http_request);

  std::string response;
  response.append(kHttpVersion);
  response.append(" ");
  response.append(std::to_string(200));
  response.append(" ");
  response.append("OK");
  response.append("\n");
  response.append(kServerInformation);
  response.append("\n");
  response.append("Content-Length: " + std::to_string(content.size()));
  response.append("\n");
  response.append("Content-Type: " + content_handler.GetContentType());
  response.append("\n");
  response.append("\n");
  response.append(content);

  AppendResponse(std::move(response));
}

void RequestHandler::GetErrorResponse(const size_t kHttpErrorCode) {
  std::string response;

  response.append(kHttpVersion);
  response.append(" ");
  response.append(std::to_string(kHttpErrorCode));
  response.append(" ");
  response.append(http_error_map.at(kHttpErrorCode));
  response.append("\n");
  response.append(kServerInformation);
  response.append("\n");

  AppendResponse(std::move(response));
}



HandlerResult RequestHandler::GetResponse() {
  if (!http_request.valid) {
    GetErrorResponse(400);   
  } else if (http_request.Get("Url").compare(0, 9, "/cgi-bin/") == 0) {
    // Any url that starts with /cgi-bin/ should be handled with a cgi response
    int fd = 0;
    if ((fd = GetCgiResponse()) < 0) {
      GetErrorResponse(500);
    } else {
      return std::make_pair(kWaitForCgi, fd);
    }
  } else if (content_handler_map.find(http_request.Get("Url")) != content_handler_map.end()) {
    GetValidResponse();
  } else {
    GetErrorResponse(404);
  }

  // -1 is ignored
  return std::make_pair(kNormalResponseCompleted, -1);
}

void RequestHandler::SendResponse() {
  for (const std::string &r: responses) {
    Write(client_fd, r.c_str(), r.size());
  }
}

int RequestHandler::GetCgiResponse() {
  int write_to_child[2];
  int read_from_child[2];
  if (pipe(write_to_child) < 0) {
    perror("GetCgiResponse: create pipe for write_to_child failed");
    return -1;  
  }
  if (pipe(read_from_child) < 0) {
    perror("GetCgiResponse: create pipe for read_from_child failed");
    return -1;
  }

  CgiInfo cgi_info(http_request);
  pid_t p = fork();
  child_pid = p;
  if (p < 0) {
    perror("GetCgiReseponse: fork failed");
    return -1;
  } else if (p == 0) {
    // Read message body from parent using stdin
    close(write_to_child[1]); 
    dup2(write_to_child[0], STDIN_FILENO);

    // Send response to parent using stdout
    close(read_from_child[0]);
    dup2(read_from_child[1], STDOUT_FILENO);
   
    // Execve
    if (execve(cgi_info.GetScriptName(), 
               cgi_info.GetArgv(), cgi_info.GetEnvp()) < 0) {
      perror(cgi_info.GetScriptName());
      exit(EXIT_FAILURE);
    }
  }

  // Pass any message body (especially for POSTs) via stdin to the CGI executable
  close(write_to_child[0]);
  if (cgi_info.GetBody() != nullptr) {
    int bytes_write = write(write_to_child[1], 
                        cgi_info.GetBody(), cgi_info.GetBodySize());
    if (bytes_write < 0) {
      perror("GetCgiResponse: write to child");
    }
  }
  close(write_to_child[1]);
  return read_from_child[0];
}


/* CgiInfo */
CgiInfo::CgiInfo(const HttpRequest &http_request):
  body_size(0), argc(0), envc(0), body(nullptr) {

  ++argc;
  const std::string &script_name = GetScriptNameFromUrl(http_request.Get("Url"));
  argv[0] = new char[script_name.size() * sizeof(char)];
  strcpy(argv[0], script_name.c_str());
  argv[1] = nullptr; 

  ++envc;
  const std::string &content_length = std::string("CONTENT_LENGTH=") 
                         + http_request.Get("Content-Length");
  envp[0] = new char[content_length.size()];
  strcpy(envp[0], content_length.c_str());
  envp[1] = nullptr;

  SetBody(http_request);
}

CgiInfo::~CgiInfo() {
  for (size_t i = 0; i < argc; ++i) {
    delete[] argv[i];
  }
  for (size_t i = 0; i < envc; ++i) {
    delete[] envp[i];
  }
  delete[] body;
}

void CgiInfo::SetBody(const HttpRequest &http_request) {
  const std::string &b = http_request.Get("Body");
  body = new char[b.size()];
  strcpy(body, b.c_str()); 
  body_size = b.size();
}

const char *CgiInfo::GetScriptName() const {
  return argv[0];
}

const char *CgiInfo::GetBody() const {
  return body;
}

const char CgiInfo::GetBodySize() const {
  return body_size;
}

char **CgiInfo::GetArgv() {
  return argv;
}

char **CgiInfo::GetEnvp() {
  return envp;
}

std::string CgiInfo::GetScriptNameFromUrl(const std::string &url) {
  // The scriptname is the path to the cgi script.
  // /cgi-bin/ should be project home directory + cgi-bin directory
  // Then append the scriptname, with ? at the end.

  // n is the position one character before ?
  std::string::size_type n = url.find('?');
  if (n == std::string::npos) {
    return base_directory + url;
  } else {
    // url.substr(n) gets rid of the y part of x?y
    return base_directory + url.substr(0, n);
  }
}

void RequestHandler::AppendResponse(std::string &&response) {
  responses.push_back(std::move(response));
}



}

