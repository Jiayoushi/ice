
#include <iostream>
#include <fstream>
#include <vector>

#include "response.h"
#include "network.h"
#include "defs.h"


namespace ice {

extern std::string base_directory;

const std::string kHttpVersion = "HTTP/1.1";
const std::string kServerInformation = "Server: ice";

struct ContentInformation {
  std::string content_path;
  std::string content_type;
};

void GetValidResponse(Response &response) {
  std::string data;

  std::ifstream ifs(base_directory + "/content/home.html", std::ifstream::in | std::ifstream::binary);
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
  data.append("Content-Type: text/html");
  data.append("\n");
  data.append("\n");
  data.append(content);

  response.responses.push_back(data);
}

void GetErrorResponse(Response &response) {
  std::string data;

  data.append(kHttpVersion);
  data.append(" ");
  data.append(std::to_string(400));
  data.append(" ");
  data.append("Bad Request");
  data.append("\n");
  data.append(kServerInformation);
  data.append("\n");

  response.responses.push_back(data);
}

void GetResponse(const HttpRequest &http_request, Response &response) {
  if (http_request.valid) {
    GetValidResponse(response);
  } else {
    GetErrorResponse(response);
  }
}

void SendResponse(int client_fd, const Response &response) {
  for (const std::string &r: response.responses) {
    Write(client_fd, r.c_str(), r.size());
  }
}

}
