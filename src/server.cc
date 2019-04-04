#include "server.h"

#include <stdio.h>
#include <unistd.h>
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
          auto result = client_map_.find(fd);
          if (result == client_map_.end()) {
            perror("client_map error: unmatched fd");
          }
          std::shared_ptr<ClientInfo> &ci = result->second;
          if (fd == ci->client_fd) {
            HandlerResult hr = HandleClient(*ci);
            if (hr.first == kWaitForCgi) {
              FD_SET(hr.second, &active_fds);
              client_map_[hr.second] = ci;
            } else {
              // Error or a valid response
              RemoveClient(fd);
              Close(fd);
              FD_CLR(fd, &active_fds);
            }
          } else {
            // Cgi has responded
            // Read from cgi
            char response[kMaxBufSize];
            int bytes_read = read(fd, response, kMaxBufSize);
            if (bytes_read < 0) {
              perror("Server select read response from cgi error");
            } else {
              response[bytes_read] = '\0';
            }
            // Send to client
            int bytes_sent = write(ci->client_fd, response, bytes_read);
            if (bytes_sent < 0) {
              perror("Server select send response from cgi error");
            }
 
            // Close connection
            RemoveClient(fd);
            Close(fd);
            FD_CLR(fd, &active_fds);
          }
        }
      }
    }
  }
}

void Server::Run() {
  Connect();
  Loop();
}

HandlerResult Server::HandleClient(ClientInfo &ci) {
  // Read from the client
  char message[kMaxMessageLen];
  int message_len = Read(ci.client_fd, message, kMaxMessageLen);

  ParseHttpMessage(message, message_len, ci.http_request);

  HandlerResult rs = GetResponse(ci);
  if (rs.first == kNormalResponseCompleted) {
    SendResponse(ci.client_fd, ci.responses);
  } else if (rs.first == kWaitForCgi) {
    // Should do nothing
  } else {
    perror("Server failed to deliver a reponse");
  }
  return rs;
}

void Server::AddClient(int client_fd, const sockaddr_in &client_address) {
  client_map_[client_fd] = std::make_shared<ClientInfo>(client_fd, client_address);
}

void Server::RemoveClient(int client_fd) {
  client_map_.erase(client_fd);
}

}
