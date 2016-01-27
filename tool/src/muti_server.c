#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    struct sockaddr_in  sock_addr, mcast_addr;

    mcast_addr.sin_family = AF_INET;
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    mcast_addr.sin_port = htons(PORT);

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(LOCAL_ADDR);
    sock_addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
        perror("main - bind");
        exit(-1);
    }

    char    buffer[] = "This is a muticast";
    int     serv_struct_len = sizeof(mcast_addr);

    while (true) {
        printf("sending udp pack now...");

        error = sendto(
                sock, buffer, strlen(buffer) + 1, 0, 
                (struct sockaddr *)&mcast_addr, serv_struct_len);

        if (error == -1) {
            perror("main - sendto");
            return  -1;
        }

        sleep(5);
    }

    exit(-1);
}
