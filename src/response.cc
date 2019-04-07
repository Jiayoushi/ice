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
const std::string kGatewayInterface = "CGI/1.1";

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

void RequestHandler::AppendResponse(std::string &&response) {
  responses.push_back(std::move(response));
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
  if (http_request.Get("Method") != "HEAD") {
    response.append(content);
  }
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
    int bytes_write = Write(write_to_child[1], 
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
  body_size(0), argc(0), envc(0), 
  body(nullptr) {
  ParseUrl(http_request);
  SetArguments(http_request);
  SetEnvironmentVariables(http_request);
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

void CgiInfo::AddArgument(const std::string &s) {
  argv[argc] = new char[s.size() + 1];
  strcpy(argv[argc], s.c_str());
  ++argc;
  argv[argc] = nullptr;
}

void CgiInfo::SetArguments(const HttpRequest &http_request) {
  AddArgument(script_name);
  for (const std::string &argument: arguments) {
    AddArgument(argument);
  }
}

void CgiInfo::AddEnvironmentVariable(const std::string &s) {
  envp[envc] = new char[s.size() + 1];
  strcpy(envp[envc], s.c_str());
  ++envc;
  envp[envc] = nullptr;
}

void CgiInfo::SetEnvironmentVariables(const HttpRequest &http_request) {
  AddEnvironmentVariable("CONTENT_LENGTH=" + http_request.Get("Content-Length"));
  AddEnvironmentVariable("CONTENT_TYPE=" + http_request.Get("Content-Type"));
  AddEnvironmentVariable("GATEWAY_INTERFACE=" + kGatewayInterface);
  AddEnvironmentVariable("REQUEST_URL=" + request_url);
  AddEnvironmentVariable("SCRIPT_NAME=" + script_name);
}

void CgiInfo::SetBody(const HttpRequest &http_request) {
  const std::string &b = http_request.Get("Body");
  body = new char[b.size()];
  strcpy(body, b.c_str()); 
  body_size = b.size();
}

const char *CgiInfo::GetScriptName() const {
  return script_name.c_str();
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

int CgiInfo::Find(const std::string &s, int target, int ignore) {
  for (int i = 0; i < s.size(); ++i) {
    if (s[i] == target) {
      if (ignore == 0) {
        return i;
      } else  {
        --ignore;
      }
    }
  }

  return -1;
}

void CgiInfo::ParseUrl(const HttpRequest &http_request) {
  // /cgi-bin/script_name/arg1/arg2/?query
  const std::string &url = http_request.Get("Url");

  int script_name_end = Find(url, '/', 2);
  if (script_name_end == -1) {
    script_name_end = url.size();
  }
  script_name = base_directory + url.substr(0, script_name_end);

  int query_string_start = Find(url, '?', 0) + 1;
  if (query_string_start != -1) {
    query_string = url.substr(query_string_start);
  }

  request_url = url.substr(0, query_string_start - 1);


  char u[1024];
  strcpy(u, http_request.Get("Url").c_str());
  char *token = std::strtok(u, "/?");
  int i = 0;
  while (token != nullptr) {
    if (i++ >= 2) {
      arguments.push_back(std::string(token));
    }
    token = std::strtok(nullptr, "/?");
  }
  arguments.pop_back();
}




/* ContentHandler */
std::string ContentHandler::GetPostContent(const std::string &body) const {
  // Just count the words to show the server has successfully responsed
  std::unordered_set<std::string> s;
  std::istringstream iss(body);
  for (std::string word; iss >> word; ) {
    s.insert(word);
  }
  return "Word Count: " + std::to_string(s.size()) + "\n";
}
std::string ContentHandler::GetStaticContent() const{
  std::ifstream ifs(filepath_, std::ifstream::in | std::ifstream::binary);
  return std::string((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
}

std::string ContentHandler::GetContent(const HttpRequest &http_request) const {
    const std::string &method = http_request.Get("Method");
    if (method == "GET") {
      return GetStaticContent();
    } else if (method == "POST") {
      const std::string &body = http_request.Get("Body");
      return GetPostContent(body);
    } else if (method == "HEAD") {
      return GetStaticContent();
    } else {
      return std::string(method);
    }
}

std::string ContentHandler::GetContentType() const {
  return content_type_;
}

ContentHandler::ContentHandler():
  filepath_(),
  content_type_() {

}

ContentHandler::ContentHandler(const std::string &fp, const std::string &ct):
  filepath_(fp),
  content_type_(ct) {

  }

}
