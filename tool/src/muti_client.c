#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


/* define */
#define PORT        6000
#define MCAST_ADDR  "224.0.0.10"
#define LOCAL_ADDR  "127.0.0.1"


/*-----main-----*/
int main(void)
{
    int     sock, error;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("main - socket");
        exit(-1);
    }

    struct sockaddr_in  sock_addr;

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr) == -1)) {
        perror("main - bind");
        exit(-1);
    }

    int     loop = 1;

    error = setsockopt(
            sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

    if (error == -1) {
        perror("main - setsockopt - IPPROTO_IP");
        exit(-1);
    }

    struct ip_mreq  muti_req;

    muti_req.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
    muti_req.imr_interface.s_addr = inet_addr(LOCAL_ADDR);

    error = setsockopt(
            sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &muti_req, sizeof(muti_req));

    if (error == -1) {
        perror("main - setsockopt - IP_ADD_MEMBERSHIP");
        exit(-1);
    }

    char        recv_buff[256];
    socklen_t   serv_struct_len = sizeof(sock_addr);

    recvfrom(
    sock, recv_buff, 256, 0, (struct sockaddr *)&sock_addr, &serv_struct_len);

    sleep(20);

    error = setsockopt(
            sock, IPPROTO_IP, 
            IP_DROP_MEMBERSHIP, &muti_req, sizeof(muti_req));

    if (error == -1) {
        perror("main - setsockopt - IP_DROP_MEMBERSHIP");
        exit(-1);
    }

    exit(-1);
}
