/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#include "request.h"
#include "syswrap.h"

// przygotuj informacje konieczne do wysłania pakietu ICMP ECHO
// z podanym id do komputera o danym ip
void fill_request_data(char* ip, int id, struct RequestData* data){
    data -> address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &data -> address.sin_addr);
    
    data -> packet.icmp_type = ICMP_ECHO;
    data -> packet.icmp_code = 0;
    data -> packet.icmp_id = id;
    
    data -> seq = 0;
    data -> id = id;
}

// wyślij pakiet ICMP ECHO przez gniazdo socket z ustalonym ttl
void send_request(int socket, int ttl, struct RequestData* data){
    data -> packet.icmp_seq = data -> seq++; // przypisz kolejny numer sekwencji
    data -> packet.icmp_cksum = 0;
    data -> packet.icmp_cksum = in_cksum((u_short*)&data -> packet, 8, 0); // policz sumę kontrolną
    
    pp_setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)); // ustaw ttl
    pp_sendto(socket, &data -> packet, ICMP_HEADER_LEN, 0, &data -> address, sizeof(struct sockaddr_in));
}
