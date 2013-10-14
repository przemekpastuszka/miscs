/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#ifndef PP_REQUEST
#define PP_REQUEST

#include <netinet/ip_icmp.h>
#include "cksum.h"

// przechowuje dane konieczne do wysłania pakietu ICMP ECHO
struct RequestData{
    struct sockaddr_in address; // adres docelowy
    struct icmp packet; // zawartość pakietu
    int seq; // ilość wysłanych do tej pory pakietów
    int id; // id programu / pakietu
};

void fill_request_data(char* ip, int id, struct RequestData* data); // wypełnia strukturę RequestData
void send_request(int socket, int ttl, struct RequestData* data); // wysyła pakiet

#endif
