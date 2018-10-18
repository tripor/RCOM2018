#include "shared.h"

volatile int STOP=FALSE;

uint8_t state = 0;

void stateMachineSET(uint8_t message)
{   

    switch(state){
        case 0:
        if(message == FLAG)
            state = 1;
        break;

        case 1:
        if(message == Arec)
            state = 2;
        else if(message != FLAG)
            state = 0;
        break;

        case 2:
        if(message == Cset)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else 
            state = 0;
        break;
        case 3:
        if(message == Arec^Cset)
            state = 4;
        else if (message == FLAG)
            state = 1;
        else 
            state = 0;
        break;
        case 4:
        if(message == FLAG)
        state = 5;
        else state = 0;

        default: state = 0; 

    }





}

void stateMachineUA(uint8_t message)
{   
    
   // SET=[FLAG,A,C,BCC,FLAG]
   // UA = [FLAG,A,C,BCC,FLAG]
   switch(state){
        case 0:
        if(message == FLAG)
            state = 1;
        break;

        case 1:
        if(message == Aemi)
            state = 2;
        else if(message != FLAG)
            state = 0;
        break;

        case 2:
        if(message == Cua)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else 
            state = 0;
        break;
        case 3:
        if(message == Aemi^Cua)
            state = 4;
        else if (message == FLAG)
            state = 1;
        else 
            state = 0;
        break;
        case 4:
        if(message == FLAG)
        state = 5;
        else state = 0;

        default: state = 0; 

    }

    

}

void resetState()
{
    state = 0;
}