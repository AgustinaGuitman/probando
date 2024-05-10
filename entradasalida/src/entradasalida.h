#ifndef ENTRADASALIDA_H_
#define ENTRADASALIDA_H_

#include <utils/utils.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h> 
#include "pthread.h"
#include "semaphore.h"
#include <math.h>
#include <commons/string.h>
#include <commons/collections/queue.h>

/*
  ¿¿hace falta poner el puerto si no es servidor??
  #define PUERTO "8003"
*/

t_log* logger_entradasalida;
t_config* config_entradasalida;
t_pcb* proceso_exec;
sem_t planificadores;




int conectarKernel();
int conectarMemoria();
int atenderInterfaz();

int conexion_memoria;
int conexion_dispatch;
int conexion_interrupt;
int conexion_entradaSalida;
int conexion_kernel;

#endif /*ENTRADASALIDA_H_ */
