#ifndef ICE_SETTINGS_H_
#define ICE_SETTINGS_H_

#include <string>

namespace ice {

// Usage: ./ice <HTTP port> <log file> <content folder> <cgi folder>

extern std::string kHttpPort;
extern std::string kLogFile;
extern std::string kContentFolder;
extern std::string kCgiFolder;

extern std::string kHttpVersion;
extern std::string kServerInformation;
extern std::string kGatewayInterface;

void InitializeSettings(int argc, char *argv[]);

}

#endif
