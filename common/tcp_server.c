#include "head.h"

int socket_create(int port) {
	int server_listen;

	if ((server_listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return -1;
	}

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;


	if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
		return -1;
	}
	
    return server_listen;
}
