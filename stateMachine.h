#include "includes.h"

int getStateSet();
int getStateUa();
int getStateDisc();

void resetStates();

void stateMachineSET(unsigned int message);
void stateMachineUA(unsigned int message);
void stateMachineDisc(unsigned int message);
void stateMachineDisc2(unsigned int message);
void stateMachineUaDisc(unsigned int message);
