#ifndef KERNEL_H_
#define KERNEL_H_

#include <utils/utils.h>
#include <utils/utils.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h> 
#include "pthread.h"
#include "semaphore.h"
#include <math.h>
#include <commons/collections/queue.h>


t_log* logger_kernel;
t_config* config_kernel;
t_pcb* proceso_exec;
sem_t planificadores;

int fd_kernel;
int fd_cpu;
int fd_entradasalida;
int fd_memoria;

char *ip;
char *puerto_memoria;

int conectarCpuDispatch();
int conectarCpuInterrupt();
int conectarMemoria();
int atenderEntradaSalida();

int conexion_memoria;
int conexion_dispatch;
int conexion_interrupt;
int conexion_entradaSalida;

#endif /*KERNEL_H_*/