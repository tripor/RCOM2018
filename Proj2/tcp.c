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
    
    if(port==21)
    {
        usleep(100000);
        char buf[1000];
        int len = read(sockfd, &buf, 1000);
        buf[len] = '\0';
        printf("%s",buf);
    }

    return sockfd;
}

int writeMessage(int fd,char* message)
{
    int res = write(fd,message,strlen(message));
    if(res != strlen(message))
        return 1;
    return 0;
}

int readMessage(int fd,char* message)
{
    usleep(100000);
    int res = read(fd,message,255);
    message[res]='\0';
    return res;
}

int makeLogIn(int fd,char* user,char* pass)
{
    char* mandar=malloc((strlen(user)+6)*sizeof(char));
    sprintf(mandar,"user %s\n",user);
    if(writeMessage(fd,mandar))
        return 1;
    char response[255];
    readMessage(fd,response);
    if(response[0]!='3' || response[1]!='3' || response[2]!='1')
        return 1;
    printf("%s",response);
    free(mandar);
    mandar=malloc((strlen(pass)+6)*sizeof(char));
    sprintf(mandar,"pass %s\n",pass);
    if(writeMessage(fd,mandar))
        return 1;
    readMessage(fd,response);
    if(response[0]!='2' || response[1]!='3' || response[2]!='0')
        return 1;
    printf("%s",response);
    free(mandar);
    return 0;
}

int setPassive(int fd, char*ip , int* port)
{
    char response[255];
    if(writeMessage(fd,"pasv\n"))
        return 1;
    readMessage(fd,response);
    if(response[0]!='2' || response[1]!='2' || response[2]!='7')
        return 1;
    printf("%s",response);
    int bytes[6];
    sscanf(response,"227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).",&bytes[0],&bytes[1],&bytes[2],&bytes[3],&bytes[4],&bytes[5]);
    sprintf(ip,"%d.%d.%d.%d",bytes[0],bytes[1],bytes[2],bytes[3]);
    *port=bytes[4]*256 +bytes[5];
    return 0;
}

int getPath(int fd,char* url)
{
    char response[255];
    char* mandar=malloc((strlen(url)+6)*sizeof(char));
    sprintf(mandar,"retr %s\n",url);
    if(writeMessage(fd,mandar))
        return 1;
    readMessage(fd,response);
    if(response[0]!='1' || response[1]!='5' || response[2]!='0')
    {
        printf("%s",response);
        return 1;
    }
    printf("%s",response);
    free(mandar);
    return 0;
}


int getFile(int fd,char* filename)
{
    sleep(1);
    char* path=malloc(strlen(filename)+6);
    sprintf(path,"files/%s",filename);
    int fileFd = open(path,O_WRONLY | O_TRUNC | O_CREAT	,0600);
    if(fileFd <0)
        return 1;
    char ler;
    int res;
    while((res = read(fd,&ler,1)))
    {
        if(res < 1)
            return 1;
        if(write(fileFd,&ler,1)!=1)
            return 1;
    }
    close(fileFd);
    return 0;
}
