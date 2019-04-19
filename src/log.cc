#include "log.h"

#include <iostream>
#include <cstring>

namespace ice {

Log log;

Log::Log() {

}

void LogMsg(const std::string &s) {
  log.LogMsg(s);
}

void InitLog(const std::string &file) {
  log.Initialize(file);
}

Log::Log(const std::string &file):
  file_(file), 
  file_stream_(file, std::ios::out) {
  
}

void Log::Initialize(const std::string &file) {
  file_ = file;
  file_stream_.open(file);
  if (!file_stream_.is_open()) {
    std::cout << "Error: failed to open log file" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Log::LogMsg(const char *s) {
  file_stream_.write(s, strlen(s));
  file_stream_.flush();
}

void Log::LogMsg(const std::string &s) {
  file_stream_ << s << std::endl;
  file_stream_.flush();
}

}
