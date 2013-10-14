/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "trace_host.h"
#include "syswrap.h"

// oblicza różnicę między czasami a i b w milisekundach
long int tv_diff_to_ms(struct timeval* a, struct timeval* b){
    return (b -> tv_sec - a -> tv_sec) * 1000 +
        (b -> tv_usec - a -> tv_usec) / 1000;
}

// sprawdza czy pakiet packet, otrzymany w odpowiedzi od pewnego serwera,
// jest odpowiedzią na odpowiedni pakiet (tj. jeden z nr_of_packets ostatnich)
// wysłany przez nasz program. Jeśli tak to aktualizuje strukturę results
int check_and_set(struct icmp* packet, struct RequestData* data, int nr_of_packets,
    int interval, struct sockaddr_in* resp_address, struct TraceResults* results){
    
    // w ostatnim wywołaniu trace_host wysłanych zostało nr_of_packets pakietów
    // data -> seq przechowuje liczbę WSZYSTKICH wysłanych do tej pory pakietów
    // zatem jeśli packet został wysłany jako jeden z nr_of_packets ostatnich
    // to jego numer w tym wywołaniu trace_host to:
    // packet -> icmp_seq - (data -> seq - nr_of_packets)
    
    int seq = packet -> icmp_seq - data -> seq + nr_of_packets;
    if(packet -> icmp_id == data -> id && // id jest poprawne
        seq < nr_of_packets && // czy seq mieści się w {0, ..., nr_of_packets - 1}
        seq >= 0 &&
        results -> ips[seq][0] == '\0'){ // czy nie otrzymaliśmy już odpowiedzi na pakiet o tym numerze seq
            
            // uzupełnij wynikowe informacje
            results -> intervals[seq] = interval; // czas
            inet_ntop(AF_INET, &(resp_address -> sin_addr), results -> ips[seq], INET_ADDRSTRLEN); // adres
            
            return 1;
    }
    return 0;
}



int trace_host(int socket, struct RequestData* data, int ttl, int time_limit,
    struct TraceResults* results){
    
    int nr_of_packets = results -> nr_of_packets;
    
    // przygotuj miejsce na odpowiedź
    unsigned char response[IP_MAXPACKET + 1];
    struct sockaddr_in resp_address;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    
    // obsługa czasu
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    // wyślij nr_of_packets pakietów i wyczyść informacje o ip w results
    for(int j = 0; j < nr_of_packets; ++j){
        send_request(socket, ttl, data);
        results -> ips[j][0] = '\0';
    }
    
    // j oznacza liczbę otrzymanych odpowiedzi
    for(int j = 0; j < nr_of_packets;){
        
        gettimeofday(&end, NULL);
        int interval = tv_diff_to_ms(&start, &end); // czas jaki upłynął od wysłania pakietów
        if(interval > time_limit){ // przekroczono czas oczekiwania
            return j;
        }
        
        // pp_recvfrom sprawdza czy jakieś pakiety oczekują w kolejce na pobranie
        // jeśli nie zwraca -1
        if(pp_recvfrom(socket, response, IP_MAXPACKET, 0, &resp_address, &addr_len) < 0){
            continue;
        }
        
        struct ip* ip_resp = (struct ip*) response;
        struct icmp* icmp_resp = (struct icmp*) (response + ip_resp -> ip_hl * 4);
        
        // sprawdź czy masz do czynienia z pakietem typu ICMP TIME EXCEEDED
        if(icmp_resp -> icmp_type == ICMP_TIME_EXCEEDED && icmp_resp -> icmp_code == ICMP_EXC_TTL){
            
            // jeśli tak, to otrzymaliśmy również oryginalny pakiet ICMP ECHO
            struct ip* original = (struct ip*) (response + ip_resp -> ip_hl * 4 + ICMP_HEADER_LEN);
            icmp_resp = (struct icmp*) (response + ip_resp -> ip_hl * 4 + ICMP_HEADER_LEN + original -> ip_hl * 4);
            
            // jeśli wszystko jest ok, zwiększ liczbę otrzymanych odpowiedzi
            if(original -> ip_p == IPPROTO_ICMP && 
                check_and_set(icmp_resp, data, nr_of_packets, interval, &resp_address, results)){
                    ++j;
            }
        }
        
        // jak wyżej
        if (icmp_resp -> icmp_type == ICMP_ECHOREPLY &&
            check_and_set(icmp_resp, data, nr_of_packets, interval, &resp_address, results)){
                ++j;
        }
    }
    
    return nr_of_packets;
}
