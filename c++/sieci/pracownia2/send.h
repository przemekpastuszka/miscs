/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#ifndef PP_SEND
#define PP_SEND

#include <arpa/inet.h>
#include "queue.h"

void send_request(int socket, struct sockaddr_in* addr, int start, int length, struct Queue* q);

#endif
