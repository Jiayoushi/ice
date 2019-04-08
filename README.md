# ice [![Build Status](https://travis-ci.org/Jiayoushi/ice.svg?branch=master)](https://travis-ci.org/Jiayoushi/ice)
A simple C/C++ select-based HTTP server that conforms to a subset of HTTP 1.1 RFC.
Supports GET, POST, HEAD, CGI and persistent connection.

## Installation
```
cmake .
make
```

## Usage
```
./ice <HTTP port> <log file> <content folder> <cgi folder>
```
