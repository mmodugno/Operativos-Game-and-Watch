/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que deja
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{

*bytes = paquete->buffer->size + sizeof(op_code) + sizeof(int);

void* a_enviar = malloc(*bytes); //Preguntar!
int offset = 0; //desplazamiento

memcpy(a_enviar+offset, &(paquete->codigo_operacion),sizeof(op_code));
offset += sizeof(op_code);

memcpy(a_enviar+offset, &(paquete->buffer->size),sizeof(int));
offset += sizeof(int);

memcpy(a_enviar+offset, &(paquete->buffer->stream),paquete->buffer->size);

return a_enviar;

}


int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{

t_paquete* paquete = malloc(sizeof(t_paquete));
t_buffer* buffer = malloc(sizeof(t_buffer));

paquete->codigo_operacion = MENSAJE;

buffer->size = strlen(mensaje) + 1; //por fin de palabra
buffer->stream = mensaje;

paquete->buffer = buffer;

int tamanio_paquete = 0; //Para instanciar el TAMAÑO del paquete

void* a_enviar = serializar_paquete(paquete, &tamanio_paquete);

send(socket_cliente, a_enviar,tamanio_paquete,0);

//LIBERAR LOS PUNTEROS
}

//TODO

char* recibir_mensaje(int socket_cliente)
{
	char* mensaje;
	t_paquete* paquete = malloc(sizeof(paquete));

	recv(socket_cliente, &(paquete->codigo_operacion),sizeof(op_code),0);

	recv(socket_cliente, &(paquete->buffer->size),sizeof(int),0);

	recv(socket_cliente, &(paquete->buffer->stream),paquete->buffer->size,0);

	switch(paquete->codigo_operacion){
	case MENSAJE:
		mensaje = malloc(paquete->buffer->size);
		memcpy(mensaje,(paquete->buffer->stream),paquete->buffer->size);


	break;
	}
	free(paquete);
	return mensaje;

}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
