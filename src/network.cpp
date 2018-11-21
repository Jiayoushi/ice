#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "network.h"

namespace network {

int Socket(int domain, int type, int protocol) {
  int listen_file_descriptor = socket(domain, type, protocol);
  if (listen_file_descriptor < 0) {
    perror("Error: failed to set up socket ");
    exit(EXIT_FAILURE);
  }
  return listen_file_descriptor;
}

void Bind(int listen_file_descriptor, sockaddr_in &server_address) {
  if (bind(listen_file_descriptor, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
}

void Listen(int listen_file_descriptor, int max_pending_connections) {
  if (listen(listen_file_descriptor, max_pending_connections) < 0) {
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

void SetSocketOptions(int listen_file_descriptor) {
  int optval = 1;
  if (setsockopt(listen_file_descriptor, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)) < 0) {
    perror("Error: setsockopt ");
    exit(EXIT_FAILURE);
  }
}

int SetUp() {
  int listen_file_descriptor = Socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_address;
  SetServerAddress(server_address);
  SetSocketOptions(listen_file_descriptor);
  Bind(listen_file_descriptor, server_address);
  Listen(listen_file_descriptor, 128);
  return listen_file_descriptor;
}

int Accept(int listen_file_descriptor) {
  int client_file_descriptor = accept(listen_file_descriptor, (sockaddr *)NULL, NULL);
  if (client_file_descriptor < 0) {
    perror("Error: accept");
    exit(EXIT_FAILURE);
  }

  return client_file_descriptor;
}

int Read(int client_file_descriptor, char *buffer, const size_t length) {
  int length_read = read(client_file_descriptor, buffer, length);
  if (length_read < 0) {
    perror("Error: read");
    exit(EXIT_FAILURE);
  }

  return length_read;
}

int Write(int client_file_descriptor, char *buffer, const size_t length) {
  int length_write = write(client_file_descriptor, buffer, length);
  if (length_write < 0) {
    perror("Error: read");
    exit(EXIT_FAILURE);
  }

  return length_write;
}
}

