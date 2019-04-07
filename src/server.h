#ifndef ICE_SERVER_H_
#define ICE_SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <string>

#include "http.h"
#include "response.h"

namespace ice {


class Server {
 public:
  typedef std::unordered_map<FileDescriptor, std::shared_ptr<RequestHandler>>
          RequestHandlerMap;  

  Server();
  ~Server();

  void Run();

 private:
  const size_t kMaxMessageLen = 2048;

  fd_set active_fds_;
  fd_set read_fds_;

  void Connect();
  void Loop();
   
  void AcceptConnection(int listen_fd);
  void RemoveRequest(int client_fd);
  void HandleRequest(int fd);
  int ReadRequest(int client_fd, char *buf, int buf_size);

  int listen_fd_;
  RequestHandlerMap request_handler_map_;
};

}

#endif
