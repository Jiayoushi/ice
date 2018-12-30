#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <string>
#include <vector>

#include "http.h"

namespace ice {

struct Response {
  std::vector<std::string> responses;
};

void GetResponse(const HttpRequest &http_request, Response &response);
void SendResponse(int client_fd, const Response &response);

}
#endif
