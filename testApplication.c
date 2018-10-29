#include "testApplication.h"

int app_stage=2;
int file_size;
char my_filename[255];


void sendDataPackage(char *text, int fd,int seq){

  unsigned char * mandar=malloc(1+1+1+1+PackageSize);
  int i=0;
  mandar[i]=DataC;
  i++;
  mandar[i]=seq;
  i++;
  unsigned char L2=strlen(text)/256;
  unsigned char L1=strlen(text)%256;
  mandar[i]=L2;
  i++;
  mandar[i]=L1;
  i++;
  for(int j=0;i<strlen(text)+4;i++,j++)
  {
    mandar[i]=text[j];
  }
  sendData(mandar, PackageSize+4, fd);
  return;
}


void applicationSend(int fd, char* path){


  FILE * file;
  file = fopen(path, "rb");
  if(file == NULL)
  {
    perror("\n ERROR: File didn't open. \n");
    return;
  }
  int size;
  fseek(file,0,2);
  size = ftell(file);

  char *filename = basename(path);

  sendControlPackage(fd, size, filename, StartC); //start
  printf("Control package start sent.\n");
  
  
  char *fileText = calloc(size,sizeof(char));

  char ch;
  int i=0;
  while((ch = fgetc(file)) != EOF)
  {
    fileText[i]=ch;
    i++;
  }
    
  char *text=calloc(PackageSize+1,sizeof(char));
  int j=1;
  for(i=0;i<sizeof(char)*size;i+=sizeof(char)*PackageSize,j++)
  {
    fileText+=i;
    memcpy(text,fileText,sizeof(char)*PackageSize);
    sendDataPackage(text,fd,j);
  }  
  printf("All data sent.\n");

  sendControlPackage(fd, size, filename, EndC);
  printf("Control package end sent.\n");

  fclose(file); 

  return;
}


void sendControlPackage(int fd, int size, char* filename, int startOrEnd){



    char  stringSize[255];


    unsigned int typeSize = 0;
    sprintf(stringSize, "%d", size);
    unsigned int lengthSize = strlen(stringSize);

    unsigned int typeName = 1;
    unsigned int lengthName = strlen(filename);




    //Tamanho do pacote de controlo
    int pacoteSize = 1 + 1 + 1 + lengthSize + 1 + 1 +lengthName; //C + TLV1 + TLV2


    unsigned char *ctrl_package = malloc(sizeof(unsigned char)*pacoteSize);


    ctrl_package[0] = startOrEnd;

    int i=0;

    ctrl_package[i] = typeSize;
    i++;
    ctrl_package[i] = lengthSize;
    i++;
    
    for(int k=lengthSize-1;k>=0;k--,i++)
    {
      ctrl_package[i]=stringSize[k];
    }

    ctrl_package[i] = typeName;
    i++;
    ctrl_package[i] = lengthName;
    i++;

    for(int k=lengthName-1;k>=0;k--,i++)
    {
      ctrl_package[i]=filename[k];
    }


    sendData(ctrl_package, pacoteSize, fd);
    return;


}

void receiveDataRead(int fd)
{
  int total=0;
  unsigned char *text= malloc(sizeof(unsigned char)*file_size);
  while(total!=PackageSize)
  {
    unsigned char *lido=malloc(sizeof(unsigned char)*(PackageSize+5));
    readData(fd,lido);
    int i=2;
    int L2=lido[i];
    i++;
    int L1=lido[i];
    i++;
    int numeroDeOctetos=L2*256+L1;
    for(int j=i;j<numeroDeOctetos+4;j++,total++)
    {
      text[total]=lido[j];
    }
    free(lido);
  }
  int file=open(my_filename,O_CREAT | O_WRONLY | O_TRUNC,0600);
  for(int i=0;i<PackageSize;i++)
  {
    write(file,&(text[i]),1);
  }
  close(file);
}



void receiveControlPackage(int fd, int startOrEnd)
{
  unsigned char *message=calloc(255,sizeof(unsigned char));
  char takeOff[255];
  readData(fd,message);
  int i=0;
  if(message[i]==startOrEnd)
  {
    app_stage=startOrEnd;
  }
  i++;
  int sizeOcteto=0;
  do
  {
    if(message[i]==0)
    {
      i++;
      sizeOcteto=message[i];
      i++;
      int j=0;
      for(;sizeOcteto>0;i++,sizeOcteto--,j++)
      {
        takeOff[j]=message[i];
      }
      takeOff[j]='\0';
      sscanf(takeOff,"%d",&file_size);
    }
    else if(message[i]==1)
    {
      i++;
      sizeOcteto=message[i];
      i++;
      int j=0;
      for(;sizeOcteto>0;i++,sizeOcteto--,j++)
      {
        takeOff[j]=message[i];
      }
      takeOff[j]='\0';
      sscanf(takeOff,"%s",my_filename);
    }
  }while(message[i]!=0);
  printf("Filename: %s Size: %d \n",my_filename,file_size);
}

void receiveData(int fd){

  receiveControlPackage(fd,StartC);
  printf("Received control package start.\n");
  receiveDataRead(fd);
  printf("All data received.\n");
  receiveControlPackage(fd,EndC);
  printf("Received control package end.\n");
  return;
}




