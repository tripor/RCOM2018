#include "includes.h"
#include "auxiliar.h"
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
    printf("Filename: %s\n",information.filename);
    printf("Port: %d\n",information.port);
    printf("Host-name: %s\n",information.host_name);
    printf("IP: %s\n",information.ip);

    int fd = makeConnection(information.ip,information.port);
    if(makeLogIn(fd,information.user,information.pass)!=0)
    {
        printf("Error making log in.\n");
        exit(2);
    }
    char pasv_ip[16];
    int pasv_port;
    if(setPassive(fd,pasv_ip,&pasv_port)!=0)
    {
        printf("Error in setting pasv\n");
        exit(2);
    }
    printf("Ip pasv: %s\n",pasv_ip);
    printf("Port pasv: %d\n",pasv_port);

    int pasv_fd = makeConnection(pasv_ip,pasv_port);

    if(getPath(fd,information.url)!=0)
    {
        printf("Error setting the path.\n");
        exit(2);
    }

    if(getFile(pasv_fd,information.filename)!=0)
    {
        printf("Error downloading the file\n");
        exit(2);
    }

    close(fd);
    close(pasv_fd);
    return 0;
}
