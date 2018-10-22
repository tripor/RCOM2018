#include "includes.h"
#include "shared.h"
#include "stateMachine.h"
#include "data.h"
#include "connect.h"
#include "disconnect.h"

int main(int argc, unsigned char** argv)
{
  if ( (argc != 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS1", argv[1])!=0) ))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS0\n");
    exit(1);
  }

  int fd=llOpen(argv[1]);

  makeConnectionReceiver(fd);
  unsigned char guardar[100];
  guardar[0]=70;
  readData(fd,guardar);
  for(int i=0;i<10;i++)
  {
    printf("valor: %x\n",guardar[i]);
  }

  printf("Disconnecting...\n");
  disconnectReceiver(fd);

  llClose(fd);
  return 0;
}
