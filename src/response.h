#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <string>
#include <vector>

#include "http.h"

namespace ice {

struct Response {
  std::vector<std::string> responses;
};

enum ResponseStatus {
  kNormalResponseCompleted = 0,  // Either a ok response or a error response
  kWaitForCgi = 1, // Need to wait for cgi signal
  kInternalError = 2,  // Server has internal error
}; 

void InitContentMapping();
ResponseStatus GetResponse(const HttpRequest &http_request, Response &response);
void SendResponse(int client_fd, const Response &response);

class CgiInfo {
 static const size_t kMaxCgiArgumentsNum = 128;
 public:
  CgiInfo(const HttpRequest &http_request);
  ~CgiInfo();

  const char *GetScriptName() const; 
  const char *GetBody() const;
  const char GetBodySize() const;
  char **GetArgv();
  char **GetEnvp();
  
 private:
  std::string GetScriptNameFromUrl(const std::string &url);

  size_t body_size;
  size_t argc;
  size_t envc;
  char *argv[kMaxCgiArgumentsNum];
  char *envp[kMaxCgiArgumentsNum];
  char *body;
};

}
#endif
