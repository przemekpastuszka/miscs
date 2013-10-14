/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "syswrap.h"
#include <unistd.h>

int pp_fork(){
    int n = fork();
    if(n < 0){
        ERROR("fork error");
    }
    return n;
}

void pp_close(int fd){
    if(close(fd) < 0){
        ERROR("socket closing error");
    }
    // printf("Connection %d closed\n", fd);
}

int pp_recv(int fd, void* buff, int len, int flags){
    int n = recv(fd, buff, len, flags);
    if(n < 0){
        ERROR("recv error");
    }
    return n;
}

void pp_listen(int fd, int max_connections){
    if(listen(fd, max_connections) < 0){
        ERROR("listen error");
    }
}

int pp_accept(int fd){
    struct sockaddr_in client_address;
    socklen_t len = sizeof(client_address);
    
    int new_sd = accept(fd, (struct sockaddr*)&client_address, &len);
    if(new_sd < 0){
        ERROR("accept error");
    }
    
    char ip[20];
    inet_ntop(AF_INET, &client_address.sin_addr, ip, sizeof(ip));
    printf("New connection with %s; Number: %d\n", ip, new_sd); 
    
    return new_sd;
}

void pp_send(int fd, const void *msg, int len, int flags){
    if(send(fd, msg, len, flags) < 0){
        ERROR("send error");
    }
}

void pp_select(int nfds, fd_set *restrict readfds,
              fd_set *restrict writefds, fd_set *restrict errorfds,
              struct timeval *restrict timeout){
                  
    if (select(nfds, readfds, writefds, errorfds, timeout) < 0){
        ERROR ("select error");
    }
}

int pp_socket(int family, int type, int protocol){
    int	n;
    if ( (n = socket(family, type, protocol)) < 0){
        ERROR ("socket error");
    }
    return n;
}

void pp_bind(int fd, const struct sockaddr_in *sa, socklen_t salen){
    if (bind(fd, (struct sockaddr*)sa, salen) < 0){
        ERROR ("bind error");
    }
}

int pp_recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr){
    if (recvfrom(fd, ptr, nbytes, flags, (struct sockaddr*)sa, salenptr) < 0){
        ERROR ("recvfrom error");
    }
    return 0;
}

void pp_sendto(int fd, const void *ptr, int nbytes, int flags, const struct sockaddr_in *sa, socklen_t salen){
    if (sendto(fd, ptr, nbytes, flags, (struct sockaddr*)sa, salen) != nbytes){
        ERROR ("sendto error");
    }
}

void pp_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen){
    if (setsockopt(sockfd,level,optname,optval,optlen) < 0){
        ERROR ("setsockopt error");
    }
}

void* pp_malloc(size_t size){
    void* ptr = malloc(size);
    if(ptr == NULL){
        ERROR("malloc error");
    }
    return ptr;
}

void pp_getaddrinfo(const char* node, const char* service, const struct addrinfo *hints, struct addrinfo **res){
    if (getaddrinfo(node, service, hints, res) < 0){
        ERROR ("getaddrinfo error");
    }
}

void pp_fcntl(int socket, int cmd, int opt){
    if(fcntl(socket, cmd, opt) < 0){
        ERROR("fcntl error");
    }
}
