#include "includes.h"
#include "data.h"

void applicationSend(int fd, char* path);
void sendDataPackage(int size, char filePacket, int fd);
void sendControlPackage(int fd, int size,  char* path, int startOrEnd);
void receiveData( char* path,int fd);
