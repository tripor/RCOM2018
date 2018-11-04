#include "disconnect.h"
#include "shared.h"
#include "stateMachine.h"
#include "connect.h"

int count_disconnect=0;
int disconnect_alarm=0;


void touchDisconnect()
{
    disconnect_alarm=1;
    printf("Timeout number %d in DISC message response. Resending message...\n", count_disconnect);
    count_disconnect++;
    if(count_disconnect>=4)
    {
        printf("Couldn't send the disconnect message. Exiting...\n");
        exit(1);
    }
}
void disconnectSender(int fd)
{
    int res;
    signal(SIGALRM, touchDisconnect);
    disconnect_alarm=0;
    count_disconnect=0;
    //Enviar a mensagem de disconnect para o Receiver
    sendMessage("DISC","W",fd);
    printf("Message DISC sent to Receiver.Waiting response...\n");
    //Receber a mensagem de disconnect do Receiver
    alarm(WAITTIME);
    while(getStateDisc()!=5){
        unsigned char receive;
        res=read(fd,&receive,1);
        if(disconnect_alarm)
        {
            disconnect_alarm=0;
            resetStates();
            sendMessage("DISC","W",fd);
            alarm(WAITTIME);
            continue;
        }
        if(res==0)continue;
        stateMachineDisc(receive);
    }
    alarm(0);
    disconnect_alarm=0;
    printf("Received the DISC message from Receiver. Sending UA message.\n");
    //Send UA message to Receiver
    sendMessage("UA","W",fd);
    printf("UA message sent\n");
}

void disconnectReceiver(int fd)
{
    unsigned char receive;
    int res;
    disconnect_alarm=0;
    count_disconnect=0;
    printf("Waiting for DISC message from Sender...\n");
    //Receber a mensagem de disconnect do Emissor
    while(getStateDisc()!=5){
        res=read(fd,&receive,1);
        if(res==0)continue;
        stateMachineDisc2(receive);
    }
    //Send message to Emissor
    printf("Sending DISC message to Sender...\n");
    sendMessage("DISC","R",fd);
    printf("DISC message sent to Sender. Waiting response...\n");
    resetStates();
    signal(SIGALRM, touchDisconnect);
    alarm(WAITTIME);
    while(getStateDisc()!=5){
        res=read(fd,&receive,1);
        if(disconnect_alarm)
        {
            disconnect_alarm=0;
            resetStates();
            sendMessage("DISC","R",fd);
            alarm(WAITTIME);
            continue;
        }
        if(res==0)continue;
        stateMachineUaDisc(receive);
    }
    alarm(0);
    disconnect_alarm=0;
    printf("UA message received form Sender.\n");


}


int llClose(int fd)
{
    if(getType())
        disconnectReceiver(fd);
    else
        disconnectSender(fd);
    sleep(2);
    closeL(fd);
    return close(fd);
}
