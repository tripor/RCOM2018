#include "disconnect.h"
#include "shared.h"

int disconnect_fd;
int count_disconnect=0;
int state_disconnect=0;
int message_sent_sender=0;
int message_sent_receiver=0;

void stateMachineReceive1(unsigned int message);
void stateMachineSender(unsigned int message);

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
void touchDisconnectSender()
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
        sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cdisc,Aemi^Cdisc,FLAG);
        llWrite(sendMessage,disconnect_fd);
        printf("DISC message resend, waiting for response...\n");
        alarm(3);
    }
}
void disconnectSender(int fd)
{
    disconnect_fd=fd;
    count_disconnect=0;
    state_disconnect=0;
    message_sent_sender=0;
    char sendMessage[255]="";
    signal(SIGALRM, touchDisconnectSender);
    //Enviar a mensagem de disconnect para o Receiver
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cdisc,Aemi^Cdisc,FLAG);
    llWrite(sendMessage,fd);
    printf("Mensage DISC sent to Receiver.Waiting response...\n");
    //Receber a mensagem de disconnect do Receiver
    while(state_disconnect!=5){
        char *receive=malloc(255);
        alarm(3);
        llRead(&receive,fd);
        unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
        sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
        stateMachineSender(flag);
        stateMachineSender(a);
        stateMachineSender(c);
        stateMachineSender(bcc);
        stateMachineSender(flag2);
        if(state_disconnect!=5) printf("Got the wrong message. Retrying...\n");
    }
    printf("Received the DISC message from Receiver. Sending UA message.\n");
    message_sent_sender=1;

    //Send UA message to Receiver
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Cdisc,Arec^Cdisc,FLAG);
    llWrite(sendMessage,fd);
    printf("UA message sent\n");
}

void disconnectReceiver(int fd)
{
    disconnect_fd=fd;
    count_disconnect=0;
    state_disconnect=0;
    message_sent_receiver=0;
    char sendMessage[255]="";
    signal(SIGALRM, touchDisconnectReceiver);

    //Receber a mensagem de disconnect do Emissor
    while(state_disconnect!=5){
      char *receive=malloc(255);
      llRead(&receive,fd);
      unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
      sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
      stateMachineReceive1(flag);
      stateMachineReceive1(a);
      stateMachineReceive1(c);
      stateMachineReceive1(bcc);
      stateMachineReceive1(flag2);
    }
    //Send message to Emissor
    printf("Sending DISC message to Sender...\n");
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Cdisc,Arec^Cdisc,FLAG);
    llWrite(sendMessage,fd);
    printf("DISC message sent to Sender. Waiting response...\n");
    state_disconnect=0;
    while(state_disconnect!=5){
        char *receive=malloc(255);
        alarm(3);
        llRead(&receive,fd);
        unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
        sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
        stateMachineSender(flag);
        stateMachineSender(a);
        stateMachineSender(c);
        stateMachineSender(bcc);
        stateMachineSender(flag2);
        if(state_disconnect!=5) printf("Got the wrong message. Retrying...\n");
    }
    printf("UA message received form Sender.\n");


}


void stateMachineReceive1(unsigned int message)
{
    switch(state_disconnect){
        case 0:
            if(message == FLAG)
                state_disconnect = 1;
            break;
        case 1:
            if(message == Aemi)
                state_disconnect = 2;
            else if(message != FLAG)
                state_disconnect = 0;
            break;
        case 2:
            if(message == Cdisc)
                state_disconnect = 3;
            else if (message == FLAG)
                state_disconnect = 1;
            else
                state_disconnect = 0;
            break;
        case 3:
            if(message == (Aemi^Cdisc))
                state_disconnect = 4;
            else if (message == FLAG)
                state_disconnect = 1;
            else
                state_disconnect = 0;
            break;
        case 4:
            if(message == FLAG)
                state_disconnect = 5;
            else state_disconnect = 0;

            break;
        default: state_disconnect = 0;

    }
}

void stateMachineReceive2(unsigned int message)
{
    switch(state_disconnect){
        case 0:
            if(message == FLAG)
                state_disconnect = 1;
            break;
        case 1:
            if(message == Aemi)
                state_disconnect = 2;
            else if(message != FLAG)
                state_disconnect = 0;
            break;
        case 2:
            if(message == Cua)
                state_disconnect = 3;
            else if (message == FLAG)
                state_disconnect = 1;
            else
                state_disconnect = 0;
            break;
        case 3:
            if(message == (Aemi^Cua))
                state_disconnect = 4;
            else if (message == FLAG)
                state_disconnect = 1;
            else
                state_disconnect = 0;
            break;
        case 4:
            if(message == FLAG)
                state_disconnect = 5;
            else state_disconnect = 0;

            break;
        default: state_disconnect = 0;

    }
}

void stateMachineSender(unsigned int message)
{
    switch(state_disconnect){
        case 0:
            if(message == FLAG)
                state_disconnect = 1;
            break;
        case 1:
            if(message == Arec)
                state_disconnect = 2;
            else if(message != FLAG)
                state_disconnect = 0;
            break;
        case 2:
            if(message == Cdisc)
                state_disconnect = 3;
            else if (message == FLAG)
                state_disconnect = 1;
            else
                state_disconnect = 0;
            break;
        case 3:
            if(message == (Arec^Cdisc))
                state_disconnect = 4;
            else if (message == FLAG)
                state_disconnect = 1;
            else
                state_disconnect = 0;
            break;
        case 4:
            if(message == FLAG)
                state_disconnect = 5;
            else state_disconnect = 0;

            break;
        default: state_disconnect = 0;

    }
}