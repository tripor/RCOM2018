#include "includes.h"
#include "macros.h"
#include "shared.h"

void sendData(unsigned int *data,int length,int fd);
void readData(int fd,unsigned int *guardar);
void writeStuff(unsigned int data,int fd);
void touch2();
unsigned int unStuff(unsigned int first,unsigned int second);
void sendDataAux(unsigned int *data,int length,int fd);
