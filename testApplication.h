#include "includes.h"
#include "data.h"

void applicationSend(unsigned char fd, char* path);
void sendDataPackage(int size, int filePacket, int fd);
void sendControlPackage(int fd, int size,  char* path, int startOrEnd);
void receiveData(int size,  char* path);
