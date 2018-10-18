#include "data.h"

void changestate2Read(unsigned int message,unsigned int bcc);
void changestate2Write(unsigned int message);
void sendDataAux(unsigned int *data,int length,int fd);

int s=0;
int state2=0;

int count2=1;
int program_fd2=0;
unsigned int *data;
int length;
void touch2()
{
	printf("alarme # %d\n", count2);
  count2++;
  if(count2>=4)
  {
    printf("Couldn't make connection. Exiting...\n");
    llClose(program_fd2);
    exit(1);
  }

  sendDataAux(data,length,program_fd2);
  alarm(3);
}

void writeStuff(unsigned int data,int fd)
{
  char sendMessage[255]="";
  if(data==FLAG)
  {
    strcpy(sendMessage,"");
    sprintf(sendMessage,"%x",ESCAPE);
    llWrite(sendMessage,fd);
    strcpy(sendMessage,"");
    sprintf(sendMessage,"%x",ESCAPEF);
    llWrite(sendMessage,fd);
  }
  else if(data==ESCAPE)
  {
    strcpy(sendMessage,"");
    sprintf(sendMessage,"%x",ESCAPE);
    llWrite(sendMessage,fd);
    strcpy(sendMessage,"");
    sprintf(sendMessage,"%x",ESCAPEE);
    llWrite(sendMessage,fd);

  }
  else
  {
      strcpy(sendMessage,"");
      sprintf(sendMessage,"%x",data);
      llWrite(sendMessage,fd);
  }
}

unsigned int unStuff(unsigned int first,unsigned int second)
{
  if(first==ESCAPE && second == ESCAPEF)
  {
    return 0x7e;
  }
  else if(first==ESCAPE && second==ESCAPEE)
  {
    return ESCAPE;
  }
  else
    return 0x00;
}

void sendDataAux(unsigned int *data,int length,int fd)
{
  unsigned int bcc=0;
  char sendMessage[255]="";
  bcc^=Aemi;
  if(s==0)
    bcc^=S0;
  else
    bcc^=S1;
  for(int i=0;i<length;i++)
  {
    bcc^=data[i];
  }

  sprintf(sendMessage,"%x",FLAG);
  llWrite(sendMessage,fd);
  strcpy(sendMessage,"");
  sprintf(sendMessage,"%x",Aemi);
  llWrite(sendMessage,fd);
  strcpy(sendMessage,"");
  if(s==0){
    sprintf(sendMessage,"%x",S0);
  }
  else{
    sprintf(sendMessage,"%x",S1);
  }
  llWrite(sendMessage,fd);

  writeStuff(bcc,fd);

  for(int i=0;i<length;i++)
  {
    writeStuff(data[i],fd);
  }

  writeStuff(bcc,fd);
  strcpy(sendMessage,"");
  sprintf(sendMessage,"%x",FLAG);
  llWrite(sendMessage,fd);
}

void sendData(unsigned int *data2,int length2,int fd)
{
  signal(SIGALRM, touch2);
  sendDataAux(data2,length2,fd);
  program_fd2=fd;
  data=data2;
  length=length2;
  while(state2!=5){
    char *receive=malloc(255);
    alarm(3);
    llRead(&receive,fd);
    unsigned int flag=0,a=0,c=0,bcc=0,flag2=0;
    sscanf(receive,"%x %x %x %x %x",&flag,&a,&c,&bcc,&flag2);
    changestate2Write(flag);
    changestate2Write(a);
    changestate2Write(c);
    changestate2Write(bcc);
    changestate2Write(flag2);
  }
  if(s==0)s=1;
  else s=0;
}


