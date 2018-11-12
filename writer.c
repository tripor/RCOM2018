#include "includes.h"
#include "shared.h"
#include "stateMachine.h"
#include "data.h"
#include "connect.h"
#include "disconnect.h"
#include "testApplication.h"

int main(int argc, char** argv)
{
  if ( (argc != 3) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS1", argv[1])!=0) ))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS0\n");
    exit(1);
  }
  setWrite();
  int fd=llOpen(argv[1]);
  struct timeval tv;


  gettimeofday(&tv,NULL);
  long long int begin=tv.tv_usec;
  long long int segBegin=tv.tv_sec * 1000000 + begin;

  applicationSend(fd,argv[2]);
 
  gettimeofday(&tv,NULL);
  long long int end=tv.tv_usec;
  long long int segEnd=tv.tv_sec * 1000000 + end;
 
    printf("Time elpased is %lf \n",(segEnd-segBegin)/1000000.0);

  printf("Disconnecting...\n");
  
  llClose(fd);

  return 0;
}

