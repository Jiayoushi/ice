#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <fstream>

#include "http.h"

namespace ice {

void InitContentMapping();


/* RequestHandler */
const size_t kMaxBufSize = 1024;
enum ResponseStatus {
  kNormalResponseCompleted = 0,  // Either a ok response or a error response
  kWaitForCgi = 1, // Need to wait for cgi signal
  kInternalError = 2,  // Server has internal error
};
 
typedef int FileDescriptor;
typedef std::pair<ResponseStatus, FileDescriptor> HandlerResult;

struct StaticContent {
  std::string content_path;
  std::string content_type;
  std::string content;

  StaticContent(const std::string &path, const std::string &type):
    content_path(path), content_type(type), content() {
      std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
      content = std::string((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
  }
};

class RequestHandler {
 public:
  typedef std::vector<std::string> Response;

  int client_fd;
  std::string ip_address;
  HttpRequest http_request;
  Response responses;

  RequestHandler() {}
  RequestHandler(int client_fd, const sockaddr_in &client_address):
    client_fd(client_fd), 
    ip_address(inet_ntoa(client_address.sin_addr)),
    http_request(),
    responses() {
  }

  HandlerResult GetResponse();
  void SendResponse();
 private:
  void AppendResponse(std::string &&data);
  void GetValidResponse(const StaticContent &static_content);
  void GetErrorResponse(const size_t kHttpErrorCode);
  int GetCgiResponse();
};


/* CgiInfo */
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
