/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "syswrap.h"
#include "trace_results.h"
#include "trace_host.h"

#define PACKETS_TO_SEND 3 // ile pakietów należy przesłać w jednej iteracji
#define MAX_TTL 30 // wysyłaj pakiety z TTL należącym do zbioru {1, ..., MAX_TTL}
#define TIME_LIMIT 1000 // maksymalny czas oczekiwania na odpowiedź (w milisekundach)

int main (int argc, char** argv){
    if (argc != 2){ 
        printf ("Usage: ./pptrace <ip>\n"); exit(1);
    }
    
    int socket = pp_socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    pp_fcntl(socket, F_SETFL, O_NONBLOCK); // przełączenie gniazda w tryb nieblokujący
    
    // request przechowuje dane konieczne do wysłania pakietu ICMP ECHO
    // są to: docelowe ip, id programu (pid) oraz numer w sekwencji
    struct RequestData request;
    fill_request_data(argv[1], getpid(), &request);
    
    // przechowuje adresy serwerów, które odpowiedziały i czasy tych odpowiedzi
    struct TraceResults results;
    init_trace_results(&results, PACKETS_TO_SEND);
    int target_reached = 0; // czy ICMP dotarło do docelowego serwera
    
    for(int i = 1; i <= MAX_TTL && target_reached == 0; ++i){
        printf("%d. ", i);
        
        // wyślij PACKETS_TO_SEND pakietów o ttl równym i
        // k jest równe liczbie serwerów, które odpowiedziały
        int k = trace_host(socket, &request, i, TIME_LIMIT, &results);
        
        // wypisywanie numerów ip
        for(int j = 0; j < PACKETS_TO_SEND; ++j){
            if(results.ips[j][0] == '\0'){ // nie było odpowiedzi na j-ty pakiet
                continue;
            }
            
            // czy odpowiedź nadeszła od serwera docelowego
            if(strcmp(argv[1], results.ips[j]) == 0){
                target_reached = 1;
            }
            
            // przed wypisaniem adresu ip należy sprawdzić czy już tego wcześniej
            // nie zrobiliśmy
            int is_unique = 1;
            for(int z = 0; z < j; ++z){
                if(strcmp(results.ips[z], results.ips[j]) == 0){ // takie ip zostało już wypisane
                    is_unique = 0;
                    break;
                }
            }
            if(is_unique){
                printf("%s ", results.ips[j]);
            }
        }
        
        // wypisywanie informacji o czasie odpowiedzi
        if(k == 0){ // nikt nie odpowiedział
            printf("*\n");
        }
        else{
            if(k == PACKETS_TO_SEND){ // otrzymano wszystkie odpowiedzi - policz średni czas
                long int sum = 0;
                for(int j = 0; j < k; ++j){
                    sum += results.intervals[j];
                }
                printf("%ldms\n", sum / k);
            }
            else{
                printf("???\n");
            }
        }
        
    }
    
    free_trace_results(&results); // zwolnij zasoby
    
    return 0;
}
