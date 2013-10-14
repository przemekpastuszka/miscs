/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 3 z przedmiotu Sieci Komputerowe
 * 
 * 5/06/2011
 */

#ifndef PP_SYSWRAP
#define PP_SYSWRAP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }


// tu znajdują się funkcje opakowujące wywołania systemowe
// ich najważniejszym zadaniem jest przechwytywanie ewentualnych błędów

int pp_socket(int family, int type, int protocol);
void pp_bind(int fd, const struct sockaddr_in *sa, socklen_t salen);
int pp_recv(int fd, void* buff, int len, int flags);
int pp_recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr);
void pp_send(int fd, const void *msg, int len, int flags);
void pp_sendto(int fd, const void *ptr, int nbytes, int flags, const struct sockaddr_in *sa, socklen_t salen);
void pp_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
void pp_getaddrinfo(const char* node, const char* service, const struct addrinfo *hints, struct addrinfo **res);
void* pp_malloc(size_t size);
void pp_fcntl(int socket, int cmd, int opt);
void pp_select(int nfds, fd_set *restrict readfds,
              fd_set *restrict writefds, fd_set *restrict errorfds,
              struct timeval *restrict timeout);
int pp_fork();
void pp_close(int fd);
int pp_accept(int fd);
void pp_listen(int fd, int max_connections);
#endif
