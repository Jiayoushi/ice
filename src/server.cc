#include "server.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

#include "network.h"
#include "settings.h"


namespace ice {

Server::Server() {
  InitHttpParserSettings();
  InitContentMapping();
  InitLogger(kLogFile);
}

Server::~Server() {
}

void Server::Connect() {
  listen_fd_ = SetUp();
}

void Server::Loop() {
  FD_ZERO(&active_fds_);
  FD_SET(listen_fd_, &active_fds_);

  while (true) {
    read_fds_ = active_fds_;

    Select(FD_SETSIZE, &read_fds_, nullptr, nullptr, nullptr);
    for (size_t fd = 0; fd < FD_SETSIZE; ++fd) {
      if (FD_ISSET(fd, &read_fds_)) {
        if (fd == listen_fd_) {
          AcceptConnection(fd);
        } else {
          HandleRequest(fd);
        }
      }
    }
  }
}

void Server::Run() {
  Connect();
  Loop();
}

void Server::HandleRequest(int fd) {
  auto result = request_handler_map_.find(fd);
  if (result == request_handler_map_.end()) {
    Log("client_map error: unmatched fd");
    return;
  }

  RequestHandler &ci = *(result->second);
  // fd matches client_fd, read from client and prepare response
  if (fd == ci.GetClientFd()) {
    int &client_fd = fd;

    // Read from the client
    char message[kMaxMessageLen];
    int message_len = ReadRequest(client_fd, message, kMaxMessageLen);
    if (message_len == 0) {
      RemoveMapping(client_fd);
      return;
    }
    
    // Parse client's message
    ParseHttpMessage(message, message_len, ci.GetHttpRequest());

    // Get response according to parsed request
    HandlerResult rs = ci.GetResponse();
    // If it's a valid response or an error response
    if (rs.first == kNormalResponseCompleted) {
      ci.SendResponse();
    // If not, let select waits for cgi
    } else if (rs.first == kWaitForCgi) {
      int child_fd = rs.second;
      FD_SET(child_fd, &active_fds_);
      request_handler_map_[child_fd] = result->second;
    } else {
      Log("HandleRequest: unmatched HandlerResult");
      RemoveMapping(client_fd);
    }
  // Cgi has responded
  } else {
    int &child_fd = fd;
    // Wait for this child to finish
    if (waitpid(ci.GetChildPid(), NULL, 0) < 0) {
      Log("Wait for cgi child failed: ");
    } else {
      // Read from cgi
      char response[kMaxBufSize];
      int bytes_read = read(child_fd, response, kMaxBufSize);
      if (bytes_read < 0) {
        Log("HandleRequest read response from cgi error");
      } else {
        response[bytes_read] = '\0';
      }
      // Send to client
      Write(ci.GetClientFd(), response, bytes_read);
    }
 
    RemoveMapping(child_fd);
  }
}

void Server::AcceptConnection(int listen_fd) {
  sockaddr_in client_address;
  socklen_t address_length = sizeof(client_address);

  int client_fd = Accept(listen_fd, (sockaddr *)&client_address, &address_length);
  request_handler_map_[client_fd] = std::make_shared<RequestHandler>(client_fd, client_address);
  FD_SET(client_fd, &active_fds_);

  Log("Accept client, assigned file descriptor: " + std::to_string(client_fd));
}

void Server::RemoveMapping(int client_fd) {
  request_handler_map_.erase(client_fd);
  Close(client_fd);
  FD_CLR(client_fd, &active_fds_);
  Log("Remove client, assigned file descriptor: " + std::to_string(client_fd));
}

int Server::ReadRequest(int client_fd, char *buf, int buf_size) {
  int bytes_read = read(client_fd, buf, buf_size);
  if (bytes_read < 0) {
    Log("Error: read");
    return -1;
  } else if (bytes_read == 0) {
    Log("Client " + std::to_string(client_fd) + " has closed connection");
  }

  return bytes_read;
}

}
