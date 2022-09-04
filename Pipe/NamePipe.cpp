#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MSG_SIZE 80
#define PIPENAME "/NamePipeFile"

int main() {
    char aMsg[MSG_SIZE];
    int aFd, aRead, aRc;

    //기존 파일 삭제
    if (access(PIPENAME, F_OK) == 0) {
        unlink(PIPENAME);
    }

    //pipe 생성
    if ((aRc = mkfifo(PIPENAME, 0666)) < 0) {
        printf("fail to make named pipe\n");
        return 0;
    }

    if ((aFd = open(PIPENAME, O_RDWR)) < 0) {
        printf("fail to open named pipe\n");
        return 0;
    }

    while (1) {
        if ((aRead = read(aFd, aMsg, sizeof(aMsg))) < 0) {
            printf("fail to call read()\n");
            return 0;
        }
        printf("Recv : %s\n", aMsg);
    }

}
