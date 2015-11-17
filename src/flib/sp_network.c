/*------------------------------------------
    Source file content Seven part

    Part Zero:  Include
    Part One:   Define
    Part Two:   Local data
    Part Three: Local function

    Part Four:  Socket control
    Part Five:  Http protocal 
    Part Six:   Ping

--------------------------------------------*/


/*------------------------------------------
                Include
--------------------------------------------*/

#include "spinc.h"
#include "spnet.h"
#include "sphtml.h"

#include "ping.h"


/*------------------------------------------
                Include
--------------------------------------------*/

#define ENDHTML_CHK_OFFSET 0x100


/*------------------------------------------
              Local function
--------------------------------------------*/

/* Part Four */
static  int     sp_net_sock_settimer(int socket, int nSec, int uSec, int nFlags);


/*------------------------------------------
        Part Four: Socket control

        1. sp_net_set_sockif
        2. sp_net_sock_connect
        3. sp_net_sock_read
        4. sp_net_sock_settimer

--------------------------------------------*/

/*-----sp_net_set_sockif-----*/
int sp_net_set_sockif(const char *hostName, SOCKIF *sInfo)
{
    struct  hostent *pHost;

    while(!(pHost = gethostbyname(hostName))) {
        if(h_errno)
            return  FUN_RUN_END;
    }

    sInfo->sin_family = AF_INET;
    sInfo->sin_port = htons(80);
    sInfo->sin_addr = *((struct in_addr *)(pHost->h_addr));

    return  FUN_RUN_OK;
}


/*-----sp_net_sock_connect-----*/
int sp_net_sock_connect(SOCKIF *sockInfo)
{
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == FUN_RUN_FAIL)
        return  FUN_RUN_END;

    if (sp_net_sock_settimer(
        sock, TAKE_A_NOTHING, 0, SO_SNDTIMEO) == FUN_RUN_FAIL) {
        close(sock);
        return  FUN_RUN_END;
    }

    if (connect(sock, (SOCKAD *)sockInfo, sizeof(SOCKIF)) == FUN_RUN_FAIL) {
        close(sock);
        return  FUN_RUN_END;
    }

    return  sock;
}


/*-----sp_net_sock_read-----*/
int sp_net_sock_read(
    int nSock, char *savBuf, int bufLimit, 
    int readTimes, int nSec, long microSec)
{
    char    *check_point;
    int     cont_offset, str_size, zero_times = 8;

    for (cont_offset = 0; readTimes > 0 && zero_times; readTimes--) {
        if ((str_size = select_read(nSock, savBuf + cont_offset, 
           RECE_DATA, nSec, microSec)) == FUN_RUN_FAIL)
            break;

        if (str_size == 0) {
            zero_times--;
            continue;
        }

        cont_offset += str_size;
        check_point = savBuf + (cont_offset - (str_size > ENDHTML_CHK_OFFSET) ? 
                      ENDHTML_CHK_OFFSET : str_size);

        if ((check_point = strnstr(check_point, MATCH_ENDHTML, ENDHTML_CHK_OFFSET)))
            break;

        if (cont_offset + RECE_DATA > bufLimit)
            break;
    }

    savBuf[cont_offset] = 0;

    printf("Total size: %d\n", cont_offset);

    return  cont_offset;
}


/*-----sp_net_sock_settimer------*/
static int sp_net_sock_settimer(int socket, int nSec, int uSec, int nFlags)
{
    TMVAL  time_val;

    if(nFlags != SO_RCVTIMEO && nFlags != SO_SNDTIMEO)
        return  FUN_RUN_FAIL;

    time_val.tv_sec = nSec;
    time_val.tv_usec = uSec;

    return  setsockopt(socket, SOL_SOCKET, nFlags, &time_val, sizeof(time_val));
}


/*------------------------------------------
        Part Five: Http protocal

        1. sp_http_interact

--------------------------------------------*/

/*-----sp_http_interact-----*/
int sp_http_interact(WEB *wbStru, int nSock, char *strBuf, int bufSize)
{
    short   strSize, rdSize;

    strSize = sprintf(strBuf, HTTP_GFILE_STR, wbStru->web_path, 
                        wbStru->web_file, wbStru->web_host, rPac);
    
    if(write(nSock, strBuf, strSize) != strSize) 
        return  FUN_RUN_END;

    if((rdSize = select_read(nSock, strBuf, bufSize, TAKE_A_SEC, 0)) < FRET_VAL)
        return  FUN_RUN_END;

    strBuf[rdSize] = 0;

    return  rdSize;
}


/*------------------------------------------
        Part Six: Ping

        1. sp_net_speed_ping

--------------------------------------------*/

/*-----sp_net_speed_ping-----*/
long sp_net_speed_ping(const char *ping_host, int num_pack)
{
    PINGIF  ping_info;
    long    total_time;
    int     count;

    for (count = total_time = 0; count < num_pack; ) {
        if (ping(&ping_info, ping_host) != FUN_RUN_OK) {
            perror("Ping---> ping error");
            num_pack--;
            continue;
        }

        total_time += ping_info.pi_rrt;
        count++;
    }

    return  (num_pack) ? (total_time / num_pack) : FUN_RUN_FAIL;
}
