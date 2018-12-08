#include "includes.h"

int makeConnection(char* ip,int port);
int writeMessage(int fd,char* message);
int readMessage(int fd,char* message);
int makeLogIn(int fd,char* user,char* pass);