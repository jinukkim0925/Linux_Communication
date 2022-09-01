#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FD_MAX_SIZE 1024
#define TIME_VALUE 10

void ErrorMessage(const char *aMsg) {
    perror(aMsg);
    exit(0); 
}

int main(int argc, char **argv)
{
    int aServerSockfd, aClientSockfd, aSockfd;
    int aState, aClient_len;
    int i, aMax_client, aMaxfd;
    int aClient[FD_MAX_SIZE];

    struct timeval aTimeval;
    struct sockaddr_in aClientaddr, aServeraddr;
    fd_set aReadfsd, aOtherfds, aAllfds;
    
    //int current_cli_num
    char buf[255];
    char line[255];

    memset(line, 0x00, sizeof(line));
    aState = 0;

    aTimeval.tv_sec = TIME_VALUE;
    aTimeval.tv_usec = 0;

    if ((aServerSockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        ErrorMessage("socket error : ");
        
    memset(&aServeraddr, 0x00, sizeof(aServeraddr));
    aServeraddr.sin_family = AF_INET;
    aServeraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    aServeraddr.sin_port = htons(9000);

    aState = bind(aServerSockfd, (struct sockaddr *)&aServeraddr, sizeof(aServeraddr));
    
    if (aState == -1)
        ErrorMessage("bind error : ");

    aState = listen(aServerSockfd, 5);
    if (aState == -1)
        ErrorMessage("listen error : ");

    aClientSockfd = aServerSockfd;

    aMax_client = -1;
    aMaxfd = aServerSockfd;

    for (i = 0; i < FD_MAX_SIZE; i++) {
        aClient[i] = -1;
    }

    FD_ZERO(&aReadfsd);
    FD_SET(aServerSockfd, &aReadfsd);
    
    printf("\nTCP Server Waiting ...");
    fflush(stdout);

    while(1) {
        aAllfds = aReadfsd;
    
        aTimeval.tv_sec = TIME_VALUE;
        aTimeval.tv_usec = 0;
        
        aState = select(aMaxfd + 1, &aAllfds, NULL, NULL, &aTimeval);

        if (aState == 0) {
            printf("\n %d초동안 아무 수신 없음 !!!\n", TIME_VALUE);
        }
        
        if (FD_ISSET(aServerSockfd, &aAllfds)) {
            aClient_len = sizeof(aClientaddr);
            aClientSockfd = accept(aServerSockfd, (struct sockaddr *)&aClientaddr, (socklen_t *)&aClient_len);
            printf("\nconnection from (%s , %d)",
                    inet_ntoa(aClientaddr.sin_addr),ntohs(aClientaddr.sin_port));
            
            for (i = 0; i < FD_MAX_SIZE; i++) {
                if (aClient[i] < 0 ) {
                    aClient[i] = aClientSockfd;
                    printf("\nclientNUM=%d\nclientFD=%d\n", i + 1, aClientSockfd);
                    break;
                }
            }
            
            printf("accept [%d]\n", aClientSockfd);
            printf("===================================\n");
            if (i == FD_MAX_SIZE) 
                perror("too many clients\n");
            
            FD_SET(aClientSockfd, &aReadfsd);
            if (aClientSockfd > aMaxfd)
                aMaxfd = aClientSockfd;

            if (i > aMax_client)
                aMax_client = i;
            
            if (--aState <= 0)
                continue;

        }
        
        for (i = 0; i <= aMax_client; i++) {
            if ((aSockfd = aClient[i]) < 0)
                continue;
            
            if (FD_ISSET(aSockfd, &aAllfds)) {
                memset(buf, 0x00, sizeof(buf));

                if (read(aSockfd, buf, sizeof(buf)) <= 0){
                    close(aSockfd);
                    perror("Close sockfd : ");
                    FD_CLR(aSockfd, &aReadfsd);
                    aClient[i] = -1;
                }
                printf("[%d]RECV %s\n", aSockfd, buf);
                write(aSockfd, buf, 255);

                if (--aState <= 0)
                    break;
            }
        }
    }

    return 0;
}
