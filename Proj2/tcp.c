#include "tcp.h"


int makeConnection(char* ip,int port)
{
    int sockfd;
	struct sockaddr_in server_addr;

	/*server address handling*/
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip); /*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(port);			  /*server TCP port must be network byte ordered */

	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(0);
	}
	/*connect to the server*/
	if (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
	{
		perror("connect()");
		exit(0);
	}

    printf("Connection to server made!\n");

    sleep(1);
    char buf[500];
    int len = read(sockfd, &buf, 500);
    buf[len] = '\0';
    printf("%s",buf);

    return sockfd;
}

int writeMessage(int fd,char* message)
{
    int res = write(fd,message,strlen(message));
    printf("%d",res);
    if(res != strlen(message))
        return 1;
    return 0;
}

int readMessage(int fd,char* message)
{
    sleep(1);
    int res = read(fd,message,255);
    message[res]='\0';
    return res;
}

int makeLogIn(int fd,char* user,char* pass)
{
    char* mandar=malloc((strlen(user)+6)*sizeof(char));
    sprintf(mandar,"USER %s\n",user);
    if(writeMessage(fd,mandar)!=0)
        return 1;
    char response[255];
    readMessage(fd,response);
    printf("%s\n",response);
    printf("Log in made\n");
    return 0;
}
