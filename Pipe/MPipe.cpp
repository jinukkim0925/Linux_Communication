#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h>

struct TMsgBuf1
{
    long fMsgType;
    char fText[256];
    char fMyName[16];
    int seq;
};

struct TMsgBuf2
{
    long fMsgType;
    char fIp[128];
    char fHost[16];
    int seq;
};

int main() {
    
    key_t aKeyId;
    int i;
    struct TMsgBuf1 aBuf1;
    struct TMsgBuf2 aBuf2;

    //Message Queue 할당
    aKeyId = msgget((key_t)1234, IPC_CREAT | 0666);
    if (aKeyId == -1) {
        perror("MsgGet error");
        return 0;
    }
    printf("Key : %d\n", aKeyId);
    aBuf1.seq = 0;
    i = 0;
    while (1) {
        memset(&aBuf1, 0x00, sizeof(aBuf1));
        memset(&aBuf2, 0x00, sizeof(aBuf2));
        if (i % 2 == 0) {
            aBuf1.fMsgType = 4;
            snprintf(aBuf1.fText, sizeof(aBuf1.fText), "hello [%d]\n", i);
            snprintf(aBuf1.fMyName, sizeof(aBuf1.fMyName), "name [%d]\n", i);
            aBuf2.seq = i;
            aBuf1.seq = i;
            if (msgsnd( aKeyId, (void*)&aBuf1, sizeof(struct TMsgBuf1), IPC_NOWAIT) == -1) {
                perror("MsgSend error : ");
                return 0;
            }
        } else {
            aBuf2.fMsgType = 3;
            snprintf(aBuf2.fIp, sizeof(aBuf2.fIp), "10.0.8.%d", i);
            snprintf(aBuf2.fHost, sizeof(aBuf2.fHost), "ip[%d]", i);
            aBuf2.seq = i;
            if (msgsnd(aKeyId, (void *)&aBuf2, sizeof(struct TMsgBuf2), IPC_NOWAIT) == -1) {
                perror("MsgSend error : ");
                return 0;
            }
        }
        printf("send %d\n", i);
        i++;
        sleep(1);
    }
    return 0;
}
