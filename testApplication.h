#include "includes.h"

void applicationSend(int fd, char* path);
void sendControlPackage(int fd, int size,  char* path, int startOrEnd);
void receiveData( int fd);
