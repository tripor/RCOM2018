#include "includes.h"

int getStateSet();
int getStateUa();
int getStateDisc();

void resetStates();

void stateMachineSET(unsigned char message);
void stateMachineUA(unsigned char message);
void stateMachineDisc(unsigned char message);
void stateMachineDisc2(unsigned char message);
void stateMachineUaDisc(unsigned char message);
