#include "includes.h"
#include "macros.h"
#include "shared.h"

void sendData(char *data2,int length2,int fd);
void readData(int fd,char *guardar2);
void writeStuff(char data,int fd);
void touch2();
char unStuff(char first,char second);
void sendDataAux(char *data,int length,int fd);
