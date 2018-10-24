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

  int fd=llOpen(argv[1]);

  makeConnectionReceiver(fd);

  receiveData(argv[2], fd);


  printf("Disconnecting...\n");
  disconnectReceiver(fd);



  llClose(fd);
  return 0;
}
