#ifndef _PING_H
#define _PING_H


/*-----------------------------
           include
-------------------------------*/

#include "icmp.h"
#include "spinc.h"
#include "spnet.h"
#include <netinet/ip.h>
#include <arpa/inet.h>


/*-----------------------------
            define
-------------------------------*/

#define MAX_PADDATA_LEN     0x40

#define PING_WAIT_SEC       0x0
#define PING_WAIT_USEC      0x280


/*-----------------------------
           typedef 
-------------------------------*/

typedef struct  sockaddr_in SOCKIF;
typedef struct  addrinfo    ADDRIF;

typedef struct  ping_info   PINGIF;


/*-----------------------------
           typedef 
-------------------------------*/

struct  ping_info {
    uLong  pi_rrt;
    uInt   pi_ttl;
    uInt   pi_seq;
};


/*-----------------------------
        global function
-------------------------------*/

int     ping(PINGIF *ping_info, const char *host);


#endif
