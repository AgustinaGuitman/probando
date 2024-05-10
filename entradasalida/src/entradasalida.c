#include <stdlib.h>
#include <stdio.h>

#include "entradasalida.h"

int conexiones;
sem_t mlog;
pthread_mutex_t mconexiones;

int main(int argc, char* argv[]) {
    

    config_entradasalida = iniciar_config("clienteIO.config");
    logger_entradasalida = iniciar_logger("entradasalida.log","ENTRADASALIDA");

    conexiones=0;

    // Creacion de hilos

    pthread_t t1,t2,t3; //cambiar nombres a los hilos por algo como hilo_kernel, hilo_cpu, etc para mayot

    pthread_create(&t1, NULL, (void*) conectarMemoria, NULL);
	pthread_create(&t2, NULL, (void*) conectarKernel, NULL); //ojo, funciona por separado pero no junto
	pthread_create(&t3, NULL, (void*) leer_consola, NULL);

    pthread_join(t3, NULL); //aca habra que hacer join hasta el t3 si todo funcionara correctamente

    log_destroy(logger_entradasalida);
 	config_destroy(config_entradasalida);

    liberar_conexion(conexion_memoria);
	liberar_conexion(conexion_kernel);
	liberar_conexion(conexion_entradaSalida);


    return 0;
}

/*---------------------------------Conexion Memoria---------------------------------*/

    int conectarMemoria() {

    int conexion_memoria;
    char* ip;
    char* puerto_memoria;
    char* valor;

	ip= config_get_string_value(config_entradasalida,"IP_MEMORIA");
	puerto_memoria=config_get_string_value(config_entradasalida,"PUERTO_MEMORIA");
    valor = config_get_string_value(config_entradasalida, "CLAVE");

    // Loggeamos el valor de config

    log_info(logger_entradasalida, "Valor leido de la config: %s", valor);

    // Creamos una conexion hacia el servidor

    conexion_memoria = crear_conexion(ip, puerto_memoria);

    // Enviamos al servidor el valor de CLAVE como mensaje

    

    if (conexion_memoria <= 0){
        printf("No se pudo establecer una conexion con la Memoria\n");
    }
    else{
		printf("Conexion con Memoria exitosa");
    }

    //handshake_cliente(conexion_memoria, ENTRADASALIDA_MEMORIA);
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

     terminar_programa(conexion_memoria, logger, config_entradasalida);
    }



/*---------------------------------Conexion Kernel---------------------------------*/

int conectarKernel() {

    int conexion_kernel;
    char* ip;
    char* puerto_kernel;
    char* valor;

	ip= config_get_string_value(config_entradasalida,"IP_KERNEL");
	puerto_kernel=config_get_string_value(config_entradasalida,"PUERTO_KERNEL");
    valor = config_get_string_value(config_entradasalida, "CLAVE");

    // Loggeamos el valor de config

    log_info(logger_entradasalida, "Valor leido de la config: %s", valor);

    // Creamos una conexion hacia el servidor

    conexion_kernel = crear_conexion(ip, puerto_kernel);

    // Enviamos al servidor el valor de CLAVE como mensaje

    

    if (conexion_kernel <= 0){
        printf("No se pudo establecer una conexion con el Kernel\n");
    }
    else{
		printf("Conexion con Kernel exitosa");
    }

    //handshake_cliente(conexion_kernel, ENTRADASALIDA_KERNEL);

     // Empieza el handshake

    size_t bytes;

    int32_t handshake = 1;
    int32_t result;
    uint32_t resultOk=0;
    bytes = send(conexion_kernel, &handshake, sizeof(int32_t), 0);
    bytes = recv(conexion_kernel, &result, sizeof(int32_t), MSG_WAITALL);

    if (result == resultOk)
    {
        // Handshake OK
        printf("Conexion con KERNEL exitosa");
    }
    else
    {
        printf("ERROR: Handshake con KERNEL fallido");
    }
    // Fin de handshake
    enviar_mensaje(valor, conexion_kernel);

    paquete(conexion_kernel);

	pthread_mutex_lock(&mconexiones);
	conexiones++;
	pthread_mutex_unlock(&mconexiones);

     terminar_programa(conexion_kernel, logger, config_entradasalida);
    }
