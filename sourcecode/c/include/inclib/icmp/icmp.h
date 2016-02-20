#ifndef _ICMP_H
#define _ICMP_H


/*-----------------------------
           include
------------------------------*/

#include "spinc.h"


/*-----------------------------
           define 
------------------------------*/

#define FRET_P                      1
#define FRET_Z                      0
#define FRET_N                      -1

#define DG_IP_LEN                   0x14
#define DG_ICMP_MINLEN              0x1A
#define DG_ICMP_MAXLEN              0x5C0

/* type num and code num */
/* form - one type contain much code num */
#define TYPE_ECHO_REPLY             0x0
#define CODE_ECHO_REPLY             0x0
#define TYPE_ECHO_REPLY_MIN_LEN     (sizeof(ICMPECHO))

#define TYPE_ERROR                  0x3
#define CODE_NETWORK_UNREACH        0x0
#define CODE_HOST_UNREACH           0x1
#define CODE_PROTO_UNREACH          0x2
#define CODE_PORT_UNREACH           0x3
#define CODE_NEED_BURST             0x4
#define CODE_ROUTE_FAIL             0x5
#define CODE_NETWORK_UNKNOWN        0x6
#define CODE_HOST_UNKNOWN           0x7
#define CODE_HOST_ABANDON           0x8
#define CODE_NETWORK_CLOSED         0x9
#define CODE_HOST_CLOSED            0xA
#define CODE_TOS_NETWORK_UNREACH    0xB
#define CODE_TOS_HOST_UNREACH       0xC
#define CODE_SHUT_BY_FILTER         0xD
#define CODE_EXCEED_HOST_AUTHORITY  0xE
#define CODE_NO_POWER               0xF

#define TYPE_ECHO                   0x8
#define CODE_ECHO                   0x0
#define TYPE_ECHO_MIN_LEN           TYPE_ECHO_REPLY_MIN_LEN


/*-----------------------------
           typedef
------------------------------*/

typedef struct  internet_ctl_msg_protocal   ICMP;
typedef struct  icmp_type_echo              ICMPECHO;


/*-----------------------------
            struct
------------------------------*/

struct internet_ctl_msg_protocal {
    uChar   icmp_type;
    uChar   icmp_code;
    uShort icmp_checksum;
};

struct icmp_type_echo {
    ICMP    icmp_entity;

    uShort icmp_echo_id;
    uShort icmp_echo_seqnum;

    uChar   icmp_pad[18];
};


/*-----------------------------
        access function
-------------------------------*/

int icmp_create(char *write_buf, int buf_len, uShort type,
                uShort code, const char *data, int data_len);
int icmp_resolve(uChar *data, int data_len, int type, int code);

#endif
