#include "disconnect.h"
#include "shared.h"
#include "stateMachine.h"

int disconnect_fd;
int count_disconnect=0;
int message_sent_sender=0;
int message_sent_receiver=0;


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
        //Mandar outra vez a mensagem para o receiver
        sendMessage("DISC","W",disconnect_fd);
        printf("DISC message resend, waiting for response...\n");
        turnAlarm(1);
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
        //Mandar outra vez a mensagem para o receiver
        sendMessage("DISC","R",disconnect_fd);
        printf("DISC message resend, waiting for response...\n");
        turnAlarm(1);
    }
}
void disconnectSender(int fd)
{
    disconnect_fd=fd;
    count_disconnect=0;
    message_sent_sender=0;
    //Enviar a mensagem de disconnect para o Receiver
    sendMessage("DISC","W",fd);
    printf("Mensage DISC sent to Receiver.Waiting response...\n");
    //Receber a mensagem de disconnect do Receiver
    while(getStateDisc()!=5){
      signal(SIGALRM, touchDisconnectSender);
        unsigned char *receive=malloc(2);
        alarm(3);
        llRead(fd,receive);
        if(getAlarm()==1)
        {
          turnAlarm(0);
          continue;
        }
        stateMachineDisc(receive[0]);
    }
    printf("Received the DISC message from Receiver. Sending UA message.\n");
    message_sent_sender=1;
    //Send UA message to Receiver
    sendMessage("UA","W",fd);
    printf("UA message sent\n");
}

void disconnectReceiver(int fd)
{
    disconnect_fd=fd;
    count_disconnect=0;
    message_sent_receiver=0;
    printf("Waiting for DISC message from Sender...\n");
    //Receber a mensagem de disconnect do Emissor
    while(getStateDisc()!=5){
        unsigned char *receive=malloc(2);
        llRead(fd,receive);
        stateMachineDisc2(receive[0]);
    }
    //Send message to Emissor
    printf("Sending DISC message to Sender...\n");
    sendMessage("DISC","R",fd);
    printf("DISC message sent to Sender. Waiting response...\n");
    resetStates();
    while(getStateDisc()!=5){
        signal(SIGALRM, touchDisconnectReceiver);
        unsigned char *receive=malloc(2);
        alarm(3);
        llRead(fd,receive);
        if(getAlarm()==1)
        {
          turnAlarm(0);
          continue;
        }
        stateMachineUaDisc(receive[0]);
    }
    printf("UA message received form Sender.\n");


}
