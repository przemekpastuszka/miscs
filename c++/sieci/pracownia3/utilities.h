/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 3 z przedmiotu Sieci Komputerowe
 * 
 * 5/06/2011
 */
 
#ifndef PP_UTILITIES
#define PP_UTILITIES

#define MOVED_PERMANENTLY 0
#define FORBIDDEN 1
#define NOT_FOUND 2
#define NOT_IMPLEMENTED 3

#include <sys/time.h>

void ms_to_tv(int ms, struct timeval* b); // wypełnia strukturę timeval
int parse_query(char* query, char* get, char* host, char* connection); // parsuje zapytania od klienta
void send_simple_message(int fd, int type, const char* location); // wysyła komunikaty 403, 404, 501, 301
void send_data(int fd, const char* path, FILE* file, int size); // przesyła plik

#endif
