#include "server.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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
  auto result = request_handler_map.find(fd);
  if (result == request_handler_map.end()) {
    perror("client_map error: unmatched fd");
    return;
  }

  RequestHandler &ci = *(result->second);
  // fd matches client_fd, read from client and prepare response
  if (fd == ci.client_fd) {
    int &client_fd = fd;

    // Read from the client
    char message[kMaxMessageLen];
    int message_len = ReadRequest(client_fd, message, kMaxMessageLen);

    // Parse client's message
    ParseHttpMessage(message, message_len, ci.http_request);

    // Get response according to parsed request
    HandlerResult rs = ci.GetResponse();
    // If it's a valid response or an error response
    if (rs.first == kNormalResponseCompleted) {
      ci.SendResponse();
      RemoveRequest(client_fd);
    // If not, let select waits for cgi
    } else if (rs.first == kWaitForCgi) {
      int child_fd = rs.second;
      FD_SET(child_fd, &active_fds);
      request_handler_map[child_fd] = result->second;
    } else {
      perror("HandleRequest: unmatched HandlerResult");
      RemoveRequest(client_fd);
    }
  // Cgi has responded
  } else {
    int &child_fd = fd;
    // Wait for this child to finish
    if (waitpid(ci.child_pid, NULL, 0) < 0) {
      perror("Wait for cgi child failed: ");
    } else {
      // Read from cgi
     char response[kMaxBufSize];
      int bytes_read = read(child_fd, response, kMaxBufSize);
      if (bytes_read < 0) {
        perror("HandleRequest read response from cgi error");
      } else {
        response[bytes_read] = '\0';
      }
      // Send to client
      Write(ci.client_fd, response, bytes_read);
    }
 
    RemoveRequest(child_fd);
    // Close connection
    RemoveRequest(ci.client_fd);
  }
}

void Server::AcceptConnection(int listen_fd) {
  sockaddr_in client_address;
  socklen_t address_length = sizeof(client_address);

  int client_fd = Accept(listen_fd, (sockaddr *)&client_address, &address_length);
  request_handler_map[client_fd] = std::make_shared<RequestHandler>(client_fd, client_address);
  FD_SET(client_fd, &active_fds);
}

void Server::RemoveRequest(int client_fd) {
  request_handler_map.erase(client_fd);
  Close(client_fd);
  FD_CLR(client_fd, &active_fds);
}

int Server::ReadRequest(int client_fd, char *buf, int buf_size) {
  int bytes_read = read(client_fd, buf, buf_size);
  if (bytes_read < 0) {
    perror("Error: read");
    return -1;
  }

  return bytes_read;
}

}
