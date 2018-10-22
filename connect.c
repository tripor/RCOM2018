#include "connect.h"
#include "stateMachine.h"

int connect_count=1;
int connect_fd=0;
int message_sent=0;
void touchConnectSender()
{
    //Caso a mensagem tenha sido enviada com sucesso
    if(message_sent==0)
    {
        printf("Timeout number %d in SET message response. Resending message...\n", connect_count);
        connect_count++;
        if(connect_count>=4)
        {
            printf("Couldn't make connection. Exiting...\n");
            llClose(connect_fd);
            exit(1);
        }
        //Resending message
        printf("Resending SET message to Receiver...\n");
        sendMessage("SET","W",connect_fd);
        printf("SET message resent...\n");
        turnAlarm(1);
    }
}

void makeConnectionReceiver(int fd)
{
    connect_fd=fd;
    connect_count=0;
    //esperar pela mensagem de set do emissor
    printf("Waiting for the SET message from Sender...\n");
    while(getStateSet()!=5){
      char *receive=malloc(2);
      llRead(fd,receive);
      stateMachineSET(receive[0]);
    }
    //Mandar a mensagem de UA
    printf("SET message received. Sending UA message to Receiver...\n");
    sendMessage("UA","R",fd);
    printf("UA message sent to Receiver.\n");
}

void makeConnectionSender(int fd)
{
    connect_fd=fd;
    connect_count=0;
    //Mandar mensagem de SET
    printf("Sending SET message to Receiver...\n");
    sendMessage("SET","W",fd);
    printf("SET message sent to Receiver. Waiting for response...\n");
    //Esperar pela resposta do recetor
    while(getStateUa()!=5){
        signal(SIGALRM, touchConnectSender);
        char *receive=malloc(2);
        alarm(3);
        llRead(fd,receive);
        if(getAlarm()==1)
        {
          turnAlarm(0);
          continue;
        }
        stateMachineUA(receive[0]);
    }
    message_sent=1;
    printf("UA message received from Receiver.\n");

}
