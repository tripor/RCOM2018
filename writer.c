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

  clock_t begin = clock();

  applicationSend(fd,argv[2]);

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time: %lf seconds\n",time_spent);


  printf("Disconnecting...\n");
  
  llClose(fd);

  return 0;
}
