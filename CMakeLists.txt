cmake_minimum_required(VERSION 3.5)
project(ice)
include_directories(include)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(ice 
               src/main.cc 
               src/server.cc 
               src/network.cc 
               src/http.cc
               src/log.cc
               src/response.cc 
               src/settings.cc
               libs/http_parser.cc)
