#include "includes.h"
#include "macros.h"
#include "shared.h"

int llwrite(int fd,unsigned char *data,int length);
int llRead(int fd,unsigned char *guardar2);
void writeStuff(unsigned char data,int fd);
void touch2();
unsigned char unStuff(unsigned char first,unsigned char second);
void sendDataAux(unsigned char *data,int length,int fd);
void setZero();