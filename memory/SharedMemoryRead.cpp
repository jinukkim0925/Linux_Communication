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

    int aShmId, i;
    TShmInfo *aShmInfo;
    void *aSharedMemory = NULL;

    aShmId = shmget((key_t)3836, sizeof(TShmInfo)* SHM_INFO_COUNT, 0666 | IPC_CREAT);

    if (aShmId == -1) {
        perror("shmget falled : ");
        exit(0);
    }

    aSharedMemory = shmat(aShmId, NULL, 0);
    if (aSharedMemory == (void *) -1) {
        perror("shmat failed : ");
        exit(0);
    }

    aShmInfo = (TShmInfo*)aSharedMemory;
    while(1) {
        for (i = 0; i < SHM_INFO_COUNT; i++) {
            fprintf(stderr, "-------- [%d] shared info ------- \n", i);
            fprintf(stderr, "String IP[%s] \n", aShmInfo[i].str_ip);
            fprintf(stderr, "Int IP[%u] \n", aShmInfo[i].int_ip);
            fprintf(stderr, "Int ID[%u] \n", aShmInfo[i].int_id);
        }
        sleep(1);
    }
}

