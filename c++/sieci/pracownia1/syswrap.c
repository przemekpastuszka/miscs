/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "syswrap.h"

int pp_socket(int family, int type, int protocol){
    int	n;
    if ( (n = socket(family, type, protocol)) < 0){
        ERROR ("socket error");
    }
    return n;
}

void pp_bind(int fd, const struct sockaddr_in *sa, socklen_t salen){
    if (bind(fd, (struct sockaddr*)sa, salen) < 0)
        ERROR ("bind error");
}

int pp_recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr){
    if (recvfrom(fd, ptr, nbytes, flags, (struct sockaddr*)sa, salenptr) < 0){
        if(errno == EWOULDBLOCK){
            return -1;
        }
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

void pp_fcntl(int socket, int cmd, int opt){
    if(fcntl(socket, cmd, opt) < 0){
        ERROR("fcntl error");
    }
}
