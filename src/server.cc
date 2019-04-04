#include "server.h"

#include <stdio.h>
#include <iostream>

#include "network.h"
#include "defs.h"

namespace ice {

std::string base_directory;
std::string content_directory;

Server::Server(const std::string &kBaseDirectory) {
  base_directory = kBaseDirectory;
  content_directory = kBaseDirectory + "/content/";
  InitHttpParserSettings();
  InitContentMapping();
}

Server::~Server() {
}

void Server::Connect() {
  listen_fd_ = SetUp();
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
          ResponseStatus rs = HandleClient(fd);
          //if (rs == kWaitForCgi) {
            // Do nothing
          //} else {
            // Error or a valid response
            RemoveClient(fd);
            Close(fd);
            FD_CLR(fd, &active_fds);
          //}
        }
      }
    }
  }
}

void Server::Run() {
  Connect();
  Loop();
}

ResponseStatus Server::HandleClient(int client_fd) {
  ClientInfo &client_info = client_infos_[client_fd];
  char message[kMaxMessageLen];
  int message_len = Read(client_info.client_fd, message, kMaxMessageLen);

  ParseHttpMessage(message, message_len, client_info.http_request);

  Response response;
  ResponseStatus rs = GetResponse(client_info.http_request, response);
  if (rs == kNormalResponseCompleted) {
    SendResponse(client_fd, response);
  } else if (rs == kWaitForCgi) {
    SendResponse(client_fd, response);
  } else {
    perror("Server failed to deliver a reponse");
  }
  return rs;
}

void Server::AddClient(int client_fd, const sockaddr_in &client_address) {
  client_infos_[client_fd] = ClientInfo(client_fd, client_address);
}

void Server::RemoveClient(int client_fd) {
  client_infos_.erase(client_fd);
}

}
