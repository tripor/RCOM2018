#include "data.h"


int s=0;

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
  sprintf(sendMessage,"%x",FLAG);
  llWrite(sendMessage,fd);
}


void readData(int fd)
{
  char *receive=malloc(255);
  llRead(&receive,fd);
}
