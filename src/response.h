#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <string>

#include "http.h"

namespace ice {

struct Response {
  std::string content;
  size_t size;
};

void GetResponse(const HttpRequest &http_request, Response &response);


}
#endif
