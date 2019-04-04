#include "response.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <unordered_map>
#include <iostream>
#include <fstream>
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

struct ContentInformation {
  std::string content_path;
  std::string content_type;
  std::string content;

  ContentInformation(const std::string &path, const std::string &type):
    content_path(path), content_type(type), content() {
      std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
      content = std::string((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
  }
};

std::unordered_map<std::string, ContentInformation> content_map;

void InitContentMapping() {
  content_map.insert({"/", ContentInformation(content_directory + "home.html", "text/html")});
  content_map.insert({"/favicon.ico", ContentInformation(content_directory + "ow.ico", "image/apng")});
}

void GetValidResponse(ClientInfo &ci, const ContentInformation &content_information) {
  std::string data;

  data.append(kHttpVersion);
  data.append(" ");
  data.append(std::to_string(200));
  data.append(" ");
  data.append("OK");
  data.append("\n");
  data.append(kServerInformation);
  data.append("\n");
  data.append("Content-Length: " + 
               std::to_string(content_information.content.size()));
  data.append("\n");
  data.append("Content-Type: " + content_information.content_type);
  data.append("\n");
  data.append("\n");
  data.append(content_information.content);

  ci.responses.push_back(data);
}

void GetErrorResponse(ClientInfo &ci, const size_t kHttpErrorCode) {
  std::string data;

  data.append(kHttpVersion);
  data.append(" ");
  data.append(std::to_string(kHttpErrorCode));
  data.append(" ");
  data.append(http_error_map.at(kHttpErrorCode));
  data.append("\n");
  data.append(kServerInformation);
  data.append("\n");

  ci.responses.push_back(data);
}

int GetCgiResponse(const ClientInfo &ci) {
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

  CgiInfo cgi_info(ci.http_request);
  pid_t p = fork();
  if (p < 0) {
    perror("GetCgiReseponse: fork failed");
    return -1;
  } else if (p == 0) {
    // Read message body from parent using stdin
    close(write_to_child[1]); 
    dup2(write_to_child[0], STDIN_FILENO);
    char buf[kMaxBufSize];
    int bytes_read = read(STDIN_FILENO, buf, kMaxBufSize);
    if (bytes_read < 0) {
      perror("GetCgiResponse: child read");
    } else {
      buf[bytes_read] = '\0';
    }

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

  if (wait(nullptr) < 0) {
    perror("GetCgiResponse: wait failed");
  }

  return read_from_child[0];
}

HandlerResult GetResponse(ClientInfo &ci) {
  if (!ci.http_request.valid) {
    GetErrorResponse(ci, 400);   
  } else if (ci.http_request.Get("Url").compare(0, 9, "/cgi-bin/") == 0) {
    // Any url that starts with /cgi-bin/ should be handled with a cgi response
    int fd = 0;
    if ((fd = GetCgiResponse(ci)) < 0) {
      GetErrorResponse(ci, 500);
    } else {
      return std::make_pair(kWaitForCgi, fd);
    }
  } else if (content_map.find(ci.http_request.Get("Url")) != content_map.end()) {
    GetValidResponse(ci, content_map.at(ci.http_request.Get("Url")));
  } else {
    GetErrorResponse(ci, 404);
  }

  // -1 is ignored
  return std::make_pair(kNormalResponseCompleted, -1);
}

void SendResponse(int client_fd, const Response &response) {
  for (const std::string &r: response) {
    Write(client_fd, r.c_str(), r.size());
  }
}


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
  envp[0] = new char[content_length.size() + 1];
  strcpy(envp[0], content_length.c_str());
  envp[1] = nullptr;
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

}
