#include "shared.h"

volatile int STOP=FALSE;
void lerCabo(char **guardar,int fd)
{
  char ler,buf[255]="";
	int i=0,res;
  while (STOP==FALSE)
	{
   	res = read(fd,&ler,1);
		buf[i]=ler;
		i++;
    if (ler=='\0')
		  STOP=TRUE;
  }
	printf("%s\n",buf);
}
