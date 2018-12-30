#include <iostream>
#include <string>

#include "server.h"

int main(int argc, char *argv[]) {
  const std::string kBaseDirectory = std::string(argv[1]);

  ice::Server server(kBaseDirectory);
  server.Run();	  

  return 0;
}
