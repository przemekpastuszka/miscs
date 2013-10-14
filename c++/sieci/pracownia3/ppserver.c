/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 3 z przedmiotu Sieci Komputerowe
 * 
 * 5/06/2011
 */

#include <stdio.h>
#include <stdlib.h>
#include "syswrap.h"
#include "server_start.h"
#include "server_response.h"

#define MAX_CONNECTIONS 10

int main(int argc, char** argv){
    
    if (argc != 3){ 
        printf("Usage: ./ppclient <port> <katalog>\n");
        exit(1);
    }
    
    int port = atoi(argv[1]); // przygotuj argumenty wywołania
    char* dir = argv[2];
    
    int sd = start_server(port, MAX_CONNECTIONS); // wystartuj serwer
    while(1){ // serwer może zostać zabity tylko odpowiednim sygnałem
        int new_sd = pp_accept(sd); // przyjmij przychodzące połączenie
        int pid = pp_fork(); // nowy fork
        
        if(pid){
            pp_close(new_sd); // jesteśmy w głównej pętli
        }
        else{
            pp_close(sd); // jesteśmy w forku
            serve(new_sd, dir); // obsłuż klienta
            
            pp_close(new_sd);
            exit(0);
        }
    }
    
    pp_close(sd);
    
    return 0;
}
