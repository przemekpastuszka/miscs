/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#include <stdlib.h>
#include "queue.h"
#include "syswrap.h"

void init_queue(int max_size, struct Queue* q){
    q -> elements = (struct UDPRequest*)pp_malloc((sizeof(int) * 2 + sizeof(struct timeval)) * max_size);
    q -> size = 0;
}
void free_queue(struct Queue* q){
    free(q -> elements);
}
int empty(struct Queue*  q){
    return q -> size <= 0;
}

// usuwa element w najprostszy możliwy sposób - przesuwa pozostałe elementy o jedną pozycję w lewo
void del_element(int pos, struct Queue* q){
    for(int k = pos + 1; k < q -> size; ++k){
        q -> elements[k - 1] = q -> elements[k];
    }
    --q -> size;
}
void push(struct UDPRequest element, struct Queue* q){
    q -> elements[q -> size++] = element;
}
