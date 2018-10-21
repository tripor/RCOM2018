#include "includes.h"
#include "shared.h"
#include "stateMachine.h"
#include "data.h"
#include "connect.h"

int main(int argc, char** argv)
{
  if ( (argc != 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS1", argv[1])!=0) ))
  {
    printf("Usage:\tnserial SerialPort\n\tex: nserial /dev/ttyS0\n");
    exit(1);
  }

  int fd=llOpen(argv[1]);

  makeConnectionReceiver(fd);
  unsigned int guardar[100];
  readData(fd,guardar);
  for(int i=0;i<10;i++)
  {
    printf("valor: %x\n",guardar[i]);
  }

  llClose(fd);
  return 0;
}
