#include "includes.h"
#include "macros.h"

void setRead();
void setWrite();
/**
 * 0-write 1-read
 */
int getType();

int writeByte(int fd,unsigned char byte);

void sendMessage( char *type, char * typeSender,int fd);

