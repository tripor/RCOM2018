#include "disconnect.h"
#include "shared.h"
#include "stateMachine.h"

int disconnect_fd;
int count_disconnect=0;
int message_sent_sender=0;
int message_sent_receiver=0;

void stateMachineReceive1(unsigned int message);
void stateMachineSender(unsigned int message);
void stateMachineReceive2(unsigned int message);

/**
 * Signal alarm handler no caso de timeout do Sender
 */
void touchDisconnectSender()
{
    //Caso a messagem ainda não tenha sido recebida
    if(message_sent_sender==0)
    {
        printf("Timeout number %d in DISC message response. Resending message...\n", count_disconnect);
        count_disconnect++;
        if(count_disconnect>=4)
        {
            printf("Couldn't send the disconnect message. Exiting...\n");
            llClose(disconnect_fd);
            exit(1);
        }
        char sendMessage[255]="";
        //Mandar outra vez a mensagem para o receiver
        sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cdisc,Aemi^Cdisc,FLAG);
        llWrite(sendMessage,disconnect_fd);
        printf("DISC message resend, waiting for response...\n");
        alarm(3);
    }
}
/**
 * Signal alarm handler no caso de timeout do Sender
 */
void touchDisconnectReceiver()
{
    //Caso a messagem ainda não tenha sido recebida
    if(message_sent_receiver==0)
    {
        printf("Timeout number %d in DISC message response. Resending message...\n", count_disconnect);
        count_disconnect++;
        if(count_disconnect>=4)
        {
            printf("Couldn't send the disconnect message. Exiting...\n");
            llClose(disconnect_fd);
            exit(1);
        }
        char sendMessage[255]="";
        //Mandar outra vez a mensagem para o receiver
        sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Cdisc,Arec^Cdisc,FLAG);
        llWrite(sendMessage,disconnect_fd);
        printf("DISC message resend, waiting for response...\n");
        alarm(3);
    }
}
void disconnectSender(int fd)
{
    disconnect_fd=fd;
    count_disconnect=0;
    message_sent_sender=0;
    char sendMessage[255]="";
    signal(SIGALRM, touchDisconnectSender);
    //Enviar a mensagem de disconnect para o Receiver
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cdisc,Aemi^Cdisc,FLAG);
    llWrite(sendMessage,fd);
    printf("Mensage DISC sent to Receiver.Waiting response...\n");
    //Receber a mensagem de disconnect do Receiver
    while(getStateDisc()!=5){
        char *receive=malloc(255);
        alarm(3);
        llRead(&receive,fd);
        unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
        sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
        stateMachineDisc(flag);
        stateMachineDisc(a);
        stateMachineDisc(c);
        stateMachineDisc(bcc);
        stateMachineDisc(flag2);
        if(getStateDisc()!=5) printf("Got the wrong message. Retrying...\n");
    }
    printf("Received the DISC message from Receiver. Sending UA message.\n");
    message_sent_sender=1;

    //Send UA message to Receiver
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cua,Aemi^Cua,FLAG);
    llWrite(sendMessage,fd);
    printf("UA message sent\n");
}

void disconnectReceiver(int fd)
{
    disconnect_fd=fd;
    count_disconnect=0;
    message_sent_receiver=0;
    char sendMessage[255]="";
    signal(SIGALRM, touchDisconnectReceiver);

    //Receber a mensagem de disconnect do Emissor
    while(getStateDisc()!=5){
        char *receive=malloc(255);
        llRead(&receive,fd);
        unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
        sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
        stateMachineDisc2(flag);
        stateMachineDisc2(a);
        stateMachineDisc2(c);
        stateMachineDisc2(bcc);
        stateMachineDisc2(flag2);
        if(getStateDisc()!=5) printf("Got the wrong message. Retrying...\n");
    }
    //Send message to Emissor
    printf("Sending DISC message to Sender...\n");
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Cdisc,Arec^Cdisc,FLAG);
    llWrite(sendMessage,fd);
    printf("DISC message sent to Sender. Waiting response...\n");
    resetStates();
    while(getStateDisc()!=5){
        char *receive=malloc(255);
        alarm(3);
        llRead(&receive,fd);
        unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
        sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
        stateMachineUaDisc(flag);
        stateMachineUaDisc(a);
        stateMachineUaDisc(c);
        stateMachineUaDisc(bcc);
        stateMachineUaDisc(flag2);
        if(getStateDisc()!=5) printf("Got the wrong message. Retrying...\n");
    }
    printf("UA message received form Sender.\n");


}
