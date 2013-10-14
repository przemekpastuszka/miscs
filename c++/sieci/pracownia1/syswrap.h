/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#ifndef PP_SOCKWRAP
#define PP_SOCKWRAP

#include <arpa/inet.h>


#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }


// tu znajdują się funkcje opakowujące wywołania systemowe
// ich najważniejszym zadaniem jest przechwytywanie ewentualnych błędów

int pp_socket(int family, int type, int protocol);
void pp_bind(int fd, const struct sockaddr_in *sa, socklen_t salen);
int pp_recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr);
void pp_sendto(int fd, const void *ptr, int nbytes, int flags, const struct sockaddr_in *sa, socklen_t salen);
void pp_setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
void* pp_malloc(size_t size);
void pp_fcntl(int socket, int cmd, int opt);

#endif
