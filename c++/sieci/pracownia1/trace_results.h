/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#ifndef PP_TRACE_RESULTS
#define PP_TRACE_RESULTS

// przechowuje adresy serwerów, które odpowiedziały, a także ich czasy odpowiedzi
struct TraceResults{
    int nr_of_packets; // ile pakietów wysłano
    char** ips; // adresy
    long int* intervals; // czasy
};

// inicjalizacja i destrukcja
void init_trace_results(struct TraceResults* result, int packets_nr);
void free_trace_results(struct TraceResults* result);

#endif
