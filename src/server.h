#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>

namespace ice {

struct ClientInfo {
  int client_fd;
  std::string ip_address;

  ClientInfo() {
  }

  ClientInfo(int client_fd, const sockaddr_in &client_address):
    client_fd(client_fd),
    ip_address(inet_ntoa(client_address.sin_addr)) {
  }
};

class Server {
  public:
    Server();
    ~Server();
    
    void Run();

  private:
    const size_t kMaxMessageLen = 2048;

    void Connect();
    void Loop();
   
    void AddClient(int client_fd, const sockaddr_in &client_address);
    void HandleClient(int client_fd);

    int listen_fd_;
    std::unordered_map<int, ClientInfo> client_infos_;
};

}

#endif