#include "response.h"

#include <iostream>
#include <fstream>

namespace ice {

void GetResponse(const HttpRequest &http_request, Response &response) {
  std::ifstream ifs("../content/home.html", std::ifstream::in | std::ifstream::binary);
  response.content.assign((std::istreambuf_iterator<char>(ifs)), \
                          (std::istreambuf_iterator<char>()));
}



}
