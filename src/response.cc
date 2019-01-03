#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>

#include "response.h"
#include "network.h"
#include "defs.h"


namespace ice {

extern std::string content_directory;
const std::string kHttpVersion = "HTTP/1.1";
const std::string kServerInformation = "Server: ice";

std::unordered_map<size_t, std::string> http_error_map({\
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

void GetCgiResponse(const HttpRequest &http_request, Response &response) {
  std::string data;

  response.responses.push_back(data);
}

void GetResponse(const HttpRequest &http_request, Response &response) {
  if (!http_request.valid) {
    GetErrorResponse(response, 400);   
  } else if (content_map.find(http_request.Get("Url")) == content_map.end()) {
    if (http_request.Get("Url").compare(0, 5, "/cgi/") == 0) {
      GetCgiResponse(http_request, response);
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

}
