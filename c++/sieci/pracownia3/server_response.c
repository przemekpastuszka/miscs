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
#include <string.h>
#include <sys/stat.h>
#include "syswrap.h"
#include "utilities.h"
#include "server_response.h"

#define MAX_REQUEST_SIZE 10000 // maksymalny rozmiar przychodzącego od przeglądarki pakietu
#define TIMEOUT 1500 // przez ile milisekund utrzymywać połączenie

void serve(int fd, char* directory){
    char query[MAX_REQUEST_SIZE];
    fd_set tmp;
    
    while(1){
        FD_ZERO(&tmp);
        FD_SET(fd, &tmp);
        
        struct timeval timeout;
        ms_to_tv(TIMEOUT, &timeout); // odczekaj TIMEOUT milisekund
        pp_select(fd + 1, &tmp, NULL, NULL, &timeout);
        if(FD_ISSET(fd, &tmp)){ // otrzymano zapytanie od klienta
            pp_recv(fd, query, MAX_REQUEST_SIZE - 1, 0); // pobierz je
            
            // wyciągnij trzy informacje z zapytania: ścieżkę get, nazwę hosta i parametry połączenia
            char get[1000], host[1000], connection[1000];
            int i = parse_query(query, get, host, connection);
            if(i < 3){ // nie udało się znaleźć trzech powyższych informacji
                printf("Wrong query on %d socket (%d)\n", fd, i);
                send_simple_message(fd, NOT_IMPLEMENTED, ""); // 501
                break;
            }
            printf("From %d: GET %s on host %s. Connection %s\n", fd, get, host, connection);
            
            
            char path[300];
            sprintf(path, "%s/%s%s", directory, host, get);
            
            // żądana ścieżka zawiera '../', zatem może potencjalnie odnosić się do katalogu, do którego klient nie ma dostępu
            if(strstr(path, "../")){ 
                send_simple_message(fd, FORBIDDEN, "");
            }
            
            FILE* file = fopen(path, "rb");
            if(file){ // czy ścieżka jest poprawna (tj. żądany zasób istnieje)
                struct stat statbuffer;
                stat(path, &statbuffer);
                if(S_ISDIR(statbuffer.st_mode)){ // czy ścieżka reprezentuje katalog
                    char location[200];
                    if(get[strlen(get) - 1] != '/'){
                        sprintf(location, "%s/index.html", get);
                    }
                    else{
                        sprintf(location, "%sindex.html", get);
                    }
                    send_simple_message(fd, MOVED_PERMANENTLY, location); // jeśli tak to przekieruj w odpowiednie miejsce
                }
                else{
                    send_data(fd, path, file, statbuffer.st_size); // prześlij żądany plik
                }
                fclose(file);
            }
            else{
                send_simple_message(fd, NOT_FOUND, ""); // zasób nie istnieje
            }
            
            if(strcmp(connection, "close") == 0){ // klient ma życzenie, aby nie utrzymywać połączenia
                break;
            }
        }
        else{
            break;
        }
    }
}
