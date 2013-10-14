/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "recieve.h"
#include "send.h"
#include "syswrap.h"

int main(int argc, char** argv){
    
    if (argc != 4){ 
        printf ("Usage: ./ppclient <port> <nazwa_pliku> <rozmiar_pliku>\n"); exit(1);
    }
    
    char* port = argv[1]; // przygotuj argumenty wywołania
    char* file_name = argv[2];
    int file_size = atoi(argv[3]);
    
    int socket = pp_socket(AF_INET, SOCK_DGRAM, 0); // stwórz gniazdo
    
    // pobranie adresu IP serwera
    struct addrinfo hints;
    struct addrinfo* servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    pp_getaddrinfo(SERVER, port, &hints, &servinfo);
    struct sockaddr_in* address = (struct sockaddr_in*)servinfo -> ai_addr;
    
    char buffer[MAX_FILE_SIZE]; // bufor na plik wynikowy
    
    struct Queue q; // stwórz kolejkę przechowującą informacje o żądaniach dla serwera
    init_queue(MAX_GROUP_SIZE, &q);
    
    // algorytm działa następująco: wyślij CURR_GROUP_SIZE pakietów z żądaniem do serwera
    // następnie oczekuj na odpowiedź. Jeśli ta nadejdzie, usuń odpowiednią informację
    // z kolejki, wyślij kolejne żądanie i wróć do oczekiwania na odpowiedź.
    
    for(int i = 0; i < file_size; i += 1000){ // wyślij żądanie dla każdej kolejnej paczki 1000-bitowej
        int len = 1000;
        if(file_size - i < 1000){ // jeśli wielkość pliku nie jest wielokrotnością 1000
            len = file_size - i;
        }
        
        if(i / 1000 < CURR_GROUP_SIZE){ // wysyła CURR_GROUP_SIZE pakietów
            send_request(socket, address, i, len, &q);
            continue;
        }
        recive_request(socket, address, &q, buffer); // poczekaj na odpowiedź
        printf("Sending: %d\n", 100 * i / file_size);
        send_request(socket, address, i, len, &q); // odpowiedź nadeszła - wyślij kolejne żądanie
    }
    
    // wysłano wszystkie żądania
    printf("Waiting for few last packets...\n");
    while(!empty(&q)){ // oczekiwanie na ostatnie odpowiedzi
        recive_request(socket, address, &q, buffer);
    }
    printf("Done\n");
    
    FILE* file = fopen(file_name, "wb"); // zapisz wynik do pliku
    fwrite(buffer, sizeof(char), file_size, file);
    fclose(file);
    
    freeaddrinfo(servinfo); // czyszczenie
    free_queue(&q);
    return 0;
}
