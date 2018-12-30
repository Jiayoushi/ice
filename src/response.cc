#include "response.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace ice {

const std::string kHttpVersion = "HTTP/1.1";
const std::string kServerInformation = "Server: ice";

void GetValidResponse(const HttpRequest &http_request, Response &response) {
  std::string &data = response.data;

  std::ifstream ifs("../content/home.html", std::ifstream::in | std::ifstream::binary);
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
  data.append("\n");
  data.append(content);
}

void GetErrorResponse(Response &response) {
  std::string &data = response.data;

  data.append(kHttpVersion);
  data.append(" ");
  data.append(std::to_string(400));
  data.append(" ");
  data.append("Bad Request");
  data.append("\n");
  data.append(kServerInformation);
  data.append("\n");
}

void GetResponse(const HttpRequest &http_request, Response &response) {
  if (http_request.valid) {
    GetValidResponse(http_request, response);
  } else {
    GetErrorResponse(response);
  }
}



}
