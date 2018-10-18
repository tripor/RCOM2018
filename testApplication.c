#include "includes.h"

void applicationSend(char fd, char* path){

    sendControlPackage(int size, char* name, int size, int startOrEnd);

    FILE * file;
    file = fopen(path, "rb");
    int size = file.size;
    int fileToGo= fread(fileBuf, sizeof(char), max_size, file);
    while(fileToGo > 0)
    {
        fileToGo +=
    }
    sendControlPackage(int size, char* name, int size, int startOrEnd);
}

void sendDataPackage(int size, filePacket, int fd){

    unsigned char* data_package = (unsigned char*) malloc(size + 4);
   
    data_package[0] = 1; //C
    data_package[1] = 255; //N
    data_package[2] = size/256; //L2
    data_package[3] = size/%256; //L1
    memcpy(&package[4], filePacket, size);


}

void sendControlPackage(int size, char* path, int startOrEnd){
    

    unsigned int cStart = 2; //Start
    unsigned int cEnd = 3; //End
    unsigned int type = 255;//Sequence Number
    unsigned int length = size/256;
    unsigned int value = size%256;
  
   
    int packageSize = 5 + strlen(fileSize) + strlen(fileName);
    unsigned char ctrl_package[packageSize];

    if(startOrEnd == 0){//Start
    ctrl_package[0] = cStart;
    }
    else if(startOrEnd == 1){//End
    ctrl_package[0] = cEnd;
    }
    else return -1;

   

}

