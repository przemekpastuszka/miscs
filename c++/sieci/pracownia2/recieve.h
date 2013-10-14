/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#ifndef PP_RECIVE
#define PP_RECIVE

#include <arpa/inet.h>
#include "queue.h"

void recive_request(int socket, struct sockaddr_in* addr, struct Queue* q, char* buffer);

#endif
