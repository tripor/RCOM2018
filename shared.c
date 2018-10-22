#include "shared.h"

struct termios *oldtio,*newtio;


int llWrite(int fd, char * buffer, int length)
{
  int res=write(fd,buffer,length);
  printf("Bytes send: %d Text: %s\n",res,buffer);
  return res;
}

int llRead(int fd, char * buffer)
{
  char ler,buf[255]="";
	int i=0;
  while (1)
	{
   	read(fd,&ler,1);
    printf("Byte read: %d Text: %c\n",1,ler);
    if (ler=='\0' )
      break;
		buf[i]=ler;
		i++;
  }
  strcpy(buffer,buf);
  return i;
}


int llOpen(char *canal)
{
  int fd;

  oldtio=(struct termios *)malloc(sizeof(struct termios));
  newtio=(struct termios *)malloc(sizeof(struct termios));
  // Open port to read
  fd = open(canal, O_RDWR | O_NOCTTY );

  if (fd <0) {perror(canal); exit(-1); }

  if ( tcgetattr(fd,oldtio) == -1) { /* save current port settings */
    perror("tcgetattr");
    exit(-1);
  }
  bzero(newtio, sizeof(*newtio));
  (*newtio).c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
  (*newtio).c_iflag = IGNPAR;
  (*newtio).c_oflag = 0;

  /* set input mode (non-canonical, no echo,...) */
  (*newtio).c_lflag = 0;

  (*newtio).c_cc[VTIME]    = 1;
  (*newtio).c_cc[VMIN]     = 1;
  tcflush(fd, TCIOFLUSH);

  if ( tcsetattr(fd,TCSANOW,newtio) == -1) {
    perror("tcsetattr");
    exit(-1);
  }

  printf("New termios structure set\n");
  return fd;
}

void llClose(int fd)
{
	sleep(2);
  tcsetattr(fd,TCSANOW,oldtio);
  close(fd);
}


void sendMessage(char *type,char * typeSender,int fd)
{
  char send[2],a,c;
  send[1]='\0';
  if(strcmp("W",typeSender)==0) a=Aemi;
  else a=Arec;
  if(strcmp("SET",type)==0) c=Cset;
  else if(strcmp("UA",type)==0) c=Cua;
  else if(strcmp("DISC",type)==0) c=Cdisc;
  else if(strcmp("RR1",type)==0) c=Crr1;
  else if(strcmp("RR0",type)==0) c=Crr0;
  else if(strcmp("REJ1",type)==0) c=Crej1;
  else if(strcmp("REJ2",type)==0) c=Crej0;

  send[0]=FLAG;
  llWrite(fd,send,2);

  send[0]=a;
  llWrite(fd,send,2);

  send[0]=c;
  llWrite(fd,send,2);

  send[0]=a^c;
  llWrite(fd,send,2);

  send[0]=FLAG;
  llWrite(fd,send,2);
}
