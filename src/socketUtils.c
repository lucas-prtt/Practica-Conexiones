#include "socketUtils.h"


void say(char msg[]){
    printf("%s",msg);
}

int crearSocket(char* ip, char* puerto, struct addrinfo * * server_info){
    int soc;
    struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // IGNORADA SI ip != NULL (en cliente)

	getaddrinfo(ip, puerto, &hints, server_info);
    soc = socket((*server_info)->ai_family, (*server_info)->ai_socktype, (*server_info)->ai_protocol);
    setsockopt(soc, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
    return soc;
}

int crearSocketServer(char* puerto){
    int soc;
    struct addrinfo * serverInfo = NULL;
    soc = crearSocket(NULL, puerto, &serverInfo);
    bind(soc, serverInfo->ai_addr, serverInfo->ai_addrlen);
	listen(soc, SOMAXCONN);
	freeaddrinfo(serverInfo);
    return soc;
}

int conectarSocketClient(char* ip, char* puerto){
    int soc;
    struct addrinfo * serverInfo = NULL;
    soc = crearSocket(ip, puerto, &serverInfo);
    connect(soc, serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo);
    return soc;
}

int esperarClientes(int socket_server, void (*atenderCliente)(void*)){ //EsperarClientes(): Se queda siempre en esta funcion la cual crea threads. Toma la funcion atenderCliente para crear el thread "
    while (1) { //Copiada de utnso.com (guia sockets), hay que ver si anda
        pthread_t thread;
        int *fd_conexion_ptr = malloc(sizeof(int));
        *fd_conexion_ptr = accept(socket_server, NULL, NULL);
        pthread_create(&thread,
                    NULL,
                    (void*) atenderCliente,
                    fd_conexion_ptr);
        pthread_detach(thread);
    }
}

void enviarStream(int socket, void* stream, int size){
    send(socket, stream, size, 0);
    return;
}
void agregarAStream(void * stream, int * offset, void * content, int size){
    memcpy(stream + offset, content, size);
    offset += size;
    return;
}
paquete * crearPaquete(){
    paquete * pac = malloc(sizeof(paquete));
    pac->siguiente_segmento = NULL;
    pac->size = -1;
    return pac;
}
void agregarAPaquete(paquete * paquete, void * elementoParaAgregar, int size){
    paquete * segmento = paquete;
    while(segmento->siguiente_segmento != NULL){
        segmento = segmento->siguiente_segmento;
    }
    if (segmento->tamanio != -1){
    segmento->siguiente_segmento = crearPaquete();
    segmento = segmento->siguiente_segmento;
    }
    segmento->size = size;
    segmento->stream = malloc(size);
    memcpy(segmento->stream, elementoParaAgregar, size);
    return;
    // Estructura dinamica paquete (size, stream, sig). Stream es un puntero a donde se guarde cualquier dato, size el tamaño de ese dato y siguiente_segmento el puntero a la siguiente parte del paquete
}

int paqueteSizeEntero(paquete * pac){
    if(pac == NULL)
    return 0; 
    tam = 0;
    while(pac->siguiente_segmento != NULL){
        tam += pac->size;
        pac = pac->siguiente_segmento;
    }
    tam += pac->size;
    return tam;
}

int paqueteAStream(paquete * pac, void * stream){
    if (pac == NULL){
        stream = NULL;   
        return 0;
    }
    int tam = paqueteSizeEntero(pac);
    stream = malloc(tam);
    int offset = 0;
    while (pac->siguiente_segmento != NULL){
        agregarAStream(stream, &offset, pac->stream, pac->size);
        pac = pac->siguiente_segmento;
    }
    agregarAStream(stream, &offset, pac->stream, pac->size);
    return tam;
}
void eliminarPaquete(paquete * pac){
    paquete * temp;
    while(pac->siguiente_segmento != NULL){
        temp = pac;
        pac = pac->siguiente_segmento;
        free(temp->stream);
        free(temp);
    }
    free(pac -> stream);
    free(pac);
}


void enviar_paquete(int socket, paquete * pac)// Envia el paquete en formato int, stream de tamaño int, int, stream de tamaño int, siendo stream cualquier cosa. 
{
    void * stream ;
    int sizeStream = paqueteAStream(pac, stream);
    enviarStream(socket, stream, sizeStream); 
    free(stream);
}
