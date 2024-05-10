#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include<commons/string.h>
#include<assert.h>
#include<signal.h>
#include<string.h>
#include <pthread.h>
#define PUERTO ""


typedef struct t_pcb 
{
int id;
int prioridad;
int program_counter;

t_list* recursos_asignados;
t_list* archivos_abiertos;

}t_pcb;

typedef enum
{
	MENSAJE,
	PAQUETE,
	KERNEL_DISPATCH,
	KERNEL_INTERRUPT,
	KERNEL_MEMORIA,
	ENTRADASALIDA_MEMORIA,
	ENTRADASALIDA_KERNEL,
	CPU_MEMORIA,
	KERNEL,
	CPU_DISPATCH,
	CPU_INTERRUPT
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

extern t_log* logger;

int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

void* recibir_buffer(int*, int);
//int iniciar_servidor(char*);
int iniciar_servidor(char* puerto, t_log *log, char *msj_server);
int esperar_cliente(int socket_servidor, t_log* un_log, char *msj);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

t_log* iniciar_logger(char* nombreLog, char* proceso);
t_config* iniciar_config(char* archivo);

//t_log* iniciar_logger(void);
//t_config* iniciar_config(void);
void leer_consola(t_log*);
void paquete(int);
void terminar_programa(int, t_log*, t_config*);


#endif
