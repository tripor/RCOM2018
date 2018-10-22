#include "includes.h"
#include "shared.h"
#include "stateMachine.h"
#include "data.h"
#include "connect.h"
#include "disconnect.h"

int main(int argc, char** argv)
{
  if ( (argc != 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS1", argv[1])!=0) ))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS0\n");
    exit(1);
  }

  int fd=llOpen(argv[1]);

  makeConnectionSender(fd);



  unsigned char mandar[10]={1,2,3,4,5,6,7,8,9,10};
  sendData(mandar,10,fd);




  printf("Disconnecting...\n");
  disconnectSender(fd);
  llClose(fd);
  return 0;
}
