#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
    struct sockaddr_in aClientAddr, aServerAddr;

    aSockFd = socket(PF_INET, SOCK_DGRAM, 0);
    if (aSockFd < 0) {
        perror("socket error : ");
        exit(0);
    }

    bzero(&aServerAddr, sizeof(aServerAddr));
    aServerAddr.sin_family = AF_INET;
    aServerAddr.sin_port = htons(4000);
    aServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

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
