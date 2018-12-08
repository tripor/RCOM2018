#include "shared.h"
#include "includes.h"

int llOpen( char *canal);

void makeConnectionReceiver(int fd);
void makeConnectionSender(int fd);

void touchConnectSender();

void closeL(int fd);