#include<cstdio>
#include<iostream>
#include<string.h>
#include<fcntl.h>
#include<sys/unistd.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>

using namespace std;
#define FD_MAX_SIZE 1024
#define TIME_VALUE 10 * 1000

void ErrorMessage(const char *aMsg) {
    perror(aMsg);
    exit(0); 
}

int main(int argc, char **argv)
{
    int aEpollfd, aServerSockfd, aClientSockfd, aSockfd;
    int aState, aClient_len;
    int i, aEventCnt;

    struct epoll_event aEvent, *aEpollEvents;
    struct sockaddr_in aClientaddr, aServeraddr;
    fd_set aReadfsd, aAllfds;
    
    //int current_cli_num
    char buf[255];
    char line[255];

    memset(line, 0x00, sizeof(line));
    aState = 0;

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

    FD_ZERO(&aReadfsd);
    FD_SET(aServerSockfd, &aReadfsd);
    
    printf("\nTCP Server Waiting ... \n");
    fflush(stdout);

#if 0
    aSockfd = accept(aServerSockfd, (struct sockaddr *)&aClientaddr, (socklen_t *)&aClient_len);
    fds[0].fd = aSockfd;
    fds[0].events = POLLIN | POLLPRI;            
#endif

    aEpollfd = epoll_create(FD_MAX_SIZE);
    aEpollEvents = (epoll_event*) malloc(sizeof(struct epoll_event)* FD_MAX_SIZE);

    aEvent.events = EPOLLIN;
    aEvent.data.fd = aServerSockfd;
    epoll_ctl(aEpollfd, EPOLL_CTL_ADD, aServerSockfd, &aEvent);
    
    while(1) {
        aAllfds = aReadfsd;

        aEventCnt = epoll_wait(aEpollfd, aEpollEvents, FD_MAX_SIZE, TIME_VALUE);
            
        if (aEventCnt == 0) {
            printf("\n %d초동안 아무 수신 없음 !!!\n", TIME_VALUE / 1000);
            continue;
        }

        if(aEventCnt == -1) {
            printf("wait Error !!! \n");
            break;
        }

        for (i = 0; i < aEventCnt; i++) {

            if (aEpollEvents[i].data.fd == aServerSockfd) {
                aClient_len = sizeof(aClientaddr);
                aClientSockfd = accept(aServerSockfd, (struct sockaddr *)&aClientaddr, (socklen_t *)&aClient_len);
                
                aEvent.events = EPOLLIN;
                aEvent.data.fd = aClientSockfd;
                epoll_ctl(aEpollfd, EPOLL_CTL_ADD, aClientSockfd, &aEvent);

                printf("\nconnection from (%s , %d) \n",
                    inet_ntoa(aClientaddr.sin_addr),ntohs(aClientaddr.sin_port));                
                fflush(stdout);
                FD_SET(aClientSockfd, &aReadfsd);
            } 
            else {
                memset(buf, 0x00, sizeof(buf)); 
                if (read(aEpollEvents[i].data.fd, buf, sizeof(buf)) <= 0) {
                        close(aEpollEvents[i].data.fd);
                        perror("Close socket : ");
                        printf("Close client : %d \n", aEpollEvents[i].data.fd);
                        //FD_CLR(aSockfd, &aReadfsd);
                        fflush(stdout);
                } else {
                    printf("NUM [%d] [%d] RECV %s\n", i, aEpollEvents[i].data.fd, buf);
                    write(aEpollEvents[i].data.fd, buf, 255);
                    fflush(stdout);
                }
            }
        }
    }

    close(aServerSockfd);
    close(aEpollfd);

    return 0;
}
