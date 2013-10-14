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
#include "utilities.h"
#include "syswrap.h"

void ms_to_tv(int ms, struct timeval* b){
    b -> tv_sec = ms / 1000;
    b -> tv_usec = (ms % 1000) * 1000;
}

int parse_query(char* query, char* get, char* host, char* connection){
    char label[100];
    char info[1000];
    
    int c = 0;
    
    // czytaj do końca żądania lub dopóki nie znaleziono wszystkich informacji
    while(*query != '\0' && c != 3){ 
        if(*query == '\n'){
            ++query;
        }
        
        if(sscanf(query, "%99s %999s", label, info) < 2){
            return -1;
        }
        if(strcmp(label, "GET") == 0){
            strcpy(get, info); ++c;
        }
        if(strcmp(label, "Host:") == 0){
            strcpy(host, info); ++c;
            char *pointer = strrchr(host, ':'); // usuwa informację o porcie
            if(pointer){
                *pointer = '\0';
            }
        }
        if(strcmp(label, "Connection:") == 0){
            strcpy(connection, info); ++c;
        }
        while(*query != '\n' && *query != '\0'){ // przejdź do następnej linii
            ++query;
        }
        
    }
    
    return c;
}

void send_simple_message(int fd, int type, const char* location){
    char* text[] = {"301 Moved Permanently", "403 Forbidden", "404 Not Found", "501 Not Implemented"};
    char message[600];
    
    char location_msg[300];
    location_msg[0] = 0;
    if(type == MOVED_PERMANENTLY){ // w przypadku Moved Permanently trzeba podać ścieżkę
        sprintf(location_msg, "Location: %s\n", location);
    }
    
    sprintf(message, "HTTP/1.1 %s\n%sContent-Length: %d\nContent-Type: text/plain\n\n%s",
        text[type], location_msg, (int) strlen(text[type]), text[type]);
    
    pp_send(fd, message, strlen(message), 0);
}

// ustawia nazwę MIME w zależności od rozszerzenia zasobu
void set_type(const char* path, char* type){
    char* pointer = strrchr(path, '.');
    if(pointer){
        if(strcmp(pointer, ".txt") == 0){
            strcpy(type, "text/plain");
        }
        if(strcmp(pointer, ".html") == 0){
            strcpy(type, "text/html");
        }
        if(strcmp(pointer, ".css") == 0){
            strcpy(type, "text/css");
        }
        if(strcmp(pointer, ".jpg") == 0 || strcmp(pointer, ".jpeg") == 0){
            strcpy(type, "image/jpeg");
        }
        if(strcmp(pointer, ".png") == 0){
            strcpy(type, "image/png");
        }
        if(strcmp(pointer, ".pdf") == 0){
            strcpy(type, "application/pdf");
        }
    }
    else{
        strcpy(type, "application/octet-stream");
    }
}

void send_data(int fd, const char* path, FILE* file, int size){
    char* response = (char*) pp_malloc(sizeof(char) * (size + 300));
    
    char type[100];
    set_type(path, type);
    
    sprintf(response, "HTTP/1.1 200 OK\nContent-Length: %d\nContent-Type: %s\n\n", size, type);
    int len = strlen(response);
    fread(response + len, 1, size, file); // wczytaj zawartość pliku
    
    pp_send(fd, response, len + size, 0); // wyślij
    
    free(response);
}
