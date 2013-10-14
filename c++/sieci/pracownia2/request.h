/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 2 z przedmiotu Sieci Komputerowe
 * 
 * 21/04/2011
 */

#ifndef PP_REQUEST
#define PP_REQUEST

#include <sys/time.h>

// przechowuje informacje o żądaniu przesłanym do serwera
struct UDPRequest{
    int start, length; // zażądano length bitów począwszy od pozycji start
    struct timeval time; // czas, kiedy wysłano żądanie
};

#endif
