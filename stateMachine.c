#include "shared.h"


char state = 0;


void makeConnection(int fd,char type)
{
  if(type=='W')
  {
    char sendMessage[255]="";
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cset,Aemi^Cset,FLAG);
    llWrite(sendMessage,fd);
  }
  else if(type=='R')
  {
    char *receive=malloc(255);
    llRead(&receive,fd);
    printf("%s",receive);
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
