#ifndef SOCKETUTILS_HEADER
#define SOCKETUTILS_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
void say(char[] msg);
int crearSocket(char* ip, char* puerto, struct addrinfo *servinfo);


#endif

