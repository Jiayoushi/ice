#include <iostream>
#include <string>

#include "settings.h"
#include "server.h"

int main(int argc, char *argv[]) {
  ice::InitializeSettings(argc, argv);
  ice::Server server;
  server.Run();	  
  return 0;
}