void readData(int fd)
{
  unsigned int value,bcc=0,i=0,confirmar=0;
  char *receive=malloc(255);
  unsigned int guardar[1000];
  while(state2!=6){
    confirmar=0;
    bcc=0;
    i=0;
    strcpy(receive,"");
    llRead(&receive,fd);
    sscanf(receive,"%x",&value);
    changestate2Read(value,0);
    if(state2!=0)
    {
      value=0;
      while(value!=FLAG && i<=MaximumRead)
      {
        strcpy(receive,"");
        llRead(&receive,fd);
        sscanf(receive,"%x",&value);
        guardar[i]=value;
        i++;
      }
      if(i>MaximumRead) continue;
      i=i-3;
      bcc^=guardar[0];//A
      bcc^=guardar[1];//C
      int j=3,help=0;
      if(unStuff(guardar[2],guardar[3])!=0)
        j++;
      if(unStuff(guardar[i],guardar[i+1])!=0)
      {
          i--;
          help++;
        }
      for(;j<=i;j++)
      {
        unsigned int un=unStuff(guardar[j],guardar[j+1]);
        if(j!=i)
        {
          if(un==0)
            bcc^=guardar[j];
          else
          {
            bcc^=un;
            j++;
          }
        }
        else
        {
          bcc^=guardar[j];
        }
      }
      changestate2Read(guardar[0],bcc);
      changestate2Read(guardar[1],bcc);
      if(state2==4)confirmar++;
      if(unStuff(guardar[2],guardar[3])==0)
        changestate2Read(guardar[2],bcc);
      else
        changestate2Read(unStuff(guardar[2],guardar[3]),bcc);
      if(unStuff(guardar[i+help],guardar[i+1+help])==0)
        changestate2Read(guardar[i+1+help],bcc);
      else
        changestate2Read(unStuff(guardar[i+help],guardar[i+1+help]),bcc);
      changestate2Read(guardar[i+2+help],bcc);

    }
    if(confirmar!=0)
    {
      break;
    }
  }


  char sendMessage[255]="";
  if(confirmar==0){
    if(s==0)
      sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Crr0,Arec^Crr0,FLAG);
    else
      sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Crr1,Arec^Crr1,FLAG);
    llWrite(sendMessage,fd);
  }
  else
  {
    if(s==0)
      sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Crej0,Arec^Crej0,FLAG);
    else
      sprintf(sendMessage,"%x %x %x %x %x",FLAG,Arec,Crej1,Arec^Crej1,FLAG);
    llWrite(sendMessage,fd);
  }
}

void changestate2Read(unsigned int message,unsigned int bcc)
{
    switch(state2){
      case 0:
      if(message == FLAG)
          state2 = 1;
      break;

      case 1:
      if(message == Aemi)
          state2 = 2;
      else if(message != FLAG)
          state2 = 0;
      break;

      case 2:
      if(message == S0 && s==0)
      {
        s=1;
        state2 = 3;
      }
      else if(message == S1 && s==1)
      {
        s=0;
        state2 = 3;
      }
      else if (message == FLAG)
          state2 = 1;
      else
          state2 = 0;
      break;
      case 3:
      if(message == (bcc))
          state2 = 4;
      else if (message == FLAG)
          state2 = 1;
      else
          state2 = 0;
      break;
      case 4:
      if(message == (bcc))
          state2 = 5;
      else if (message == FLAG)
          state2 = 1;
      else
          state2 = 0;
      break;
      case 5:
      if(message == FLAG)
      state2 = 6;
      else state2 = 0;

      break;
      default: state2 = 0;
    }
}
void changestate2Write(unsigned int message)
{
  switch(state2){
    case 0:
    if(message == FLAG)
        state2 = 1;
    break;

    case 1:
    if(message == Arec)
        state2 = 2;
    else if(message != FLAG)
        state2 = 0;
    break;

    case 2:
    if(message == Crr0 && s==1)
    {
      state2 = 3;
    }
    else if(message == Crr1 && s==0)
    {
      state2 = 3;
    }
    else if (message == FLAG)
        state2 = 1;
    else
        state2 = 0;
    break;
    case 3:
    if((message == (Arec^Crr0) && s==1) || (message == (Arec^Crr1) && s==0) )
        state2 = 4;
    else if (message == FLAG)
        state2 = 1;
    else
        state2 = 0;
    break;
    case 4:
    if(message == FLAG)
    state2 = 5;
    else state2 = 0;

    break;
    default: state2 = 0;
  }
}
