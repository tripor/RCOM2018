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

    //FAZER O WHILE E O SPLIT MESSAGE
    sendControlPackage(fd, size, filename, 1);//end

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

void receiveData(char* path, int fd){

  STOP=FALSE;
  unsigned int* fullMessage[100];
  unsigned int thisByte;

  //Control Package
  unsigned int CData;
  unsigned int type;
  unsigned int length;
  unsigned int lengthCounter = 0;
  unsigned int fileSize;
  char fileName;
  unsigned int ctrlPackageSize;

  //Data Package
  unsigned int Cbyte;
  unsigned int Nread[2];
  unsigned int l2;
  unsigned int l1;
  unsigned int k; // l2 * 256 + l1;
  unsigned int dataCounter = 0;


  readData(int fd,unsigned int *fullMessage);


//Leitura control package START
  CData = fullMessage[0];
  if(CData != StartC)
    printf("ERROR: Received data is wrong, doesn't start with the correct control package, current value: %x", CData);

  type = fullMessage[1]; //Ver FileSize
  if(type != 0)
    printf("ERROR: Received data is wrong, doesn't have the correct type value for the information sent (0), current value: %x", type);

  length = fullMessage[2];
  unsigned int* value[length];

  while(lengthCounter < length)
  {
    value[lengthCounter] = fullMessage[2 + lengthCounter]
    lengthCounter++;
  }
  fileSize = value;



  type = fullMessage[3 + lengthCounter]; //Ver fileName
  if(type != 1)
    printf("ERROR: Received data is wrong, doesn't have the correct type value for the information sent (1), current value: %x", type);

  length = fullMessage[4 + lengthCounter];
  unsigned int* value[length];
  lengthCounter = 0;
  while(lengthCounter < length)
  {
    value[lengthCounter] = fullMessage[2 + lengthCounter]
    lengthCounter++;
  }

  fileName = value;

  ctrlPackageSize = length*2 + 5;

  //Leitura Data Package
  Cbyte = fullMessage[ctrlPackageSize + 1]
  if(CData != Cbyte)
    printf("ERROR: Received data is wrong, doesn't start with the data package, current value %x", Cbyte);

  Nread[0] = fullMessage[ctrlPackageSize + 2];
  Nread[1] = fullMessage[ctrlPackageSize + 3];
  if(Nread != 255)
    printf("ERROR: Received data is wrong,parsed N value is not 255, current value %x", Nread);

  l2 = fullMessage[ctrlPackageSize + 4];
  l1 = fullMessage[ctrlPackageSize + 5];
  k = l2 * 256 + l1;
  unsigned int data_content[k];

  while(dataCounter < k)){
    data_content[k] = fullMessage[ctrlPackageSize + 5 + dataCounter];
    dataCounter++;
  }

  //Leitura control package END
  unsigned int tmpPackage1[ctrlPackageSize], tmpPackage2[ctrlPackageSize];

  tmpPackage1 = fullMessage >> (ctrlPackageSize + 5 + dataCounter);
  tmpPackage2 = fullMessage%(ctrlPackageSize + 5 + dataCounter);

  tmpPackage1 = tmpPackage1 >> 1;

  if(tmpPackage2[0] == EndC)
    printf("ERROR: Received data is wrong, doesn't start with the correct control package, current value: %x", tmpPackage2[0]);

    tmpPackage2 = tmpPackage2 >> 1;

    if(tmpPackage2 != tmpPackage1){
    printf("ERROR: Control packages don't matcH");
    }

    return;


}
