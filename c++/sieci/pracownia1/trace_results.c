/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#include <stdlib.h>
#include "trace_results.h"
#include "syswrap.h"

// inicjalizuje TraceResults. Nic ciekawego
void init_trace_results(struct TraceResults* result, int packets_nr){
    result -> nr_of_packets = packets_nr;
    result -> ips = (char**)pp_malloc(packets_nr * sizeof(char*));
    for(int i = 0; i < packets_nr; ++i){
        result -> ips[i] = (char*)pp_malloc((INET_ADDRSTRLEN + 1) * sizeof(char));
    }
    result -> intervals = (long int*)pp_malloc(packets_nr * sizeof(long int));
}

// zwalnia zasoby zajmowane przez TraceResults
void free_trace_results(struct TraceResults* result){
    for(int i = 0; i < result -> nr_of_packets; ++i){
        free(result -> ips[i]);
    }
    free(result -> ips);
    free(result -> intervals);
}
