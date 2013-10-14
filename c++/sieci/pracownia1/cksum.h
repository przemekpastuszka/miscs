/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 1 z przedmiotu Sieci Komputerowe
 * 
 * 5/04/2011
 */

#ifndef PP_CKSUM
#define PP_CKSUM

#define ICMP_HEADER_LEN 8

u_short in_cksum(const u_short *addr, register int len, u_short csum);

#endif

