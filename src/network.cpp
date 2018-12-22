#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "network.h"

namespace ice {

int Socket(int domain, int type, int protocol) {
  int listen_fd = socket(domain, type, protocol);
  if (listen_fd < 0) {
    perror("Error: failed to set up socket ");
    exit(EXIT_FAILURE);
  }
  return listen_fd;
}

void Bind(int listen_fd, sockaddr_in &server_address) {
  if (bind(listen_fd, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
}

void Listen(int listen_fd, int max_pending_connections) {
  if (listen(listen_fd, max_pending_connections) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

void SetServerAddress(sockaddr_in &server_address) {
  memset(&server_address, sizeof(server_address), 0);  
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080);
}

void SetSocketOptions(int listen_fd) {
  int optval = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0) {
    perror("Error: setsockopt ");
    exit(EXIT_FAILURE);
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
    perror("Error: accept");
    exit(EXIT_FAILURE);
  }

  return client_fd;
}

int Read(int client_fd, char *buffer, const size_t length) {
  int length_read = read(client_fd, buffer, length);
  if (length_read < 0) {
    perror("Error: read");
    exit(EXIT_FAILURE);
  }

  return length_read;
}

int Write(int client_fd, char *buffer, const size_t length) {
  int length_write = write(client_fd, buffer, length);
  if (length_write < 0) {
    perror("Error: write");
    exit(EXIT_FAILURE);
  }

  return length_write;
}

void Select(int fd_limit, fd_set *read_fds, \
            fd_set *write_fds, fd_set *exceptions,       \
            struct timeval *timeout) {
  if (select(fd_limit, read_fds, \
             write_fds, exceptions, timeout) < 0) {
    perror("Error: select");
    exit(EXIT_FAILURE);
  }
}


}

