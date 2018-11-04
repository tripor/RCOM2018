#include "testApplication.h"
#include "data.h"

int app_stage=2;
int file_size;
char my_filename[255];


void sendData(unsigned char *text, int fd,int seq,int leng){

  unsigned char * mandar=malloc(1+1+1+1+PackageSize);
  int i=0;
  mandar[i]=DataC;
  i++;
  mandar[i]=seq;
  i++;
  unsigned char L2=leng/256;
  unsigned char L1=leng%256;
  mandar[i]=L2;
  i++;
  mandar[i]=L1;
  i++;
  printf("L2:%d L1:%d\n",L2,mandar[i-1]);
  for(int j=0;i<leng+4;i++,j++)
  {
    mandar[i]=text[j];
  }
  llwrite(fd,mandar, PackageSize+4);
  free(mandar);
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
  rewind(file);
  char *filename = basename(path);

  sendControl(fd, size, filename, StartC); //start
  printf("Control Tram start sent.\n");

  unsigned char *fileText = calloc(size,sizeof(unsigned char));
  int ch;
  unsigned long long int i=0;
  while((ch = fgetc(file)) != EOF)
  {
    fileText[i]=ch;
    i++;

  }
  unsigned char *text=calloc(PackageSize+1,sizeof(unsigned char));
  int j=1;
  for(i=0;i<(sizeof(char)*size)-PackageSize;i+=sizeof(char)*PackageSize,j++)
  {
    for(int k=0;k<sizeof(char)*PackageSize;k++)
    {
      text[k]=(*fileText);
      fileText+=sizeof(char);
    }
    sendData(text,fd,j,sizeof(char)*PackageSize);
  }
  i+=PackageSize;
  for(int k=0;k<size-(i-sizeof(char)*PackageSize);k++)
  {
    text[k]=(*fileText);
    fileText+=sizeof(char);
  }
  sendData(text,fd,j,size-(i-sizeof(char)*PackageSize));
  printf("All data sent.\n");
  sendControl(fd, size, filename, EndC);
  printf("Control Tram end sent.\n");

  fclose(file);

  return;
}


void sendControl(int fd, int size, char* filename, int startOrEnd){



    char  stringSize[255];


    unsigned int typeSize = 0;
    sprintf(stringSize, "%d", size);
    unsigned int lengthSize = strlen(stringSize);

    unsigned int typeName = 1;
    unsigned int lengthName = strlen(filename);




    //Tamanho do pacote de controlo
    int pacoteSize = 1 + 1 + 1 + lengthSize + 1 + 1 +lengthName; //C + TLV1 + TLV2


    unsigned char *ctrl_Tram = malloc(sizeof(unsigned char)*pacoteSize);


    ctrl_Tram[0] = startOrEnd;

    int i=1;

    ctrl_Tram[i] = typeSize;
    i++;
    ctrl_Tram[i] = lengthSize;
    i++;

    for(int k=0;k<lengthSize;k++,i++)
    {
      ctrl_Tram[i]=stringSize[k];
    }

    ctrl_Tram[i] = typeName;
    i++;
    ctrl_Tram[i] = lengthName;
    i++;

    for(int k=0;k<lengthName;k++,i++)
    {
      ctrl_Tram[i]=filename[k];
    }

    printf("Sending Control.\n");
    llwrite(fd,ctrl_Tram, pacoteSize);
    return;


}

void receiveDataRead(int fd)
{
  unsigned long long int total=0;
  unsigned char *text= malloc(sizeof(unsigned char)*(file_size+1));
  unsigned char *lido=malloc(sizeof(unsigned char)*(PackageSize+10));
  while(total<file_size)
  {
    llRead(fd,lido);
    int i=2;
    int L2=lido[i];
    i++;
    int L1=lido[i];
    i++;
    int numeroDeOctetos=L2*256+L1;
    for(int j=i;j<numeroDeOctetos+4 && total<file_size;j++,total++)
    {
      text[total]=lido[j];
    }
  }
  int file=open(my_filename,O_CREAT | O_WRONLY | O_TRUNC,0600);
  for(unsigned long long int i=0;i<file_size;i++)
  {
    write(file,&(text[i]),1);
  }
  close(file);
}



void receiveControl(int fd, int startOrEnd)
{
  unsigned char *message=calloc(255,sizeof(unsigned char));
  char takeOff[255];
  llRead(fd,message);
  int i=0;
  if(message[i]==startOrEnd)
  {
    app_stage=startOrEnd;
  }
  i++;
  int sizeOcteto=0;
  printf("%d\n",message[0]);
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

  receiveControl(fd,StartC);
  printf("Received control tram start.\n");
  receiveDataRead(fd);
  printf("All data received.\n");
  receiveControl(fd,EndC);
  printf("Received control tram end.\n");
  return;
}
