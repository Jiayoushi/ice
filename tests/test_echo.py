#!/usr/bin/env python3

import socket

HOST = '127.0.0.1'
PORT = 8080

MESSAGE = b'Hello world'

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(MESSAGE)
    data = s.recv(1024)
    assert data == MESSAGE
