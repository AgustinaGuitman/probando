#include "utils.h"

/*--------------------------------UTILS CLIENTE-----------------------------------------*/

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

/*int crear_conexion(char *ip, char* puerto) {
    struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,
            					server_info->ai_socktype,
								server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
	{
		printf("Error en la conexión");
		socket_cliente = -1;
	}

	freeaddrinfo(server_info); 
	return socket_cliente;
	} */

int crear_conexion(char *ip, char* puerto) {
        struct addrinfo hints;
	struct addrinfo *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

getaddrinfo(ip, puerto, &hints, &server_info);
	
  /*  POR AHORA EL CHEQUEO DE ERRORES NO LO HACE
  if (getaddrinfo(ip, puerto, &hints, &server_info) != 0) {
        perror("Error en getaddrinfo");
        //return -1;
		exit(-1);
    }
*/
	
    int socket_cliente = socket(server_info->ai_family,
				server_info->ai_socktype,
				server_info->ai_protocol);
	
   /*  if (socket_cliente == -1) {
        perror("Error al crear socket");
        freeaddrinfo(server_info);
        //return -1;
		exit(-1);
    }*/
	//printf("El puerto es: %d: y la ip: %d\n", puerto,ip);
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)
		
    /*if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        perror("Error en connect");
        close(socket_cliente);
        freeaddrinfo(server_info);
        //return -1;
		exit(-1);
    }*/

    freeaddrinfo(server_info);
    return socket_cliente;
}


void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


/*--------------------------------UTILS SERVIDOR----------------------------------------------*/

t_log* logger;

int iniciar_servidor(char* puerto, t_log* un_log, char *msj_server)
{
	int socket_servidor;
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &server_info);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(server_info->ai_family,
				 server_info->ai_socktype,
				 server_info->ai_protocol);

	// Asociamos el socket a un puerto
	bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(server_info);
	log_info(un_log, "SERVER: %s",msj_server);

	return socket_servidor;
}


/*int iniciar_servidor(char* puerto)
{
	int socket_servidor;

	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &server_info);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(server_info->ai_family,
							server_info->ai_socktype,
							server_info->ai_protocol);

							

	// Asociamos el socket a un puerto
	bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(server_info);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}*/

/*int iniciar_servidor(char* puerto) {
    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, puerto, &hints, &server_info) != 0) {
        perror("Error en getaddrinfo");
        return -1;
    }

    int socket_servidor = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (socket_servidor == -1) {
        perror("Error al crear socket del servidor");
        freeaddrinfo(server_info);
        //return -1;
		exit(-1);
    }

    if (bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        perror("Error en bind");
        close(socket_servidor);
        freeaddrinfo(server_info);
        //return -1;
		exit(-1);
    }

    if (listen(socket_servidor, SOMAXCONN) == -1) {
        perror("Error en listen");
        close(socket_servidor);
        freeaddrinfo(server_info);
        //return -1;
		exit(-1);
    }

    //freeaddrinfo(server_info);
    return socket_servidor;
}*/

/* VIEJA
int esperar_cliente(int socket_servidor)
{
	uint32_t handshake;
	uint32_t resultOk = 0;
	uint32_t resultError = -1;
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	recv(socket_cliente, &handshake, sizeof(uint32_t), MSG_WAITALL);
	if(handshake ==1){
		send(socket_cliente, &resultOk, sizeof(uint32_t), 0);}
	else{
		send(socket_cliente, &resultError, sizeof(uint32_t), 0); // cambiar NULL por 0
	}
	//log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}*/

//Prueba
/*int esperar_cliente(int socket_servidor)
{
	uint32_t handshake;
	uint32_t resultOk = 0;
	uint32_t resultError = -1;
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	size_t bytes;


	bytes = recv(socket_cliente, &handshake, sizeof(int32_t), MSG_WAITALL);
	if (handshake == 1) {
		bytes = send(socket_cliente, &resultOk, sizeof(int32_t), 0);
		
	} else {
		bytes = send(socket_cliente, &resultError, sizeof(int32_t), 0);
	}

	return socket_cliente;
}*/
// Segunda prueba (?)
int esperar_cliente(int socket_servidor, t_log* un_log, char* msj) {

    int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(un_log,"Se conecto el cliente: %s",msj);
	
    if (socket_cliente == -1) {
        perror("Error al aceptar conexión del cliente");
        return -1;
    }

    uint32_t handshake;
    uint32_t resultOk = 0;
    uint32_t resultError = -1;
    size_t bytes = recv(socket_cliente, &handshake, sizeof(int32_t), MSG_WAITALL);

    if (bytes == -1) {
        perror("Error al recibir handshake del cliente");
        close(socket_cliente);
        return -1;
    }

    if (handshake == 1) {
        bytes = send(socket_cliente, &resultOk, sizeof(int32_t), 0);
        if (bytes == -1) {
            perror("Error al enviar handshake de respuesta al cliente");
            close(socket_cliente);
            return -1;
        }
    } else {
        bytes = send(socket_cliente, &resultError, sizeof(int32_t), 0);
        if (bytes == -1) {
            perror("Error al enviar handshake de error al cliente");
            close(socket_cliente);
            return -1;
        }
    }

    return socket_cliente;
}



/*
USA ESTA DE RECIBIR OPERACION, preguntar por que estamos usando la otra
int recibir_operacion(int socket_cliente)
{
	//enum op_code cod_op;
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) {
		printf("codigo de operacion %d", cod_op);
		return 0;
	}else
	{
		close(socket_cliente);
		return -1;
	}
} */
int recibir_operacion(int socket_cliente) {
    int cod_op;
    int bytes_recibidos = 0;
    
    while (bytes_recibidos < sizeof(int)) {
        int bytes_actual = recv(socket_cliente, ((char*)&cod_op) + bytes_recibidos, sizeof(int) - bytes_recibidos, 0);
		printf("Cod op en recibir op: %d\n",cod_op);
        if (bytes_actual <= 0) {
            if (bytes_actual == 0) {
                fprintf(stderr, "El cliente se desconectó inesperadamente\n");
            } else {
                perror("Error al recibir código de operación");
            }
            close(socket_cliente);
            return -1;
        }
        bytes_recibidos += bytes_actual;
    }

    printf("Código de operación recibido: %d\n", cod_op);
    return cod_op;
}


void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje: %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

t_log* iniciar_logger(char* nombreLog, char* proceso){

	t_log* nuevo_logger= log_create(nombreLog,proceso,1,LOG_LEVEL_INFO);

	if(nuevo_logger ==NULL){
		 printf("No pude crear el logger\n");}
		 // si se debe meter en la funcion agrego exit(1);

	return nuevo_logger;
}

t_config* iniciar_config(char* archivo){

	t_config* nuevo_config= config_create(archivo);

	if(nuevo_config ==NULL){
		printf("No pude leer la config \n");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
    log_info(logger, ">> %s", leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while(strcmp(leido, "") != 0){
		free(leido);
		leido = readline("> ");
		log_info(logger, ">> %s", leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
    free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(strcmp(leido, "") != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);

	//ENVIAR PAQUETE
	enviar_paquete(paquete, conexion);

	//ELIMINAR PAQUETE
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
    log_destroy(logger);
    config_destroy(config);
	liberar_conexion(conexion);
	//liberar_conexion(conexion_memoria);
	//liberar_conexion(conexion_interrupt);
}
