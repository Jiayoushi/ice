#!/usr/bin/env python3

import socket
import ipaddress


BASE_HOST = '127.0.0.1'
PORT = 8080
MESSAGE = b'Hello world from client'

num_clients = 100
clients = []
for i in range(num_clients):
    clients.append({'host': str(ipaddress.ip_address(BASE_HOST) + i), \
                    'socket':  socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                   })

for client in clients:
    host = client['host']
    client['socket'].connect((host, PORT))


for client in clients:
    client['socket'].sendall(MESSAGE)
    data = client['socket'].recv(1024)
    assert data == MESSAGE


for client in clients:
    client['socket'].close()
