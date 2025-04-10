#include "socketUtils.h"


void say(char msg[]){
    printf("%s",msg);
}

int crearSocket(char* ip, char* puerto, struct addrinfo *servinfo){
    int soc;
    struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);
    soc = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    setsockopt(soc, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
    return soc;
}

int crearSocketServer(char* puerto){
    int soc;
    struct addrinfo * serverInfo;
    soc = crearSocket(NULL, puerto, serverInfo);
    bind(soc, serverInfo->ai_addr, serverInfo->ai_addrlen);
	listen(soc, SOMAXCONN);
    freeaddrinfo(serverInfo);
    return soc;
}

int conectarSocketClient(char* ip, char* puerto){
    int soc;
    struct addrinfo * serverInfo;
    soc = crearSocket(ip, puerto, serverInfo);
    connect(soc, serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo);
    return soc;
}
