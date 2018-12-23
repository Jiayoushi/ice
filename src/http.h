#ifndef ICE_HTTP_H_
#define ICE_HTTP_H_


#include <string>
#include <unordered_map>

#include "../libs/http_parser.h"

namespace ice {

struct HttpRequest {
  std::unordered_map<std::string, std::string> data;

  HttpRequest():
    data() {
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
