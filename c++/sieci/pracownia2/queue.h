/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#ifndef PP_QUEUE
#define PP_QUEUE

#include "request.h"

// prosta kolejka struktur UDPRequest
// zaimplementowana na tablicy
struct Queue{
    struct UDPRequest *elements;
    int size;
};

// operacje na kolejce
void init_queue(int max_size, struct Queue* q);
void free_queue(struct Queue* q);
int empty(struct Queue* q);
void del_element(int pos, struct Queue* q);
void push(struct UDPRequest element, struct Queue* q);

#endif
