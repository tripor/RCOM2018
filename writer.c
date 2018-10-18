#include "includes.h"
#include "shared.h"
#include "stateMachine.h"
#include "data.h"

int main(int argc, char** argv)
{
  if ( (argc != 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS1", argv[1])!=0) ))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS0\n");
    exit(1);
  }

  int fd=llOpen(argv[1]);

  makeConnection(fd,'W');

  unsigned int mandar[10]={1,2,3,4,5,6,7,8,9,10};
  sendData(mandar,10,fd);
  sendData(mandar,10,fd);

  llClose(fd);
  return 0;
}
