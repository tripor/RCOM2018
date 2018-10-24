#include "testApplication.h"

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

   sendControlPackage(fd, size, filename, 0); //start

    
   /* int packetAmount = 0;
   
    unsigned char *fileSplitting = malloc(size);

 
    while(!feof(file)) {
        fscanf(file, "%c", &fileSplitting[packetAmount]);
        packetAmount++;
    }
    
    char packet = NULL;

     

    for(unsigned int i = 0; i < packetAmount; i++)
   { 
      memcpy(packet, &fileSplitting[i], sizeof(char));
      sendDataPackage(size, packet,fd);
   }

   
    sendControlPackage(fd, size, filename, 1);//end*/

    fclose(file);

    return;
}


void sendDataPackage(int size, char filePacket, int fd){ //a rever...

    /*unsigned char* data_package = (char*) malloc(size + 4);
    unsigned char buffer;
    unsigned int l2 = size/256;
    unsigned int l1 = size%256;

    data_package[0] = DataC;
    data_package[1] = SequenceN;
    data_package[2] = l2;
    data_package[3] = l1;
    memcpy(data_package, filePacket, size);

    sendData((unsigned char)data_package, size, fd);*/
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
    int packageSize = 1 + 1 + 1 + lengthSize + 1 + 1 +lengthName; //C + TLV1 + TLV2


    unsigned char *ctrl_package = malloc(sizeof(unsigned char)*packageSize);


      if(startOrEnd == 0)//start
        ctrl_package[0] = StartC;
      else if(startOrEnd == 1)//end
        ctrl_package[0] = EndC;
      else return;

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


    sendData(ctrl_package, packageSize, fd);
    return;


}

void receiveData(char* path, int fd){
/*
    //int STOP=FALSE;
  unsigned char fullMessage[100];
  //unsigned int thisByte;

  //Control Package
  unsigned int CData;
  unsigned int type;
  unsigned int length;
  unsigned int lengthCounter = 0;
  unsigned int fileSize;
  unsigned char fileName;
  unsigned int ctrlPackageSize;

  //Data Package
  unsigned int Cbyte;
  unsigned int Nread;
  unsigned int l2;
  unsigned int l1;
  unsigned int k; // l2 * 256 + l1;
  unsigned int dataCounter = 0;


  readData(fd,fullMessage);


//Leitura control package START
  CData = fullMessage[0];
  if(CData != StartC)
    printf("ERROR: Received data is wrong, doesn't start with the correct control package, current value: %x", CData);

  type = fullMessage[1]; //Ver FileSize
  if(type != 0)
    printf("ERROR: Received data is wrong, doesn't have the correct type value for the information sent (0), current value: %x", type);

  length = fullMessage[2];
  unsigned int value[length];

  while(lengthCounter < length)
  {
    value[lengthCounter] = fullMessage[2 + lengthCounter];
    lengthCounter++;
  }
  fileSize = value;



  type = fullMessage[3 + lengthCounter]; //Ver fileName
  if(type != 1)
    printf("ERROR: Received data is wrong, doesn't have the correct type value for the information sent (1), current value: %x", type);

  length = fullMessage[4 + lengthCounter];

  lengthCounter = 0;
  while(lengthCounter < length)
  {
    value[lengthCounter] = fullMessage[2 + lengthCounter];
    lengthCounter++;
  }

  fileName = value;

  ctrlPackageSize = length*2 + 5;

  //Leitura Data Package
  Cbyte = fullMessage[ctrlPackageSize + 1];

  if(CData != Cbyte)
    printf("ERROR: Received data is wrong, doesn't start with the data package, current value %x", Cbyte);

  Nread = fullMessage[ctrlPackageSize + 2];
  if(Nread != 255)
    printf("ERROR: Received data is wrong,parsed N value is not 255, current value %x", Nread);

  l2 = fullMessage[ctrlPackageSize + 3];
  l1 = fullMessage[ctrlPackageSize + 4];
  k = l2 * 256 + l1;
  unsigned int data_content[k];

  while(dataCounter < k){
    data_content[k] = fullMessage[ctrlPackageSize + 4 + dataCounter];
    dataCounter++;
  }
*/

    return;




}




