#include <stdlib.h>

#include "http.h"
#include <iostream>

namespace ice {

http_parser_settings settings;
std::string target_header_field;

void InitHttpParserSettings() {
  settings.on_url = OnUrlCallback;  
  settings.on_header_field = OnHeaderField;
  settings.on_header_value = OnHeaderValue;
  settings.on_body = OnBody;
  settings.on_headers_complete = OnHeadersComplete;
  settings.on_message_complete = OnMessageComplete;
}

void ParseHttpMessage(const char *message, size_t msg_len, struct HttpRequest &http_request) {
  http_parser *parser = (http_parser *)malloc(sizeof(http_parser));
  http_parser_init(parser, HTTP_REQUEST);
  parser->data = &http_request;

  int nparsed = 0;
  nparsed = http_parser_execute(parser, &settings, message, msg_len);

  free(parser);
}

int OnUrlCallback(http_parser *parser, const char *at, size_t len) {
  HttpRequest *http_request = (HttpRequest *)parser->data;
  http_request->data["url"] = std::string(at, len);

  for (size_t i = 0; i < len; ++i) {
    std::cout << *(at + i);
  }
  std::cout << std::endl;
  return 0;
}

int OnHeaderField(http_parser *parser, const char *at, size_t len) {
  target_header_field = std::string(at, len);
  return 0;
}

int OnHeaderValue(http_parser *parser, const char *at, size_t len) {
  HttpRequest *http_request = (HttpRequest *)parser->data;
  http_request->data[target_header_field] = std::string(at, len);
  return 0;
}

int OnBody(http_parser *parser, const char *at, size_t len) {
  return 0;
}

int OnMessageComplete(http_parser *parser) {
  return 0;
}

int OnHeadersComplete(http_parser *parser) {
  return 0;
}

}
