#include "shared.h"
#include "connect.h"

struct termios *oldtio,*newtio;
int myType;

void setRead()
{
  myType=1;
}

void setWrite()
{
  myType=0;
}

int getType()
{
  return myType;
}

/**
 * Escreve um byte
 */
int writeByte(int fd,unsigned char byte)
{
  int res=write(fd,&byte,1);
  return res;
}
void sendMessage(char *type,char * typeSender,int fd)
{
  unsigned char send,a,c;
  if(strcmp("W",typeSender)==0) a=Aemi;
  else a=Arec;
  if(strcmp("SET",type)==0) c=Cset;
  else if(strcmp("UA",type)==0) c=Cua;
  else if(strcmp("DISC",type)==0) c=Cdisc;
  else if(strcmp("RR1",type)==0) c=Crr1;
  else if(strcmp("RR0",type)==0) c=Crr0;
  else if(strcmp("REJ1",type)==0) c=Crej1;
  else if(strcmp("REJ0",type)==0) c=Crej0;

  send=FLAG;
  writeByte(fd,send);

  send=a;
  writeByte(fd,send);

  send=c;
  writeByte(fd,send);

  send=a^c;
  writeByte(fd,send);

  send=FLAG;
  writeByte(fd,send);
}
