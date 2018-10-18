#include "shared.h"

volatile int STOP=FALSE;
struct termios *oldtio,*newtio;


void llWrite(char *send,int fd)
{
  int res=write(fd,send,strlen(send)+1);
  printf("Bytes send: %d\n",res);
}

void llRead(char **guardar,int fd)
{
  char ler,buf[255]="";
	int i=0;
  while (STOP==FALSE)
	{
   	read(fd,&ler,1);
		buf[i]=ler;
		i++;
    if (ler=='\0' )
		  STOP=TRUE;
  }
  strcpy(*guardar,buf);
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

  (*newtio).c_cc[VTIME]    = 30;
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
