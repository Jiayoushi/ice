#ifndef ICE_HTTP_H_
#define ICE_HTTP_H_

#include <string>
#include <unordered_map>

#include "../libs/http_parser.h"

namespace ice {

// Method
// Url
// Host
// Accept
// Connection
// User-Agent
// Accept-Encoding
// Accept-Language

// Url-Schema
// Url-Host
// Url-Port
// Url-Path
// Url-Query
// Url-Fragment
// Url-UserInfo
struct HttpRequest {
  bool valid;
  std::unordered_map<std::string, std::string> data;
  std::string empty_string;

  HttpRequest():
    data(), valid(true) {
  }

  const std::string & Get(const std::string &key) const {
    if (data.find(key) == data.end()) {
      return empty_string;
    } else {
      return data.at(key);
    }
  }
};

void InitHttpParserSettings();
void ParseHttpMessage(const char *message, size_t msg_len, struct HttpRequest &http_info);
int OnUrlCallback(http_parser *parser, const char *at, size_t len);
int OnHeaderField(http_parser *parser, const char *at, size_t len);
int OnHeaderValue(http_parser *parser, const char *at, size_t len);
int OnBody(http_parser *parser, const char *at, size_t len);
int OnMessageComplete(http_parser *parser);
int OnHeadersComplete(http_parser *parser);
}



#endif
