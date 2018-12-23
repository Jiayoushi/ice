#!/usr/bin/env python3

import socket
import ipaddress


BASE_HOST = '127.0.0.1'
PORT = 8080

GET1 = open('sample_request_realistic', 'r').read()
MESSAGE = GET1.encode()

num_clients = 1
clients = []
for i in range(num_clients):
    clients.append({'host': str(ipaddress.ip_address(BASE_HOST) + i), \
                    'socket':  socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                   })
# Connect
for client in clients:
    host = client['host']
    client['socket'].connect((host, PORT))


# Send and echo
for client in clients:
    client['socket'].sendall(MESSAGE)
    data = client['socket'].recv(1024)
    assert data == MESSAGE

# Close
for client in clients:
    client['socket'].close()
