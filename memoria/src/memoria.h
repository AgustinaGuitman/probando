#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <utils/utils.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h> 
#include "pthread.h"
#include "semaphore.h"
#include <math.h>
#include <commons/collections/queue.h>

#define PUERTO_MEMORIA "8002"

t_log* logger_memoria;
t_config* config_memoria;
t_pcb* proceso_exec;
sem_t planificadores;

int atenderCpu(int server_memoria);
int atenderInterfaz(int server_memoria);
int atenderKernel(int server_memoria);

int server_memoria;
int fd_cpu;
int fd_kernel;
int fd_entradasalida;

#endif /* MEMORIA_H_ */