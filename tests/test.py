#!/usr/bin/env python3

import socket
import ipaddress
import subprocess
import atexit
import time

# Server
server = subprocess.Popen(['../ice'])
atexit.register(server.kill)
print(server.pid)

time.sleep(1)

# Client
BASE_HOST = '127.0.0.1'
PORT = 8080

with open('sample_request_realistic', 'r') as f:
    GET1 = f.read()

with open('../content/home.html', 'r') as f:
    EXPECTED_RESPONSE = f.read().encode()

REQUEST = GET1.encode()

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
    client['socket'].sendall(REQUEST)
    data = client['socket'].recv(1024)
    assert data == EXPECTED_RESPONSE

# Close
for client in clients:
    client['socket'].close()
