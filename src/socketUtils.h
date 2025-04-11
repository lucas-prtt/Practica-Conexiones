#ifndef SOCKETUTILS_HEADER
#define SOCKETUTILS_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

typedef struct{
    int size;
    void * stream;
    paquete * siguiente_segmento;
} paquete;

void say(char[] msg);

int crearSocket(char* ip, char* puerto, struct addrinfo *servinfo);
int crearSocketServer(char* puerto);
int conectarSocketClient(char* ip, char* puerto);

int esperarClientes(int socket_server, void (*atenderCliente)(void*));

void enviar_paquete(int socket, paquete * pac);
void eliminarPaquete(paquete * pac);
void agregarAPaquete(paquete * paquete, void * elementoParaAgregar, int size);
paquete * crearPaquete();






#endif

