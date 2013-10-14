/*
 * Przemysław Pastuszka
 * nr albumu: 233 186
 * 
 * Zadanie na pracownię nr 3 z przedmiotu Sieci Komputerowe
 * 
 * 5/06/2011
 */


#include <string.h>
#include "syswrap.h"
#include "server_start.h"

// implementacja najprostsza z możliwych
int start_server(int port, int max_connections){
    int sd = pp_socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    pp_bind(sd, &addr, sizeof(addr));
    pp_listen(sd, max_connections);
    return sd;
}
