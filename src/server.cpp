#include <iostream>
#include <stdio.h>

#include "server.h"
#include "network.h"

namespace ice {

Server::Server() {
}

Server::~Server() {
  std::cout << "Server: destructs" << std::endl;
}

void Server::Connect() {
  listen_fd_ = SetUp();
  std::cout << "Server: set up sucessful" << std::endl;
}

void Server::Loop() {
  fd_set active_fds;
  fd_set read_fds;

  FD_ZERO(&active_fds);
  FD_SET(listen_fd_, &active_fds);

  while (true) {
    read_fds = active_fds;
    Select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);
    for (size_t fd = 0; fd < FD_SETSIZE; ++fd) {
      if (FD_ISSET(fd, &read_fds)) {
        if (fd == listen_fd_) {
          sockaddr_in client_address;
          socklen_t address_length = sizeof(client_address);
          int client_fd = Accept(listen_fd_, (sockaddr *)&client_address, &address_length);

          AddClient(client_fd, client_address);
          FD_SET(client_fd, &active_fds);
        } else {
          HandleClient(fd);
        }
      }
    }
  }
}

void Server::Run() {
  Connect();
  Loop();
}

void Server::AddClient(int client_fd, const sockaddr_in &client_address) {
  client_infos_[client_fd] = ClientInfo(client_fd, client_address);
}

void Server::HandleClient(int client_fd) {
  const ClientInfo &client_info = client_infos_[client_fd];
  char buffer[kMaxMessageLen];
  int message_len = Read(client_info.client_fd, buffer, kMaxMessageLen);
  buffer[message_len] = '\0';

  if (message_len > 0) {
    Write(client_fd, buffer, message_len);
  }
}

}
