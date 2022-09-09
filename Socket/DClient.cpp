#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

typedef struct 
{
    int id;
    int address;
    char name[10];

} TPacket;

int main(int argc, char **argv) {
    int aSockFd;
    socklen_t aClient;
    TPacket aPack;
    struct sockaddr_un aClientAddr, aServerAddr;

    aSockFd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (aSockFd < 0) {
        perror("socket error : ");
        exit(0);
    }

    bzero(&aServerAddr, sizeof(aServerAddr));
    bzero(&aClientAddr, sizeof(aClientAddr));

    aClientAddr.sun_family = AF_UNIX;
    strcpy(aClientAddr.sun_path, "#Client");
    aClientAddr.sun_path[0] = 0;

    if (bind(aSockFd, (const struct sockaddr *) &aClientAddr, sizeof(aClientAddr)) < 0){
        perror("bind error : ");
        exit(0);
    }

    aServerAddr.sun_family = AF_UNIX;
    strcpy(aServerAddr.sun_path, argv[1]);
    aServerAddr.sun_path[0] = 0;

    aPack.id = atoi(argv[2]);
    aPack.address = atoi(argv[3]);
    strcpy(aPack.name, argv[4]);

    if (sendto(aSockFd, (void *)&aPack, sizeof(aPack), 0, (struct sockaddr *)&aServerAddr, sizeof(aServerAddr)) < 0) {
        perror("send error : ");
        exit(0);
    }


    aClient = sizeof(aServerAddr);
    if (recvfrom(aSockFd, (void *)&aPack, sizeof(aPack), 0, (struct sockaddr *)&aServerAddr, &aClient) < 0) {
        perror("recv error : ");
        exit(0);
    }
    printf("Check -> ID : %d / Address : %d / Name : %s \n", aPack.id, aPack.address, aPack.name);

    close(aSockFd);
    exit(0);
}
