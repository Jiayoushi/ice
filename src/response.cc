#include "response.h"

#include <iostream>
#include <fstream>

namespace ice {

const std::string kHttpVersion = "HTTP/1.1";
const std::string kServer = "Server: ice";

void GetResponse(const HttpRequest &http_request, Response &response) {
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
  data.append(kServer);
  data.append("\n");
  data.append("Content-Length: " + std::to_string(content.size()));
  data.append("\n");
  data.append("\n");
  data.append(content);
}



}
