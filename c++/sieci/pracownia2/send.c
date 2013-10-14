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
#include "send.h"
#include "syswrap.h"

// generuje string postaci GET start length
void create_request_string(int start, int length, char* str){
    sprintf(str, "GET %d %d\n", start, length);
}

void send_request(int socket, struct sockaddr_in* addr, int start, int length, struct Queue* q){
    char request[25]; // wyślij żądanie do serwera
    create_request_string(start, length, request);
    pp_sendto(socket, request, strlen(request), 0, addr, sizeof(struct sockaddr_in));
    
    struct UDPRequest r; // zapisz informację o tym żądaniu w kolejce
    r.start = start;
    r.length = length;
    gettimeofday(&r.time, NULL);
    push(r, q);
}
