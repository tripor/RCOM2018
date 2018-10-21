#include "shared.h"
#include "stateMachine.h"

int connect_state=0;
int data_state=0;
int disconnect_state=0;

void stateMachineSET(unsigned int message)
{

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
        if(message == Cset)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 3:
        if(message == (Aemi^Cset))
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

        break;
        default: state = 0;

    }





}

void stateMachineUA(unsigned int message)
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
        if(message == Cua)
            state = 3;
        else if (message == FLAG)
            state = 1;
        else
            state = 0;
        break;
        case 3:
        if(message == (Arec^Cua))
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
        break;
        default: state = 0;

    }



}

void resetState()
{
    state = 0;
}
