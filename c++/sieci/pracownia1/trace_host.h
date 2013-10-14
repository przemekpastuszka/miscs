/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#ifndef PP_TRACE_HOST
#define PP_TRACE_HOST

#include "request.h"
#include "trace_results.h"

// wyślij results -> nr_of_packets pakietów do serwera określonego przez data,
// z podanym ttl. Adresy serwerów, które odpowiedziały, zapisz do results, łącznie
// z czasami tych odpowiedzi.
int trace_host(int socket, struct RequestData* data, int ttl, int time_limit,
    struct TraceResults* results);

#endif
