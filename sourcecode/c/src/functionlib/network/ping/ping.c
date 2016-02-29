/*---------------------------------------------
 *     modification time: 2016-02-29 10:27
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-10-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source code contain Six part
 *
 *       Part Zero:  Include
 *       Part One:   Define
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Ping main
 *       Part Five:  Network initialize
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "ping.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/


/*---------------------------------------------
 *           Part Two: Local data
-*---------------------------------------------*/


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

/* Part Five */
static  int     ping_access(int sock, PINGIF *ping_info, SOCKIF *sock_info);
static  int     ping_socket_get(void);
static  int     ping_send_recv(int sock, SOCKIF *sock_info, char *buffer);
static  long    ping_calculate_rrt(TMVAL *send_tim, TMVAL *recv_tim);


/*---------------------------------------------
 *          Part Four: Ping main
 *
 *          1. ping
 *
-*---------------------------------------------*/

/*-----ping-----*/
int ping(PINGIF *ping_info, const char *host)
{
    SOCKIF  sock_info;
    ADDRIF *addr_info;
    int     sock;

    if (!inet_aton(host, &sock_info.sin_addr)) {
        if (getaddrinfo(host, NULL, NULL, &addr_info))
            return  FRET_N;

        sock_info.sin_addr = (((SOCKIF *)(addr_info->ai_addr))->sin_addr);
    }

    if (!(sock = ping_socket_get()))
        return  FRET_N;

    sock_info.sin_family = AF_INET;
    sock_info.sin_port = 0;

    memset(ping_info, 0, sizeof(PINGIF));
    
    if (ping_access(sock, ping_info, &sock_info) == FRET_N) {
        close(sock);
        return  FRET_N;
    }

    close(sock);

    return  FRET_P;
}


/*---------------------------------------------
 *        Part Five: Network initialize
 *
 *        1. ping_access
 *        2. ping_socket_get
 *        3. ping_send_recv
 *        4. ping_set_rrt
 *
-*---------------------------------------------*/

/*-----ping_access-----*/
static int ping_access(int sock, PINGIF *ping_info, SOCKIF *sock_info)
{
    char    buffer[DG_ICMP_MAXLEN];
    TMVAL   send_time, recv_time;
    int     recv_num;

    if (!icmp_create(
         buffer, DG_ICMP_MAXLEN,
         TYPE_ECHO, CODE_ECHO, NULL, 0))
        return  FRET_N;

    gettimeofday(&send_time, NULL);

    if ((recv_num = ping_send_recv(sock, sock_info, buffer)) == FRET_N)
        return  FRET_N;

    recv_num -= DG_IP_LEN;

    if (!icmp_resolve(
         (uChar *)(buffer + DG_IP_LEN),
         recv_num, TYPE_ECHO_REPLY, CODE_ECHO_REPLY))
        return  FRET_N;

    gettimeofday(&recv_time, NULL);

    ping_info->pi_rrt = ping_calculate_rrt(&send_time, &recv_time);
 
    return  FRET_P;  
}


/*-----ping_socket_get-----*/
int ping_socket_get(void)
{
    struct  protoent    *protocal;
    int     sock;

    if (!(protocal = getprotobyname("icmp")))
        return  FRET_N;

    if ((sock = socket(AF_INET, SOCK_RAW, protocal->p_proto)) == FRET_N)
        return  FRET_N;

    if(sp_net_sock_settimer(
       sock, PING_WAIT_SEC, PING_WAIT_USEC, SO_RCVTIMEO) == FRET_N) {
        close(sock);
        return  FRET_N;
    }
 
    if(sp_net_sock_settimer(
       sock, PING_WAIT_SEC, PING_WAIT_USEC, SO_SNDTIMEO) == FRET_N) {
        close(sock);
        return  FRET_N;
    }

    return  sock;
}


/*-----ping_send_recv-----*/
int ping_send_recv(int sock, SOCKIF *sock_info, char *buffer)
{
    socklen_t   sockif_len = sizeof(struct sockaddr);
    int         recv_num;
    
    if (sendto(
        sock, buffer, TYPE_ECHO_MIN_LEN, 0, 
        (const struct sockaddr *)sock_info, sockif_len) == FRET_N)
        return  FRET_N;

    if ((recv_num = recvfrom(
        sock, buffer, DG_ICMP_MAXLEN, 0, 
        (struct sockaddr *)sock_info, &sockif_len)) == FRET_N)
        return  FRET_N;
    
    return  recv_num;
}


/*-----ping_calculate_rrt-----*/
long ping_calculate_rrt(TMVAL *send_tim, TMVAL *recv_tim)
{
    return  (recv_tim->tv_usec > send_tim->tv_usec) ?
            (recv_tim->tv_usec - send_tim->tv_usec) : 0; 
}
