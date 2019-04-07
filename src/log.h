#ifndef ICE_LOG_H
#define ICE_LOG_H

#include <string>

namespace ice {

class Log {
 public:
  Log(const std::string &s);

 private:
  std::string log_file_;
};

}



#endif
