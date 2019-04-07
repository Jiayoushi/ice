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
  file_stream_.open(log_file);
  if (!file_stream_.is_open()) {
    std::cout << "Error: failed to open log file" << std::endl;
    exit(EXIT_FAILURE);
  }
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
