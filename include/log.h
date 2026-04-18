#ifndef LOG_H
#define LOG_H
 
#include <pthread.h>
#include <stdio.h>

typedef struct
{
    pthread_mutex_t mutex; 
    FILE *archivo;
    
} Log;


void inicializar_log(Log *log, const char *ruta_archivo); 
void escribir_log(Log *log, const char *mensaje);
void destruir_log(Log *log);

#endif