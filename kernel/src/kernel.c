#include "kernel.h"

int conexiones;
pthread_mutex_t mconexiones;

int main(int argc, char *argv[])
{
    config_kernel = iniciar_config("clienteKernel.config");
    logger_kernel = iniciar_logger("kernel.log", "KERNEL");
    
    conexiones=0;
    pthread_t t1, t2, t3, t4, t5;

    pthread_create(&t1, NULL, (void *)conectarMemoria, NULL); 
    //pthread_create(&t2, NULL, (void *)conectarCpuDispatch, NULL);
    //pthread_create(&t3, NULL, (void *)conectarCpuInterrupt, NULL);
    //pthread_create(&t4, NULL, (void *)atenderEntradaSalida, NULL);
    // pthread_create(&t5, NULL, (void*) leer_consola, NULL);

    pthread_join(t1, NULL);

    log_destroy(logger_kernel);
    config_destroy(config_kernel);
    liberar_conexion(conexion_memoria);
    liberar_conexion(conexion_dispatch);
    liberar_conexion(conexion_interrupt);
    liberar_conexion(conexion_entradaSalida);

    return 0;
}

int conectarCpuDispatch()
{

    int conexion_dispatch;
    // int conexion_memoria;
    char *ip;
    char *puerto_dispatch;
    char *valor;

    ip = config_get_string_value(config_kernel, "IP_CPU");
    puerto_dispatch = config_get_string_value(config_kernel, "PUERTO_ESCUCHA_DISPATCH");
    valor = config_get_string_value(config_kernel, "CLAVE");

    log_info(logger_kernel, "Valor leido de la config: %s", valor);

    // Creamos una conexion hacia el servidor (socket y connect)

    conexion_dispatch = crear_conexion(ip, puerto_dispatch);

    // Enviamos al servidor el valor de CLAVE como mensaje

    if (conexion_dispatch <= 0)
    {
        printf(" DISPATCH: No se pudo establecer una conexion con la CPU\n");
    }
    else
    {
        printf("DISPATCH: Conexion con CPU exitosa");
    }

    // Empieza el handshake

    size_t bytes;

    int32_t handshake = 1;
    int32_t result;

    bytes = send(conexion_dispatch, &handshake, sizeof(int32_t), 0);
    bytes = recv(conexion_dispatch, &result, sizeof(int32_t), MSG_WAITALL);

    if (result == 0)
    {
        // Handshake OK
        printf("Conexion con CPU-DISPATCH exitosa");
    }
    else
    {
        printf("ERROR: Handshake con CPU-DISPATCH fallido");
    }
    // Fin de handshake

    paquete(conexion_dispatch);

    pthread_mutex_lock(&mconexiones);
	conexiones++;
	pthread_mutex_unlock(&mconexiones);

    terminar_programa(conexion_dispatch, logger, config_kernel);
}

/*-----------------------------Conexion Cpu Interrupt---------------------------------*/

int conectarCpuInterrupt()
{

    int conexion_interrupt;
    // int conexion_memoria;
    char *ip;
    char *puerto_interrupt;
    char *valor;

    ip = config_get_string_value(config_kernel, "IP_CPU");
    puerto_interrupt = config_get_string_value(config_kernel, "PUERTO_ESCUCHA_INTERRUPT");
    valor = config_get_string_value(config_kernel, "CLAVE");

    // Loggeamos el valor de config

    log_info(logger_kernel, "Valor leido de la config: %s", valor);

    // Creamos una conexion hacia el servidor (socket y connect)

    conexion_interrupt = crear_conexion(ip, puerto_interrupt);

    if (conexion_interrupt <= 0)
    {
        printf(" INTERRUPT: No se pudo establecer una conexion con la CPU\n");
    }
    else
    {
        printf("INTERRUPT: Conexion con CPU exitosa");
    }

    

    // Empieza el handshake

    size_t bytes;

    int32_t handshake = 1;
    int32_t result;

    bytes = send(conexion_interrupt, &handshake, sizeof(int32_t), 0);
    bytes = recv(conexion_interrupt, &result, sizeof(int32_t), MSG_WAITALL);

    if (result == 0)
    {
        // Handshake OK
        printf("Conexion con CPU-INTERRUPT exitosa");
    }
    else
    {
        printf("ERROR: Handshake con CPU-INTERRUPT fallido");
    }
    // Fin de handshake

    // Enviamos al servidor el valor de CLAVE como mensaje

    enviar_mensaje(valor, conexion_interrupt);
    paquete(conexion_interrupt);

    pthread_mutex_lock(&mconexiones);
	conexiones++;
	pthread_mutex_unlock(&mconexiones);

    terminar_programa(conexion_interrupt, logger, config_kernel);

     return 0;
}

/*---------------------------------Conexion Memoria---------------------------------*/

int conectarMemoria()
{

    int conexion_memoria;
    char *ip;
    char *puerto_memoria;
    char *valor;

    ip = config_get_string_value(config_kernel, "IP");
    puerto_memoria = config_get_string_value(config_kernel, "PUERTO_MEMORIA");
    valor = config_get_string_value(config_kernel, "CLAVE");
    
    
    // prueba
    //printf(ip); // no reconocia ip pq estaba con otro nombre 
    //printf(puerto_memoria); // lo toma ok

    // Loggeamos el valor de config

    log_info(logger_kernel, "Valor leido de la config: %s", valor);

    // Creamos una conexion hacia el servidor (socket y connect)

    conexion_memoria = crear_conexion(ip, puerto_memoria);
    

    if (conexion_memoria <= 0)
    {
        log_info(logger_kernel, "No se pudo establecer una conexion con la Memoria\n");
    }
    else
    {
        log_info(logger_kernel, "Conexion con Memoria exitosa");
    }

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

    // Enviamos al servidor el valor de CLAVE como mensaje
    enviar_mensaje(valor, conexion_memoria);
    paquete(conexion_memoria);

    pthread_mutex_lock(&mconexiones);
	conexiones++;
	pthread_mutex_unlock(&mconexiones);

    terminar_programa(conexion_memoria, logger, config_kernel);

    return 0;
}

/*-----------------------------Conexion Entradasalida---------------------------------*/

int atenderEntradaSalida()
{
    logger = log_create("logServer.log", "SV_Kernel", 1, LOG_LEVEL_DEBUG);

    char *puerto = config_get_string_value(config_kernel, "PUERTO_ESCUCHA");

    int server_kernel = iniciar_servidor(puerto); // socket, bind y listen
    log_info(logger, "Servidor listo para recibir al cliente");
    int cliente_kernel = esperar_cliente(server_kernel);

    t_list *lista;

    while (1)
    {
        int cod_op = recibir_operacion(cliente_kernel);
        switch (cod_op)
        {
        case MENSAJE:
            recibir_mensaje(cliente_kernel);
            break;
        case PAQUETE:
            lista = recibir_paquete(cliente_kernel);
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
    return EXIT_SUCCESS;
}
