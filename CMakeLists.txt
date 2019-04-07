cmake_minimum_required(VERSION 3.5)
project(ice)
include_directories(include)
add_executable(ice 
               src/main.cc 
               src/server.cc 
               src/network.cc 
               src/http.cc 
               src/response.cc 
               src/settings.cc
               libs/http_parser.cc)
