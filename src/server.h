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
  Server(const std::string &kBaseDirectory);
  ~Server();

  void Run();

 private:
  const size_t kMaxMessageLen = 2048;

  void Connect();
  void Loop();
   
  void AddClient(int client_fd, const sockaddr_in &client_address);
  void RemoveClient(int client_fd);
  HandlerResult HandleClient(ClientInfo &ci);

  int listen_fd_;
  ClientMap client_map_;
};

}

#endif
