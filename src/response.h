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


class CgiInfo {
 static const size_t kMaxCgiArgumentsNum = 128;
 public:
  CgiInfo(const HttpRequest &http_request);
  ~CgiInfo();
  const char *GetScriptName() const; 
  char **GetArgv();
  char **GetEnvp();
  
 private:
  std::string GetScriptNameFromUrl(const std::string &url);

  size_t argc;
  size_t envc;
  char *argv[kMaxCgiArgumentsNum];
  char *envp[kMaxCgiArgumentsNum];
  char *body;
};

}
#endif
