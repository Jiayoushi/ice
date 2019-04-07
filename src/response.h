#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_set>
#include <sstream>
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

class ContentHandler {
 public:
  ContentHandler();
  ContentHandler(const std::string &fp, const std::string &ct);

  std::string GetContent(const HttpRequest &http_request) const;
  std::string GetContentType() const;

 private:
  std::string GetStaticContent() const;
  std::string GetPostContent(const std::string &body) const;

  std::string filepath_;
  std::string content_type_;
};

class RequestHandler {
 public:
  int client_fd;
  int child_pid;  // Used when handling cgi
  std::string ip_address;
  HttpRequest http_request;
  std::string response;

  RequestHandler() {}
  RequestHandler(int client_fd, const sockaddr_in &client_address):
    client_fd(client_fd), 
    child_pid(-1),
    ip_address(inet_ntoa(client_address.sin_addr)),
    http_request(),
    response() {
  }

  HandlerResult GetResponse();
  void SendResponse();
 private:
  void AppendResponse(std::string &&data);
  int GetCgiResponse();
  void GetValidResponse();
  void GetErrorResponse(const size_t kHttpErrorCode);
};




/* CgiInfo */
class CgiInfo {
 public:
  static const size_t kMaxCgiArgumentsNum = 128;

  CgiInfo(const HttpRequest &http_request);
  ~CgiInfo();

  const char *GetScriptName() const; 
  const char *GetBody() const;
  const char GetBodySize() const;
  void AddArgument(const std::string &s);
  void AddEnvironmentVariable(const std::string &s);
  void SetArguments(const HttpRequest &http_request);
  void SetEnvironmentVariables(const HttpRequest &http_request);
  void SetBody(const HttpRequest &http_request);
  char **GetArgv();
  char **GetEnvp();
  
 private:
  std::vector<std::string> arguments_;
  std::string script_name_;
  std::string query_string_;
  std::string request_url_;
  // Ignore 0 means return the first occurence
  // Ignore 1 means ignore the first occurence
  int Find(const std::string &s, int character, int ignore);
  void ParseUrl(const HttpRequest &http_request);

  size_t body_size_;
  size_t argc_;
  size_t envc_;
  char *argv_[kMaxCgiArgumentsNum];
  char *envp_[kMaxCgiArgumentsNum];
  char *body_;
};

}
#endif
