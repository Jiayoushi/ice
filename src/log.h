#ifndef ICE_LOG_H_
#define ICE_LOG_H_

#include <string>
#include <fstream>

namespace ice {

void LogMsg(const std::string &s);
void InitLog(const std::string &log_file);


class Log {
 public:
  Log();
  Log(const std::string &file);

  void Initialize(const std::string &file);
  void LogMsg(const char *s);
  void LogMsg(const std::string &s);

 private:
  std::string file_;
  std::ofstream file_stream_;
};

extern Log log;

}



#endif
