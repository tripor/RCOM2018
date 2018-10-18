#include "shared.h"
#include "stateMachine.h"

char state = 0;
int count=1;
int program_fd=0;
void touch()
{
	printf("alarme # %d\n", count);
  count++;
  if(count>=4)
  {
    printf("Couldn't make connection. Exiting...\n");
    llClose(program_fd);
    exit(1);
  }
  char sendMessage[255]="";
  sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cset,Aemi^Cset,FLAG);
  llWrite(sendMessage,program_fd);
  alarm(3);
}

void makeConnection(int fd,char type)
{
  program_fd=fd;
  signal(SIGALRM, touch);
  if(type=='W')
  {
    char sendMessage[255]="";
    sprintf(sendMessage,"%x %x %x %x %x",FLAG,Aemi,Cset,Aemi^Cset,FLAG);
    llWrite(sendMessage,fd);
    while(state!=5){
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

  }
  else if(type=='R')
  {
    char sendMessage[255]="";
    while(state!=5){
      char *receive=malloc(255);
      llRead(&receive,fd);
      unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
      sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
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

void stateMachineSET(unsigned int message)
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
        if(message == Cset)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 3:
        if(message == (Aemi^Cset))
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

        break;
        default: state = 0;

    }





}

void stateMachineUA(unsigned int message)
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
        if(message == Cua)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 3:
        if(message == (Arec^Cua))
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
        break;
        default: state = 0;

    }



}

void resetState()
{
    state = 0;
}
