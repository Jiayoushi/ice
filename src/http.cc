#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>

namespace ice {

http_parser_settings settings;
std::string target_header_field;

const size_t kSmallBufferSize = 256;

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

  size_t nparsed = 0;
  nparsed = http_parser_execute(parser, &settings, message, msg_len);
  if (nparsed != msg_len) {
    http_request.valid = false;
  }

  free(parser);
}

int OnUrlCallback(http_parser *parser, const char *at, size_t len) {
  HttpRequest *http_request = (HttpRequest *)parser->data;
  std::unordered_map<std::string, std::string> &data = http_request->data;
  data["Url"] = std::string(at, len);

  http_parser_url url_parser;
  http_parser_url_init(&url_parser);  
  
  if (http_parser_parse_url(at, len, 0, &url_parser) != 0) {
    perror("Error: http url parsing");
    exit(EXIT_FAILURE);
  }

  data["Url-Schema"] = std::string(at + url_parser.field_data[UF_SCHEMA].off, \
                                   url_parser.field_data[UF_SCHEMA].len);
  data["Url-Host"] = std::string(at + url_parser.field_data[UF_HOST].off, \
                                 url_parser.field_data[UF_HOST].len);
  data["Url-Port"] = std::string(at + url_parser.field_data[UF_PORT].off, \
                                 url_parser.field_data[UF_PORT].len);
  data["Url-Path"] = std::string(at + url_parser.field_data[UF_PATH].off, \
                                 url_parser.field_data[UF_PATH].len); 
  data["Url-Query"] = std::string(at + url_parser.field_data[UF_QUERY].off, \
                                  url_parser.field_data[UF_QUERY].len); 
  data["Url-Fragment"] = std::string(at + url_parser.field_data[UF_FRAGMENT].off, \
                                     url_parser.field_data[UF_FRAGMENT].len);  
  data["Url-UserInfo"] = std::string(at + url_parser.field_data[UF_USERINFO].off, \
                                     url_parser.field_data[UF_USERINFO].len);  
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
  HttpRequest *http_request = (HttpRequest *)parser->data;
  http_request->data["Method"] = std::string(http_method_str(http_method(parser->method)));
  return 0;
}

}
