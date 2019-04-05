#!/usr/bin/env python3

import sys
import socket
import ipaddress
import subprocess
import atexit
import time

# Server
server = subprocess.Popen(['../ice', '..'])
atexit.register(server.kill)

time.sleep(1)

# Client
BASE_HOST = '127.0.0.1'
PORT = 8080
NUM_OF_CLIENTS = 10

requests_folder = 'requests/'
response_postfix = '_response'
requests = [('get_home', 'r'), ('get_ico', 'rb'), ('invalid_get', 'r'), ('unfound_url', 'r'), ('test_basic_cgi', 'r')]

for request in requests:
    with open(requests_folder + request[0], 'r') as f:
        REQUEST = f.read().encode()
        with open(requests_folder + request[0] + response_postfix, request[1]) as f:
            if request[1] == 'r':
                EXPECTED_RESPONSE = f.read().encode()
            else:
                EXPECTED_RESPONSE = f.read()

    num_clients = NUM_OF_CLIENTS
    clients = []
    for i in range(num_clients):
        clients.append({'host': str(ipaddress.ip_address(BASE_HOST) + i), \
                        'socket':  socket.socket(socket.AF_INET, socket.SOCK_STREAM)})
    # Connect
    for client in clients:
        host = client['host']
        client['socket'].connect((host, PORT))

    # Send and echo
    for client in clients:
        client['socket'].sendall(REQUEST)

    for client in clients:
        data = client['socket'].recv(10240)
        if data != EXPECTED_RESPONSE: 
            print('Test case ' + request[0] + ' failed.')
            print('Expect: -------------------')
            print(EXPECTED_RESPONSE)
            print('Get: ----------------------')
            print(data)
            sys.exit(1)

    # Close
    for client in clients:
        client['socket'].close()
    print("Test case " + request[0] + " passed.")
    

print("All tests passed.")
