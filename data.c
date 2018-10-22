#include "data.h"

void changestate2Read(char message,char bcc);
void changestate2Write(char message);

int s=0;
int state2=0;

int count2=1;
int program_fd2=0;
char *data;
int length;
int data_message_sent=0;
void touch2()
{
	if(data_message_sent==0)
	{
		printf("Timeout number %d in Data message response. Resending data...\n", count2);
	  count2++;
	  if(count2>=4)
	  {
	    printf("Couldn't send data. Exiting...\n");
	    llClose(program_fd2);
	    exit(1);
	  }
		printf("Resending Data to Receiver...\n");
	  sendDataAux(data,length,program_fd2);
		printf("Data sent to Receiver...\n");
		turnAlarm(1);
	}
}

void writeStuff(char data,int fd)
{
  char send[2];
	send[1]='\0';
  if(data==FLAG)
  {
		send[0]=ESCAPE;
	  llWrite(fd,send,2);
		send[0]=ESCAPEF;
	  llWrite(fd,send,2);
  }
  else if(data==ESCAPE)
  {
		send[0]=ESCAPE;
	  llWrite(fd,send,2);
		send[0]=ESCAPEE;
	  llWrite(fd,send,2);

  }
  else
  {
		send[0]=data;
	  llWrite(fd,send,2);
  }
}

char unStuff(char first,char second)
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

void sendDataAux(char *data,int length,int fd)
{
  char bcc=0;
  char send[2];
	send[1]='\0';
  bcc^=Aemi;
  if(s==0)
    bcc^=S0;
  else
    bcc^=S1;
  for(int i=0;i<length;i++)
  {
    bcc^=data[i];
  }

	send[0]=FLAG;
  llWrite(fd,send,2);
	send[0]=Aemi;
  llWrite(fd,send,2);
  if(s==0){
		send[0]=S0;
  }
  else{
		send[0]=S1;
  }
  llWrite(fd,send,2);

  writeStuff(bcc,fd);

  for(int i=0;i<length;i++)
  {
    writeStuff(data[i],fd);
  }

  writeStuff(bcc,fd);
	send[0]=FLAG;
  llWrite(fd,send,2);
}

void sendData(char *data2,int length2,int fd)
{
	printf("Alarm signal subscribed.\n");
  sendDataAux(data2,length2,fd);
  program_fd2=fd;
  data=data2;
  length=length2;
	printf("Data sent. Waiting for the response...\n");
  while(state2!=5){
  	signal(SIGALRM, touch2);
    char *receive=malloc(2);
    alarm(3);
    llRead(fd,receive);
		if(getAlarm()==1)
		{
			turnAlarm(0);
			continue;
		}
    changestate2Write(receive[0]);
  }
  if(s==0)s=1;
  else s=0;
}


void readData(int fd,char *guardar2)
{
  unsigned int bcc=0,i=0,confirmar=0;
  char *receive=malloc(2);
  char guardar[1000];
	printf("Reading data...\n");
  while(state2!=6){
    confirmar=0;
    bcc=0;
    i=0;
    int k=0;
    strcpy(receive,"");
    llRead(fd,receive);
    changestate2Read(receive[0],0);
    if(state2!=0)
    {
      receive[0]=0;
      while(receive[0]!=FLAG && i<=MaximumRead)
      {
        llRead(fd,receive);
        guardar[i]=receive[0];
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
			for(;j<=i;j++,k++)
      {
        unsigned int un=unStuff(guardar[j],guardar[j+1]);
        if(j!=i)
        {
          if(un==0)
          {
            bcc^=guardar[j];
            guardar2[k]=guardar[j];
          }
          else
          {
            bcc^=un;
            guardar2[k]=un;
            j++;
          }
        }
        else
        {
          bcc^=guardar[j];
          guardar2[k]=guardar[j];
        }
				printf("bcc   9999 : %x\n",bcc);
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
    if(confirmar!=0 && state2==0)
    {
			printf("Sending REJ message to Sender...\n");
      if(s==0)
				sendMessage("REJ0","R",fd);
      else
				sendMessage("REJ1","R",fd);
    }
  }
	printf("Data read. Sending RR message to Sender...\n");
  if(s==0)
		sendMessage("RR0","R",fd);
  else
		sendMessage("RR1","R",fd);
}

void changestate2Read(char message,char bcc)
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
void changestate2Write(char message)
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
