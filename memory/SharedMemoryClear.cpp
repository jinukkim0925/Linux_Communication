#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_INFO_COUNT 10

typedef struct _shm_info{
    char str_ip[40];
    unsigned int int_ip;
    unsigned int int_id;
} TShmInfo;

int main() {

    int aShmId;

    aShmId = shmget((key_t)3836, sizeof(TShmInfo)* SHM_INFO_COUNT, 0666 | IPC_CREAT);

    if (aShmId == -1) {
        perror("shmget falled : ");
        exit(0);
    }

    if (-1 == shmctl(aShmId, IPC_RMID, 0)) {
        printf("공유 메모리 제거 실패 \n");
        return -1;
    }else {
        printf("공유 메모리 제거 성공 !! \n");
    }

}

