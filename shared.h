#include "includes.h"
#include "macros.h"

int llWrite(int fd, unsigned char * buffer, int length);
int llRead(int fd, unsigned char * buffer);

int getAlarm();
void turnAlarm(int state);

void sendMessage(unsigned char *type,unsigned char * typeSender,int fd);

int llOpen();
void llClose(int fd);
