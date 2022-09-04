#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h>

struct TMsgBuf1
{
    long fMsgType;
    char fText[256];
    char fMyName[16];
    int fSeq;
};

struct TMsgBuf2
{
    long fMsgType;
    char fIp[128];
    char fHost[16];
    int fSeq;
};

int main(int argc, char **argv) {
    
    key_t aKeyId;
    struct TMsgBuf1 aBuf1;
    struct TMsgBuf2 aBuf2;
    int aMsgType;

    if (argc == 2)
        aMsgType = 3;
    else
        aMsgType = 4;


    //Message Queue 할당
    aKeyId = msgget((key_t)1234, IPC_CREAT | 0666);
    if (aKeyId == -1) {
        perror("MsgGet error");
        return 0;
    }

    while (1) {
        if (aMsgType == 3) {
            if (msgrcv(aKeyId, (void*)&aBuf2, sizeof(struct TMsgBuf2), aMsgType, 0) == -1) {
                perror("msgrcv error : ");
                return 0;
            }
            printf("%s %d\n", aBuf2.fHost, aBuf2.fSeq);
        } else {
            if (msgrcv(aKeyId, (void*)&aBuf1, sizeof(struct TMsgBuf1), aMsgType, 0) == -1) {
                perror("msgrcv error : ");
                return 0;
            }
            printf("%s %d\n", aBuf1.fText, aBuf1.fSeq);
        }
    }   
    return 0;
}
