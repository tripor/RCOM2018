#include "auxiliar.h"

#define DEFAULT_PORT 21

int decomposeURL(char* url,Info* information)
{
    char *text;
    text = strtok(url,"[");
    if(strcmp(text,"ftp://") !=0)
    {
        printf("Paramatro dados incorreto( ftp:// ). Formato: ./run ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }
    text = strtok(NULL,":");
    strcpy(information->user,text);
    text = strtok(NULL,"]");
    if(text[strlen(text)-1] != '@')
    {
        printf("Paramatro dados incorreto( @] ). Formato: ./run ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }
    text[strlen(text)-1] = '\0';
    strcpy(information->pass,text);
    text = strtok(NULL,"/");
    strcpy(information->host,text);
    text = strtok(NULL," ");
    strcpy(information->url,text);
    text = strtok(NULL," ");
    if(text != NULL)
    {
        printf("Paramatro dados incorreto( sem fim ). Formato: ./run ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }
    information->port=DEFAULT_PORT;


    
    struct hostent *h;
    /*
    struct hostent {
        char    *h_name;	Official name of the host. 
        char    **h_aliases;	A NULL-terminated array of alternate names for the host. 
        int     h_addrtype;	The type of address being returned; usually AF_INET.
        int     h_length;	The length of the address in bytes.
        char    **h_addr_list;	A zero-terminated array of network addresses for the host. 
        Host addresses are in Network Byte Order. 
    };

    #define h_addr h_addr_list[0]	The first address in h_addr_list. 
    */
    if ((h = gethostbyname(information->host)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }
    
    strcpy(information->host_name,h->h_name);
    strcpy(information->ip,inet_ntoa(*((struct in_addr *)h->h_addr)));

    return 0;
}

