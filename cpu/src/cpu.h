#ifndef CPU_H_
#define CPU_H_

#include <utils/utils.h>
#include <utils/utils.h>
#include <utils/utils.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h> 
#include "pthread.h"
#include "semaphore.h"
#include <math.h>
#include <commons/collections/queue.h>

#define PUERTO_ESCUCHA_D "8006"
#define PUERTO_ESCUCHA_I "8007"


t_log* logger_cpu;
t_config* config_cpui;
t_config* config_cpud;
t_config* config_cpu;
t_pcb* proceso_exec;
sem_t planificadores;




int atenderCpuDispatch();
int atenderCpuInterrupt();
int conectarMemoria();
int atenderInterfaz();

int conexion_memoria;
int conexion_dispatch;
int conexion_interrupt;
int conexion_entradaSalida;
#endif /* CPU_H_ */
