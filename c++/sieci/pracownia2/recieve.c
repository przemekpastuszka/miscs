/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "recieve.h"
#include "syswrap.h"
#include "send.h"

// policz różnicę między czasami a oraz b i podaj ją w milisekundach
int tv_diff_to_ms(struct timeval* a, struct timeval* b){
    return (b -> tv_sec - a -> tv_sec) * 1000 +
        (b -> tv_usec - a -> tv_usec) / 1000;
}

// uzupełnij strukturę timeval czasem w milisekundach
void ms_to_tv(int ms, struct timeval* b){
    b -> tv_sec = ms / 1000;
    b -> tv_usec = (ms % 1000) * 1000;
}

// sprawdź czy oba adresy i porty są zgodne
int check_address(struct sockaddr_in* remote, struct sockaddr_in* good){
    return remote -> sin_port == good -> sin_port && 
        remote -> sin_addr.s_addr == good -> sin_addr.s_addr;
}

// sprawdź, czy żądanie o parametrach start i length znajduje się w kolejce q
// jeśli tak, usuń je
int in_queue(int start, int length, struct Queue* q){
    for(int i = 0; i < q -> size; ++i){
        struct UDPRequest req = q -> elements[i];
        if(req.start == start && req.length == length){
            del_element(i, q);
            return 1;
        }
    }
    return 0;
}

void recive_request(int socket, struct sockaddr_in* addr, struct Queue* q, char* buffer){
    fd_set tmp;
    struct timeval zero;
    zero.tv_sec = zero.tv_usec = 0;
    
    while(1){ // nieustannie oczekuję na odpowiedź
        FD_ZERO(&tmp);
        FD_SET(socket, &tmp);
        
        struct timeval timeout;
        gettimeofday(&timeout, NULL);
        
        // elementy w kolejce ustawione są rosnąco według czasów wysłania
        // wystarczy więc sprawdzić czy pierwszy z nich nie przekroczył TIMEOUTu
        
        struct UDPRequest first = q -> elements[0];
        int diff = tv_diff_to_ms(&first.time, &timeout);
        if(diff > TIMEOUT){ // TIMEOUT przekroczony
            del_element(0, q); // usuń element z kolejki
            send_request(socket, addr, first.start, first.length, q); // ponownie wyślij żądanie
            continue; // kontynuuj (być może następne żądanie też przekroczyło limit czasu)
        }
        
        // na odpowiedź poczekamy nie dłużej niż limit czasu dla najstarszego żądania
        ms_to_tv(TIMEOUT - diff + 1, &timeout); // za TIMEOUT - diff + 1 milisekund najstarsze żądanie się przeterminuje
        pp_select(socket + 1, &tmp, NULL, NULL, &timeout); // czekaj na odpowiedź
        if(FD_ISSET(socket, &tmp)){ // otrzymano odpowiedź
            struct sockaddr_in remote_address;
            socklen_t len = sizeof(remote_address);
            
            char rec_buffer[MAX_RESPONSE_SIZE];
            pp_recvfrom(socket, rec_buffer, MAX_RESPONSE_SIZE, 0, &remote_address, &len); // przeczytaj odpowiedź
            if(check_address(&remote_address, addr)){ // porównaj adresy
                int start, length;
                sscanf(rec_buffer, "DATA %d %d\n", &start, &length);
                if(in_queue(start, length, q)){ // jeśli otrzymaliśmy odpowiedź na żądanie, które znajduje się w kolejce
                    char* p = rec_buffer;
                    while(*(p++) != '\n'); // przejdź do miejsca, gdzie zaczynają się dane
                    memcpy(buffer + start, p, length); // i skopiuj je do bufora
                    break;
                }
            }
        }
    }
}
