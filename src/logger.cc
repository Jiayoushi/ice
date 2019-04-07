#include "logger.h"

#include <iostream>
#include <cstring>

namespace ice {

Logger logger;

Logger::Logger() {

}

void Log(const std::string &s) {
  logger.Log(s);
}

void InitLogger(const std::string &log_file) {
  logger.Initialize(log_file);
}

Logger::Logger(const std::string &log_file):
  log_file_(log_file), 
  file_stream_(log_file, std::ios::out) {
  
}

void Logger::Initialize(const std::string &log_file) {
  log_file_ = log_file;
  file_stream_ = std::move(std::ofstream(log_file_, std::ios::out));
}

void Logger::Log(const char *s) {
  file_stream_.write(s, strlen(s));
  file_stream_.flush();
}

void Logger::Log(const std::string &s) {
  file_stream_ << s << std::endl;
  file_stream_.flush();
}

}
