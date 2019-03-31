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
  {404, "Not Found"}
});

struct ContentInformation {
  std::string content_path;
  std::string content_type;

  ContentInformation(const std::string &path, const std::string &type):
    content_path(path), content_type(type) {
  }
};

std::unordered_map<std::string, ContentInformation> content_map;

void InitContentMapping() {
  content_map.insert({"/", ContentInformation(content_directory + "home.html", "text/html")});
  content_map.insert({"/favicon.ico", ContentInformation(content_directory + "ow.ico", "image/apng")});
}

void GetValidResponse(Response &response, const ContentInformation &content_information) {
  std::string data;

  std::ifstream ifs(content_information.content_path, std::ifstream::in | std::ifstream::binary);
  std::string content((std::istreambuf_iterator<char>(ifs)), \
                      (std::istreambuf_iterator<char>()));

  data.append(kHttpVersion);
  data.append(" ");
  data.append(std::to_string(200));
  data.append(" ");
  data.append("OK");
  data.append("\n");
  data.append(kServerInformation);
  data.append("\n");
  data.append("Content-Length: " + std::to_string(content.size()));
  data.append("\n");
  data.append("Content-Type: " + content_information.content_type);
  data.append("\n");
  data.append("\n");
  data.append(content);

  response.responses.push_back(data);
}

void GetErrorResponse(Response &response, const size_t kHttpErrorCode) {
  std::string data;

  data.append(kHttpVersion);
  data.append(" ");
  data.append(std::to_string(kHttpErrorCode));
  data.append(" ");
  data.append(http_error_map.at(kHttpErrorCode));
  data.append("\n");
  data.append(kServerInformation);
  data.append("\n");

  response.responses.push_back(data);
}

int GetCgiResponse(const HttpRequest &http_request, Response &response) {
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

  pid_t child_pid = fork();
  if (child_pid < 0) {
    perror("GetCgiReseponse: fork failed");
    return -1;
  } else if (child_pid == 0) {
    // Read message body from parent using stdin
    close(write_to_child[1]); 
    dup2(STDIN_FILENO, write_to_child[0]);

    // Send response to parent using stdout
    close(read_from_child[1]);
    dup2(STDOUT_FILENO, write_to_child[1]);
   
    CgiInfo cgi_info(http_request);
    // Execve
    if (execve(cgi_info.GetScriptName(), 
               cgi_info.GetArgv(), cgi_info.GetEnvp()) < 0) {
      perror(cgi_info.GetScriptName());
      exit(EXIT_FAILURE);
    }
  }

  // Pass any message body (especially for POSTs) via stdin to the CGI executable
  close(write_to_child[0]); 

  if (wait(nullptr) < 0) {
    perror("GetCgiResponse: wait failed");
  }

  // Read from child
  close(read_from_child[1]);
  char *response_content = new char[1024];
  int bytes_read = read(read_from_child[0], response_content, 1024);
  if (bytes_read < 0) {
    perror("GetCgiResponse: Read from child failed");
  } else {
    response_content[bytes_read] = '\0';
    response.responses.push_back(std::string(response_content));
    delete[] response_content;
  }

  close(write_to_child[1]);
  close(read_from_child[0]);
  return 0;
}

void GetResponse(const HttpRequest &http_request, Response &response) {
  if (!http_request.valid) {
    GetErrorResponse(response, 400);   
  } else if (content_map.find(http_request.Get("Url")) == content_map.end()) {
    // Any url that starts with /cgi-bin/ should be handled with a cgi response
    if (http_request.Get("Url").compare(0, 9, "/cgi-bin/") == 0) {
      if (GetCgiResponse(http_request, response) < 0) {
        GetErrorResponse(response, 500);
      }
    } else {
      GetErrorResponse(response, 404);
    }
  } else {
    GetValidResponse(response, content_map.at(http_request.Get("Url")));
  }
}

void SendResponse(int client_fd, const Response &response) {
  for (const std::string &r: response.responses) {
    Write(client_fd, r.c_str(), r.size());
  }
}


CgiInfo::CgiInfo(const HttpRequest &http_request):
  argc(0), envc(0), body(nullptr) {

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
