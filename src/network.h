#ifndef NETWORK_H_
#define NETWORK_H_

#include <netinet/in.h>

namespace network {

int Socket(int domain, int type, int protocol);
void Bind(int listen_file_descriptor, sockaddr_in &server_address);
void Listen(int listen_file_descriptor, int max_pending_connections);
void SetServerAddress(sockaddr_in &server_address);
void SetSocketOptions(int listen_file_descriptor);
int Accept(int listen_file_descriptor);
int Read(int client_file_descriptor, char *buffer, const size_t length);
int Write(int client_file_descriptor, char *buffer, const size_t length);


int SetUp();







}


#endif
