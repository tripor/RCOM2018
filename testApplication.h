#include "includes.h"
#include "data.h"

void applicationSend(unsigned char fd, unsigned char* path);
void sendDataPackage(int size, int filePacket, int fd);
void sendControlPackage(int fd, int size, unsigned char* path, int startOrEnd);
void receiveData(int size, unsigned char* path);
