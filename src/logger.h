#ifndef ICE_LOGGER_H
#define ICE_LOGGER_H

#include <string>
#include <fstream>

namespace ice {

void Log(const std::string &s);
void InitLogger(const std::string &log_file);


class Logger {
 public:
  Logger();
  Logger(const std::string &log_file);

  void Initialize(const std::string &log_file);
  void Log(const char *s);
  void Log(const std::string &s);

 private:
  std::string log_file_;
  std::ofstream file_stream_;
};

extern Logger logger;

}



#endif
