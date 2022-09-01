#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_SIZE 80
#define PIPENAME "/NamePipeFile"

int main() {
    
    char aMsg[MSG_SIZE];
    int aFd;
    int aRead, i;

    //pipe열기 write 전용
    if ((aFd = open(PIPENAME, O_WRONLY)) < 0) {
        printf("fail to open named pipe\n");
        return 0;
    }
    
    // send Data
    snprintf(aMsg, sizeof(aMsg), "Send Message : Hellow!");
    if ((aRead = write(aFd, aMsg, sizeof(aMsg))) < 0) {
        printf("fail to call write()\n");
        return 0;
    }
    
}
