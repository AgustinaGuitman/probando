#include "memoria.h"

int conexiones;

int main(int argc, char *argv[1])
{
	
	config_memoria = iniciar_config("memoria.config");
	logger_memoria = iniciar_logger("memoria.log", "MEMORIA");

	//inicializar servidor de memoria

	server_memoria = iniciar_servidor(PUERTO_MEMORIA, logger_memoria, "Servidor Memoria inicializada");
	conexiones = 0;

	// Creacion de hilos

	pthread_t t1, t2, t3, t4;
	pthread_create(&t1, NULL, (void *)atenderKernel, server_memoria);
	pthread_create(&t2, NULL, (void*) atenderCpu, NULL);
	//pthread_create(&t3, NULL, (void*) atenderInterfaz, NULL);
	//pthread_create(&t4, NULL, (void*) leer_consola, NULL);

	pthread_join(t2, NULL);

	log_destroy(logger_memoria);
	config_destroy(config_memoria);

	liberar_conexion(atenderKernel);
	liberar_conexion(atenderCpu);
	liberar_conexion(atenderInterfaz);

	return 0;
}

/*----------------------------------Conexion Kernel-----------------------------------*/
int atenderKernel(int server_memoria)
{

	logger = log_create("logServerMemoria.log", "SV_MEMORIA", 1, LOG_LEVEL_DEBUG);
	
	char *puerto = config_get_string_value(config_memoria, "PUERTO_ESCUCHA");

	//int server_memoria = iniciar_servidor(puerto);
	//int server_memoria = iniciar_servidor(PUERTO_MEMORIA,logger, "Servidor memoria iniciada");
	log_info(logger, "Servidor listo para recibir al cliente");

	
	log_info(logger_memoria,"Esperando a Kernel");
	fd_kernel = esperar_cliente(server_memoria,logger_memoria,"Kernel");

	// t_list* lista;
	while (fd_kernel)
	{
		int cod_op = recibir_operacion(fd_kernel); // CUANDO FUNCIONE DESCOMENTAR
		switch (cod_op)
		{ // aca deberia ir cod_op pero devuelve valores random, puse 0 para probar si llega la "clave" del kernel.config
		case MENSAJE:
			recibir_mensaje(fd_kernel);
			break;
		case PAQUETE:
			// lista = recibir_paquete(cliente_memoria);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	close(fd_kernel);
	return EXIT_SUCCESS;
}

/*----------------------------------Conexion CPU-----------------------------------*/

int atenderCpu()
{/*
	//wait(30);
	logger = log_create("logServerMemoria.log", "SV_MEMORIA", 1, LOG_LEVEL_DEBUG);

	char *puerto = config_get_string_value(config_memoria, "PUERTO_ESCUCHA");

	int server_memoria = iniciar_servidor(puerto);
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_memoria = esperar_cliente(server_memoria);

	t_list *lista;
	while (1)
	{
		int cod_op = recibir_operacion(cliente_memoria);
		switch (cod_op)
		{
		case MENSAJE:
			recibir_mensaje(cliente_memoria);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_memoria);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;*/
}

/*----------------------------------Conexion Interfaz-----------------------------------*/

int atenderInterfaz()
{
/*
	logger = log_create("logServerMemoria.log", "SV_MEMORIA", 1, LOG_LEVEL_DEBUG);

	char *puerto = config_get_string_value(config_memoria, "PUERTO_ESCUCHA");

	int server_memoria = iniciar_servidor(puerto);
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_memoria = esperar_cliente(server_memoria);

	t_list *lista;
	while (1)
	{
		int cod_op = recibir_operacion(cliente_memoria);
		switch (cod_op)
		{
		case MENSAJE:
			recibir_mensaje(cliente_memoria);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_memoria);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;*/
}
