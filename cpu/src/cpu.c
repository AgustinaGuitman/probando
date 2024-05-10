#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"

int conexiones;
pthread_mutex_t mconexiones;

int main(int argc, char* argv[]) {
    


    config_cpu = iniciar_config("cpu.config");
    logger_cpu = iniciar_logger("cpu.log","CPU");

    conexiones=0;

    // Creacion de hilos

    pthread_t t1,t2,t3,t4;

	pthread_create(&t1, NULL, (void*) conectarMemoria, NULL);
	pthread_create(&t2, NULL, (void*) atenderCpuDispatch, NULL);
	//pthread_create(&t3, NULL, (void*) atenderCpuInterrupt, NULL);

	// pthread_create(&t4, NULL, (void*) leer_consola, NULL);

    pthread_join(t2, NULL);

    log_destroy(logger_cpu);
 	config_destroy(config_cpu);

	liberar_conexion(conexion_memoria);
	liberar_conexion(conexion_dispatch);
	liberar_conexion(conexion_interrupt);



    return 0;
}


/* ------------------------------Conexion KERNEL - CPU Dispatch-------------------------------------------*/

int atenderCpuDispatch() {
        logger = log_create("logServer.log", "SV_CPUDISPATCH", 1, LOG_LEVEL_DEBUG);

        char *puerto = config_get_string_value(config_cpu, "PUERTO_ESCUCHA_DISPATCH");

	    int server_cpud = iniciar_servidor(puerto);
	    log_info(logger, "Servidor listo para recibir al cliente");
	    int cliente_cpud = esperar_cliente(server_cpud);

	    t_list* lista;
	    while (1) {
		    int cod_op = recibir_operacion(server_cpud);
		    switch (cod_op) {
		    case MENSAJE:
			    recibir_mensaje(server_cpud);
			    break;
		    case PAQUETE:
			    lista = recibir_paquete(server_cpud);
			    log_info(logger, "Me llegaron los siguientes valores:\n");
			    break;
		    case -1:
			    log_error(logger, "el cliente se desconecto. Terminando servidor");
			    return EXIT_FAILURE;
		    default:
			    log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			    break;
		        }
	}
	return EXIT_SUCCESS;
    }

/* ------------------------------Conexion KERNEL - CPU Interrupt-------------------------------------------*/

int atenderCpuInterrupt() {
        logger = log_create("logServer.log", "SV_CPUINTERRUPT", 1, LOG_LEVEL_DEBUG);

        char *puerto = config_get_string_value(config_cpui, "PUERTO_ESCUCHA_INTERRUPT");

	    int server_cpui = iniciar_servidor(puerto);
	    log_info(logger, "Servidor listo para recibir al cliente");
	    int cliente_cpui = esperar_cliente(server_cpui);

	    t_list* lista;
	    while (1) {
		    int cod_op = recibir_operacion(server_cpui);
		    switch (cod_op) {
		    case MENSAJE:
			    recibir_mensaje(server_cpui);
			    break;
		    case PAQUETE:
			    lista = recibir_paquete(server_cpui);
			    log_info(logger, "Me llegaron los siguientes valores:\n");
			    break;
		    case -1:
			    log_error(logger, "el cliente se desconecto. Terminando servidor");
			    return EXIT_FAILURE;
		    default:
			    log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			    break;
		        }
	}
	return EXIT_SUCCESS;
    }

/* ------------------------------Conexion Memoria-------------------------------------------*/


int conectarMemoria() {

    int conexion_memoria;
    //int conexion_memoria;
    char* ip;
    char* puerto_memoria;
    char* valor;

	ip= config_get_string_value(config_cpu,"IP_MEMORIA");
	puerto_memoria=config_get_string_value(config_cpu,"PUERTO_MEMORIA");
	valor = config_get_string_value(config_cpu, "CLAVE");

    // Loggeamos el valor de config

    log_info(logger_cpu, "Valor leido de la config: %s", valor);

    // Creamos una conexion hacia el servidor

    conexion_memoria = crear_conexion(ip, puerto_memoria);

    // Enviamos al servidor el valor de CLAVE como mensaje

    

    if (conexion_memoria <= 0){
        printf("No se pudo establecer una conexion con la Memoria\n");
    }
    else{
		printf("Conexion con Memoria exitosa");
    }

    //handshake_cliente(conexion_memoria, CPU_MEMORIA);

	// Empieza el handshake

    size_t bytes;

    int32_t handshake = 1;
    int32_t result;
    uint32_t resultOk=0;
    bytes = send(conexion_memoria, &handshake, sizeof(int32_t), 0);
    bytes = recv(conexion_memoria, &result, sizeof(int32_t), MSG_WAITALL);

    if (result == resultOk)
    {
        // Handshake OK
        printf("Conexion con MEMORIA exitosa");
    }
    else
    {
        printf("ERROR: Handshake con MEMORIA fallido");
    }
    // Fin de handshake

enviar_mensaje(valor, conexion_memoria);
    paquete(conexion_memoria);

	pthread_mutex_lock(&mconexiones);
	conexiones++;
	pthread_mutex_unlock(&mconexiones);

    // terminar_programa(conexion_memoria, logger, config_cpu);
    }
