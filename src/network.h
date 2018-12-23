#ifndef ICE_NETWORK_H_
#define ICE_NETWORK_H_

#include <netinet/in.h>
#include <sys/select.h>

namespace ice {

int Socket(int domain, int type, int protocol);
void Bind(int listen_fd, sockaddr_in &server_address);
void Listen(int listen_fd, int max_pending_connections);
void SetServerAddress(sockaddr_in &server_address);
void SetSocketOptions(int listen_fd);
int Accept(int listen_fd, sockaddr *address, socklen_t *address_length);
void Close(int fd);
int Read(int client_fd, char *buffer, const size_t length);
int Write(int client_fd, char *buffer, const size_t length);

// fd_limit should be set to the highest-numbered file descriptor 
// in any of the three sets, plus 1.  
void Select(int fd_limit, fd_set *read_fds, fd_set *write_fds, fd_set *exceptions,       \
            struct timeval *timeout); 

int SetUp();

}


#endif
