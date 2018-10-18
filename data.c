#include "data.h"

void changestate2Read(unsigned int message,unsigned int bcc);

int s=0;
int state2=0;

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

void sendData(unsigned int *data,int length,int fd)
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
  sprintf(sendMessage,"%x",bcc);
  llWrite(sendMessage,fd);

  for(int i=0;i<length;i++)
  {
    writeStuff(data[i],fd);
  }

  strcpy(sendMessage,"");
  sprintf(sendMessage,"%x",bcc);
  llWrite(sendMessage,fd);
  strcpy(sendMessage,"");
  sprintf(sendMessage,"%x",FLAG);
  llWrite(sendMessage,fd);
}


void readData(int fd)
{
  unsigned int value,bcc=0,i=0;
  char *receive=malloc(255);
  unsigned int guardar[1000];
  while(state2!=6){
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
      for(int j=2;j<=i;j++)
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
      }
      changestate2Read(guardar[0],bcc);
      changestate2Read(guardar[1],bcc);
      changestate2Read(guardar[2],bcc);
      changestate2Read(guardar[i-2],bcc);
      changestate2Read(guardar[i-1],bcc);

    }
    printf("estado:%d\n",state2);
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
