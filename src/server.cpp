#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "server.h"


Server::Server() {
  std::cout << "Server begin construction" << std::endl;
}

Server::~Server() {
  std::cout << "Server begin deconstruction" << std::endl;
}

void Server::Connect() {
  listen_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_file_descriptor < 0) {
    perror("server file descriptor");
  }

  sockaddr_in server_address;
  //memset(&server_address, sizeof(server_address), 0);  
  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080);

  int optval = 1;
  setsockopt(listen_file_descriptor, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
  if (bind(listen_file_descriptor, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind");
  }
  if (listen(listen_file_descriptor, 128) < 0) {
    perror("listen");
  }
}

void Server::Loop() {
  char message[1024];
  int client_file_descriptor = accept(listen_file_descriptor, (sockaddr *)NULL, NULL);
  if (client_file_descriptor < 0) {
    perror("ERROR");
  }

  std::cout << "connected" << " " << client_file_descriptor << std::endl;
  int len = read(client_file_descriptor, message, 1024);
  write(client_file_descriptor, message, len);
  std::cout << message << std::endl;
}

void Server::Run() {
  Connect();
  Loop();
}
