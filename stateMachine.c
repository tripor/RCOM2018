#include "shared.h"
#include "stateMachine.h"

int state_set=0;
int state_ua=0;
int data_state=0;
int disconnect_state=0;


int getStateSet()
{
    return state_set;
}
int getStateUa()
{
    return state_ua;
}
int getStateDisc()
{
    return disconnect_state;
}
void resetStates()
{
  state_set=0;
  state_ua=0;
  data_state=0;
  disconnect_state=0;
}
//
void stateMachineSET(unsigned char message)
{

    switch(state_set){
        case 0:
        if(message == FLAG)
            state_set = 1;
        break;

        case 1:
        if(message == Aemi)
            state_set = 2;
        else if(message != FLAG)
            state_set = 0;
        break;

        case 2:
        if(message == Cset)
            state_set = 3;
        else if (message == FLAG)
            state_set = 1;
        else
            state_set = 0;
        break;
        case 3:
        if(message == (Aemi^Cset))
            state_set = 4;
        else if (message == FLAG)
            state_set = 1;
        else
            state_set = 0;
        break;
        case 4:
        if(message == FLAG)
        state_set = 5;
        else state_set = 0;

        break;
        default: state_set = 0;

    }





}
//
void stateMachineUA(unsigned char message)
{
   switch(state_ua){
        case 0:
        if(message == FLAG)
            state_ua = 1;
        break;

        case 1:
        if(message == Arec)
            state_ua = 2;
        else if(message != FLAG)
            state_ua = 0;
        break;

        case 2:
        if(message == Cua)
            state_ua = 3;
        else if (message == FLAG)
            state_ua = 1;
        else
            state_ua = 0;
        break;
        case 3:
        if(message == (Arec^Cua))
            state_ua = 4;
        else if (message == FLAG)
            state_ua = 1;
        else
            state_ua = 0;
        break;
        case 4:
        if(message == FLAG)
        state_ua = 5;
        else state_ua = 0;
        break;
        default: state_ua = 0;

    }



}

//
void stateMachineDisc(unsigned char message)
{
    switch(disconnect_state){
        case 0:
            if(message == FLAG)
                disconnect_state = 1;
            break;
        case 1:
            if(message == Arec)
                disconnect_state = 2;
            else if(message != FLAG)
                disconnect_state = 0;
            break;
        case 2:
            if(message == Cdisc)
                disconnect_state = 3;
            else if (message == FLAG)
                disconnect_state = 1;
            else
                disconnect_state = 0;
            break;
        case 3:
            if(message == (Arec^Cdisc))
                disconnect_state = 4;
            else if (message == FLAG)
                disconnect_state = 1;
            else
                disconnect_state = 0;
            break;
        case 4:
            if(message == FLAG)
                disconnect_state = 5;
            else disconnect_state = 0;

            break;
        default: disconnect_state = 0;

    }
}

void stateMachineDisc2(unsigned char message)
{
    switch(disconnect_state){
        case 0:
            if(message == FLAG)
                disconnect_state = 1;
            break;
        case 1:
            if(message == Aemi)
                disconnect_state = 2;
            else if(message != FLAG)
                disconnect_state = 0;
            break;
        case 2:
            if(message == Cua)
                disconnect_state = 3;
            else if (message == FLAG)
                disconnect_state = 1;
            else
                disconnect_state = 0;
            break;
        case 3:
            if(message == (Aemi^Cua))
                disconnect_state = 4;
            else if (message == FLAG)
                disconnect_state = 1;
            else
                disconnect_state = 0;
            break;
        case 4:
            if(message == FLAG)
                disconnect_state = 5;
            else disconnect_state = 0;

            break;
        default: disconnect_state = 0;

    }
}

void stateMachineUaDisc(unsigned char message)
{
    switch(disconnect_state){
        case 0:
            if(message == FLAG)
                disconnect_state = 1;
            break;
        case 1:
            if(message == Aemi)
                disconnect_state = 2;
            else if(message != FLAG)
                disconnect_state = 0;
            break;
        case 2:
            if(message == Cdisc)
                disconnect_state = 3;
            else if (message == FLAG)
                disconnect_state = 1;
            else
                disconnect_state = 0;
            break;
        case 3:
            if(message == (Aemi^Cdisc))
                disconnect_state = 4;
            else if (message == FLAG)
                disconnect_state = 1;
            else
                disconnect_state = 0;
            break;
        case 4:
            if(message == FLAG)
                disconnect_state = 5;
            else disconnect_state = 0;

            break;
        default: disconnect_state = 0;

    }
}
