#include "includes.h"
#include "macros.h"

int llWrite(int fd, char * buffer, int length);
int llRead(int fd, char * buffer);

int getAlarm();
void turnAlarm(int state);

void sendMessage(char *type,char * typeSender,int fd);

int llOpen();
void llClose(int fd);
