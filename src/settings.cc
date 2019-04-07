#ifndef ICE_SETTINGS_H_
#define ICE_SETTINGS_H_

#include <string>
#include <iostream>

namespace ice {

std::string kHttpPort;
std::string kLogFile;
std::string kContentFolder;
std::string kCgiFolder;

std::string kHttpVersion = "HTTP/1.1";
std::string kServerInformation = "Server: ice";
std::string kGatewayInterface = "CGI/1.1";

void InitializeSettings(int argc, char *argv[]) {
  if (argc != 5) {
    std::cout << "Usage: ./ice <HTTP port> <log file> <content folder> <cgi folder>"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  
  kHttpPort = std::string(argv[1]);
  kLogFile = std::string(argv[2]);
  kContentFolder = std::string(argv[3]);
  kCgiFolder = std::string(argv[4]);
}

}


#endif

