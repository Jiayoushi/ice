#include "server.h"

#include <iostream>

Server::Server() {
  std::cout << "Server begin construction" << std::endl;
}

Server::~Server() {
  std::cout << "Server begin deconstruction" << std::endl;
}

void Server::Run() {
  std::cout << "Run" << std::endl;
}
