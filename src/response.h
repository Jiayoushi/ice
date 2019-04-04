#ifndef ICE_RESPONSE_H_
#define ICE_RESPONSE_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "http.h"

namespace ice {


class ClientHandler {
 public:
  int client_fd;
  std::string ip_address;
  HttpRequest http_request;
  std::vector<std::string> responses;

  ClientHandler() {
  }

  ClientHandler(int client_fd, const sockaddr_in &client_address):
    client_fd(client_fd), 
    ip_address(inet_ntoa(client_address.sin_addr)),
    http_request(),
    responses() {
  }
};

typedef int FileDescriptor;
typedef std::unordered_map<FileDescriptor, std::shared_ptr<ClientHandler>> ClientMap;
typedef std::vector<std::string> Response;

const size_t kMaxBufSize = 1024;
enum ResponseStatus {
  kNormalResponseCompleted = 0,  // Either a ok response or a error response
  kWaitForCgi = 1, // Need to wait for cgi signal
  kInternalError = 2,  // Server has internal error
}; 
typedef std::pair<ResponseStatus, FileDescriptor> HandlerResult;

void InitContentMapping();
HandlerResult GetResponse(ClientHandler &ci);
void SendResponse(int client_fd, const Response &r);


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
