#include "network.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "logger.h"
#include "settings.h"

namespace ice {

int Socket(int domain, int type, int protocol) {
  int listen_fd = socket(domain, type, protocol);
  if (listen_fd < 0) {
    Log("Error: failed to set up socket ");
  }
  return listen_fd;
}

void Bind(int listen_fd, sockaddr_in &server_address) {
  if (bind(listen_fd, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
    Log("bind");
  }
}

void Listen(int listen_fd, int max_pending_connections) {
  if (listen(listen_fd, max_pending_connections) < 0) {
    Log("listen");
  }
}

void SetServerAddress(sockaddr_in &server_address) {
  memset(&server_address, 0, sizeof(sockaddr_in));  
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(stoi(kHttpPort));
}

void SetSocketOptions(int listen_fd) {
  int optval = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0) {
    Log("Error: setsockopt ");
  }
}

int SetUp() {
  int listen_fd = Socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_address;
  SetServerAddress(server_address);
  SetSocketOptions(listen_fd);
  Bind(listen_fd, server_address);
  Listen(listen_fd, 128);
  return listen_fd;
}

int Accept(int listen_fd, sockaddr *address, socklen_t *address_length) {
  int client_fd = accept(listen_fd, address, address_length);
  if (client_fd < 0) {
    Log("Error: accept");
  }

  return client_fd;
}

void Close(int fd) {
  if (close(fd) < 0) {
    Log("Error: close");
  }
}

int Read(int client_fd, char *buffer, const size_t length) {
  int length_read = read(client_fd, buffer, length);
  if (length_read < 0) {
    Log("Error: read");
  }

  return length_read;
}

int Write(int client_fd, const char *buffer, const size_t length) {
  if (length == 0) {
    Log("Write error: attempt to send 0 bytes");
    return -1;
  }

  int total_write = 0;
  int w = 0;
  while (total_write != length) {
    w = write(client_fd, buffer, length - total_write); 
    if (w < 0) {
      Log("Write error");
    } 

    total_write += w;
  } 
  
  return total_write;
}

void Select(int fd_limit, fd_set *read_fds,
            fd_set *write_fds, fd_set *exceptions,
            struct timeval *timeout) {
  if (select(fd_limit, read_fds,
             write_fds, exceptions, timeout) < 0) {
    Log("Error: select");
  }
}


}

