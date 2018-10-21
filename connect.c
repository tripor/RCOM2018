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
        char sendMessage[255]="";
        //Resending message
        printf("Resending SET message to Receiver...\n");
        sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cset,Aemi^Cset,FLAG);
        llWrite(sendMessage,connect_fd);
        printf("SET message resent...\n");
        alarm(3);
    }
}

void makeConnectionReceiver(int fd)
{
    connect_fd=fd;
    connect_count=0;
    char sendMessage[255]="";
    //esperar pela mensagem de set do emissor
    printf("Waiting for the SET message from Sender...\n");
    while(getStateSet()!=5){
      char *receive=malloc(255);
      llRead(&receive,fd);
      unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
      sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
      stateMachineSET(flag);
      stateMachineSET(a);
      stateMachineSET(c);
      stateMachineSET(bcc);
      stateMachineSET(flag2);
      if(getStateSet()!=5)printf("Wrong message received. Waiting for a new one...\n");
    }
    //Mandar a mensagem de UA
    printf("SET message received. Sending UA message to Receiver...\n");
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Cua,Arec^Cua,FLAG);
    llWrite(sendMessage,fd);
    printf("UA message sent to Receiver.\n");
}

void makeConnectionSender(int fd)
{
    connect=fd;
    connect_count=0;
    signal(SIGALRM, touchConnectSender);
    char sendMessage[255]="";
    //Mandar mensagem de SET
    printf("Sending SET message to Receiver...\n");
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cset,Aemi^Cset,FLAG);
    llWrite(sendMessage,fd);
    printf("SET message sent to Receiver. Waiting for response...\n");
    //Esperar pela resposta do recetor
    while(getStateUa()!=5){
        char *receive=malloc(255);
        alarm(3);
        llRead(&receive,fd);
        unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
        sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
        stateMachineUA(flag);
        stateMachineUA(a);
        stateMachineUA(c);
        stateMachineUA(bcc);
        stateMachineUA(flag2);
    }
    message_sent=1;
    printf("UA message received from Receiver.\n");

}