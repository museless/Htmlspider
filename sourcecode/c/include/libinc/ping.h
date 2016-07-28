#ifndef _PING_H
#define _PING_H


/*-----------------------------
           include
-------------------------------*/

#include "spinc.h"
#include "network.h"

#include "icmp.h"

#include <netinet/ip.h>
#include <arpa/inet.h>


/*-----------------------------
            define
-------------------------------*/

#define MAX_PADDATA_LEN     0x40

#define PING_WAIT_SEC       0x1
#define PING_WAIT_USEC      0x280

#define PING_SEND_WAIT_SEC  0x2
#define PING_SEND_WAIT_USEC 0x0


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
