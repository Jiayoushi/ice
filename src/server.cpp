#include <iostream>

#include "server.h"
#include "network.h"

Server::Server() {
}

Server::~Server() {
}

void Server::Connect() {
  listen_file_descriptor = network::SetUp();
}

void Server::Loop() {
  while (true) {
    char buffer[1024];
    int client_file_descriptor = network::Accept(listen_file_descriptor);
    int length = network::Read(client_file_descriptor, buffer, 1024);
    network::Write(client_file_descriptor, buffer, length);
  }
}

void Server::Run() {
  Connect();
  Loop();
}
