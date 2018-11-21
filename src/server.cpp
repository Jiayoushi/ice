#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "server.h"


Server::Server() {
  std::cout << "Server begin construction" << std::endl;
}

Server::~Server() {
  std::cout << "Server begin deconstruction" << std::endl;
}

void Server::Connect() {
  server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (server_file_descriptor < 0) {
    perror("server file descriptor");
  }

  sockaddr_in server_address;
  memset(&server_address, sizeof(server_address), 0);  
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htons(INADDR_ANY);
  server_address.sin_port = htons(8080);

  sockaddr_in server;
  if (bind(server_file_descriptor, (sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
  }
  if (listen(server_file_descriptor, 128) < 0) {
    perror("listen");
  }
}

void Server::Loop() {
  char message[1024];
  int client_file_descriptor = accept(server_file_descriptor, (sockaddr *)NULL, NULL);
  if (client_file_descriptor < 0) {
    perror("ERROR");
  }

  std::cout << "connected" << " " << client_file_descriptor << std::endl;
  read(client_file_descriptor, message, 1024);
  std::cout << message << std::endl;
}

void Server::Run() {
  Connect();
  Loop();
}
