#include "includes.h"
#include "aux.h"
#include "tcp.h"

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Numero incorreto de parametro. Formato: ./run ftp://[<user>:<password>@]<host>/<url-path>");
        exit(1);
    }

    Info information;

    decomposeURL(argv[1],&information);

    printf("User: %s\n",information.user);
    printf("Pass: %s\n",information.pass);
    printf("Host: %s\n",information.host);
    printf("Url-Path: %s\n",information.url);
    printf("Port: %d\n",information.port);
    printf("Host-name: %s\n",information.host_name);
    printf("IP: %s\n",information.ip);

    makeConnection(information.ip,information.port);

    return 0;
}
