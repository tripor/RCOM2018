#include "includes.h"

int makeConnection(char* ip,int port);
int writeMessage(int fd,char* message);
int readMessage(int fd,char* message);
int makeLogIn(int fd,char* user,char* pass);
int setPassive(int fd, char*ip , int* port);
int getPath(int fd,char* url);
int getFile(int fd,char* filename);