#include "testApplication.h"

//Peço desde já desculpa pelo esparguete, mas acho que nos sítios em que esteja mal dê para perceber bem a ideia

void applicationSend(char fd, char* path){



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


    sendControlPackage(fd, size, filename, 0); //start

    //fazer loop de mandar packages com o sendDataPackage, quero ter a certeza do tamanho a usar/como fazer divisões

    sendControlPackage(fd, size, filename, 0);//end

    return;
}

void sendDataPackage(int size, int filePacket, int fd){ //a rever...

    unsigned char* data_package = (unsigned char*) malloc(size + 4);
    unsigned char buffer;
    unsigned int l2 = size/256;
    unsigned int l1 = size%256;

    sprintf(buffer, "%d", DataC);
    strcat(data_package, buffer);
    sprintf(buffer, "%d", SequenceN);
    strcat(data_package, buffer);
    sprintf(buffer, "%d", l2);
    strcat(data_package, buffer);
    sprintf(buffer, "%d", l1);
    strcat(data_package, buffer);
    memcpy(data_package, filePacket, size);

    sendData(data_package, size, fd);
    return;
}

void sendControlPackage(int fd, int size, char* filename, int startOrEnd){



    char * stringSize;
    unsigned int typeSize = 0;
    unsigned int typeName = 1;
    unsigned int lengthName = strlen(filename);
    unsigned int valuePath = filename;
    sprintf(stringSize, "%d", size);
    unsigned int lengthSize = strlen(stringSize);
    unsigned int valueSize = size;

    int packageSize = 5 + lengthName + lengthSize; //C + TLV1 + TLV2
    unsigned char ctrl_package[packageSize];


      if(startOrEnd == 0)//start
      ctrl_package[0] = StartC;
      else if(startOrEnd == 1)//end
      ctrl_package[0] = EndC;
      else return -1;

      ctrl_package[1] = typeSize;
      ctrl_package[2] = lengthSize;
      //antes em baixo era (por exemplo) (valueSize >> 24)&& 0xff para só ficarem oito bits, mas achei que assim também dava
      ctrl_package[3] = (valueSize >> 24)%255;
      ctrl_package[4] = (valueSize >> 16)%255;
      ctrl_package[5] = (valueSize >> 8)%255;
      ctrl_package[6] = valueSize%255;

      ctrl_package[7] = typeName;
      ctrl_package[8] = lengthName;

      unsigned int lengthNameConstant = lengthName;
      unsigned int x;

      while(lengthName > 0)
      {
        x = lengthNameConstant - lengthName;

        ctrl_package[x+9] = filename[x];
        lengthName--;
      }


    sendData(ctrl_package, packageSize, fd);
    return;


}

void receiveData(char* path){

  unsigned int* fullMessage[100];
  unsigned int thisByte;
  void readData(int fd,unsigned int *fullMessage);

  if(fd[0] != StartC)
    {
      printf("ERROR: Doesn't have start control package;")
    }

  //resto faço amanhã


}
