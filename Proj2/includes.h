#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

typedef struct Info {
    int port;
    char user[255];
    char pass[255];
    char host[255];
    char url[255];
    char host_name[255];
    char ip[16];
    char filename[255];
}Info;