#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 4000

typedef struct{

    int id;
    int address;
    char name[10];

} TPack;

int main(int argc, char **argv){ 
    TPack aPack, aTmpPack;
    struct sockaddr_un aServerAddress, aClientAddress;
    int aServerFd, aClientFd;
    int aClientSize;
    socklen_t aClientLen;
    
    memset(&aServerAddress, 0x00, sizeof(aServerAddress));
    memset(&aClientAddress, 0x00, sizeof(aClientAddress));

    //aServerAddress.sin_family = AF_INET;
    //aServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    //aServerAddress.sin_port = htons(PORT);
    unlink(argv[1]);
    aServerAddress.sun_family = AF_UNIX;
    strcpy(aServerAddress.sun_path, argv[1]);
    aServerAddress.sun_path[0] = 0;

    if ((aServerFd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1 ){
        printf("Server : Can not Open Socket \n");
        exit(0);
    }

    if (bind(aServerFd, (struct sockaddr *)&aServerAddress, sizeof(aServerAddress)) < 0) {
        printf("Server : Can not bind local address \n");
        exit(0);
    }

    printf("Server : Wating Connection ... \n");

    while (1) { 
        aClientLen = sizeof(struct sockaddr_un);
        recvfrom(aServerFd, (void *)&aPack, sizeof(aPack), 0, (struct sockaddr *)&aClientAddress, &aClientLen);
        printf("Id : %d / Add : %d / Name : %s \n", aPack.id, aPack.address, aPack.name);
        sendto(aServerFd, (void *)&aPack, sizeof(aPack), 0, (struct sockaddr*)&aClientAddress, sizeof(aClientAddress));
    }
    
}
