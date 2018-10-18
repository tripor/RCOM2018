#include "includes.h"
#include "shared.h"
#include "stateMachine.h"

int main(int argc, char** argv)
{
  if ( (argc != 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS1", argv[1])!=0) ))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS0\n");
    exit(1);
  }

  int fd=llOpen(argv[1]);

  makeConnection(fd,'W');
  
  llClose(fd);
  return 0;
}
