#ifndef UTILS_
#define UTILS_

#include "room.h"

int getListenerSocket(char *port, int backlog);
int getConnectionSocket(char *port, char *host);

#endif