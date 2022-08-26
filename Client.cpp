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
#include <netdb.h>

#define FD_MAX_SIZE 1024

void ErrorMessage(const char *aMsg) {
    perror(aMsg);
    exit(0); 
}

int main(int argc, char **argv)
{
    int aSock, aByteRecived;
    char send_data[1024], recv_data[1024];
    struct hostent *aHost;
    struct sockaddr_in aServeraddr;
    
    //aHost = gethostname(*argv, sizeof(*argv));
    aHost = gethostbyname("127.0.0.1");

    if ((aSock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        ErrorMessage("socket error : ");

    aServeraddr.sin_family = AF_INET;
    aServeraddr.sin_port = htons(9000);
    aServeraddr.sin_addr = *((struct in_addr *)aHost->h_addr);
    memset(&(aServeraddr.sin_zero), 0x00, 8);

    if (connect(aSock, (struct sockaddr *)&aServeraddr, sizeof(struct sockaddr)) == -1)
        ErrorMessage("Connect error : ");

    while(1) {
        memset(send_data, 0x00, sizeof(send_data));
        memset(recv_data, 0x00, sizeof(recv_data));

        printf("\nSEND : ");
        fgets(send_data, sizeof(send_data), stdin);
      
        if (strcmp(send_data, "q\n") != 0 && strcmp(send_data, "Q\n") != 0)
            send(aSock, send_data, strlen(send_data), 0);
        else {
            send(aSock, send_data, strlen(send_data), 0);
            printf("Disconnected ... \n");
            close(aSock);
            break;
        }
        aByteRecived = recv(aSock, recv_data, 1024, 0);
        recv_data[aByteRecived] ='\0';

        if (strcmp(recv_data, "q\n") == 0 || strcmp(recv_data, "Q\n") == 0) {
            close(aSock);
            printf("Disconnected ... \n");
            break;
        } else {
            printf("\nRECV = %s\n " , recv_data);
        }
        
    }

    return 0;    
}
