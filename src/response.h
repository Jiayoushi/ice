#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <string>
#include <vector>

#include "http.h"

namespace ice {

struct Response {
  std::vector<std::string> responses;
};

void InitContentMapping();
void GetResponse(const HttpRequest &http_request, Response &response);
void SendResponse(int client_fd, const Response &response);

namespace cgi {

const size_t kMaxCgiArgumentsNum = 128;

class CgiInfo {
 public:
  CgiInfo(const HttpRequest &http_request);

  std::string GetFilenameFromUrl(const std::string &url);

 private:
  char *argv[kMaxCgiArgumentsNum];
  char *envp[kMaxCgiArgumentsNum];
  char *body;
};


}

}
#endif
