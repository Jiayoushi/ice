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
  FD_ZERO(&active_fds);
  FD_SET(listen_fd_, &active_fds);

  while (true) {
    read_fds = active_fds;
    Select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);
    for (size_t fd = 0; fd < FD_SETSIZE; ++fd) {
      if (FD_ISSET(fd, &read_fds)) {
        if (fd == listen_fd_) {
          AcceptClient(fd);
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

void Server::HandleClient(int fd) {
  auto result = client_map_.find(fd);
  if (result == client_map_.end()) {
    perror("client_map error: unmatched fd");
    return;
  }

  ClientInfo &ci = *(result->second);
  // fd matches client_fd, read from client and prepare response
  if (fd == ci.client_fd) {
    // Read from the client
    char message[kMaxMessageLen];
    int message_len = ReadClient(fd, message, kMaxMessageLen);

    // Parse client's message
    ParseHttpMessage(message, message_len, ci.http_request);

    // Get response according to parsed request
    HandlerResult rs = GetResponse(ci);
    // If it's a valid response or an error response
    if (rs.first == kNormalResponseCompleted) {
      SendResponse(ci.client_fd, ci.responses);
      RemoveClient(fd);
    // If not, let select waits for cgi
    } else if (rs.first == kWaitForCgi) {
      int child_fd = rs.second;
      FD_SET(child_fd, &active_fds);
      client_map_[child_fd] = result->second;
    } else {
      perror("HandleClient: unmatched HandlerResult");
      RemoveClient(fd);
    }
  // Cgi has responded
  } else {
    // Read from cgi
    char response[kMaxBufSize];
    int bytes_read = read(fd, response, kMaxBufSize);
    if (bytes_read < 0) {
      perror("HandleClient read response from cgi error");
    } else {
      response[bytes_read] = '\0';
    }
    // Send to client
    int bytes_sent = write(ci.client_fd, response, bytes_read);
    if (bytes_sent < 0) {
      perror("Server select send response from cgi error");
    }
 
    // Close connection
    RemoveClient(fd);
  }
}

void Server::AcceptClient(int listen_fd) {
  sockaddr_in client_address;
  socklen_t address_length = sizeof(client_address);

  int client_fd = Accept(listen_fd, (sockaddr *)&client_address, &address_length);
  client_map_[client_fd] = std::make_shared<ClientInfo>(client_fd, client_address);
  FD_SET(client_fd, &active_fds);
}

void Server::RemoveClient(int client_fd) {
  client_map_.erase(client_fd);
  Close(client_fd);
  FD_CLR(client_fd, &active_fds);
}

int Server::ReadClient(int client_fd, char *buf, int buf_size) {
  int bytes_read = read(client_fd, buf, buf_size);
  if (bytes_read < 0) {
    perror("Error: read");
    return -1;
  }

  return bytes_read;
}

}
