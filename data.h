#include "includes.h"
#include "macros.h"
#include "shared.h"

void sendData(unsigned char *data2,int length2,int fd);
void readData(int fd,unsigned char *guardar2);
void writeStuff(unsigned char data,int fd);
void touch2();
unsigned char unStuff(unsigned char first,unsigned char second);
void sendDataAux(unsigned char *data,int length,int fd);
