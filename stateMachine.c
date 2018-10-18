#include "shared.h"
#include "stateMachine.h"

char state = 0;


void makeConnection(int fd,char type)
{
  if(type=='W')
  {
    char sendMessage[255]="";
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cset,Aemi^Cset,FLAG);
    llWrite(sendMessage,fd);
    while (state!=5) {
      char *receive=malloc(255);
      llRead(&receive,fd);
      int flag=0,a=0,c=0,bcc=0,flag2=0;
      sprintf(sendMessage,"%x %x %x %x %x",flag,a,c,bcc,flag2);
      stateMachineUA(flag);
      stateMachineUA(a);
      stateMachineUA(c);
      stateMachineUA(bcc);
      stateMachineUA(flag2);
    }
  }
  else if(type=='R')
  {
    char sendMessage[255]="";
    while (state!=5) {
      char *receive=malloc(255);
      llRead(&receive,fd);
      int flag=0,a=0,c=0,bcc=0,flag2=0;
      sprintf(sendMessage,"%x %x %x %x %x",flag,a,c,bcc,flag2);
      stateMachineSET(flag);
      stateMachineSET(a);
      stateMachineSET(c);
      stateMachineSET(bcc);
      stateMachineSET(flag2);
    }
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Cua,Arec^Cua,FLAG);
    llWrite(sendMessage,fd);
  }
}

void stateMachineSET(char message)
{

    switch(state){
        case 0:
        if(message == FLAG)
            state = 1;
        break;

        case 1:
        if(message == Arec)
            state = 2;
        else if(message != FLAG)
            state = 0;
        break;

        case 2:
        if(message == Cset)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 3:
        if(message == (Arec^Cset))
            state = 4;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 4:
        if(message == FLAG)
        state = 5;
        else state = 0;

        default: state = 0;

    }





}

void stateMachineUA(char message)
{
   switch(state){
        case 0:
        if(message == FLAG)
            state = 1;
        break;

        case 1:
        if(message == Aemi)
            state = 2;
        else if(message != FLAG)
            state = 0;
        break;

        case 2:
        if(message == Cua)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 3:
        if(message == (Aemi^Cua))
            state = 4;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 4:
        if(message == FLAG)
        state = 5;
        else state = 0;

        default: state = 0;

    }



}

void resetState()
{
    state = 0;
}
