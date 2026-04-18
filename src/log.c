#include "../include/log.h"
#include <time.h>
#include <string.h>

void inicializar_log(Log *log, const char *ruta_archivo)
{
    pthread_mutex_init(&log->mutex, NULL); 
    
    if (ruta_archivo != NULL)
    {
        log->archivo = fopen(ruta_archivo, "w"); 

        if (log->archivo == NULL)
        {
            log->archivo = stdout; // Imprimir en salida, esta forma es la que vamos a usar. 
        }
    } 
    else 
    {
        log->archivo = stdout; // Siempre va a caer. 
    }
}


void escribir_log(Log *log, const char *mensaje)
{
    time_t ahora = time(NULL);
    struct tm info_tiempo;
    struct tm *t = localtime_r(&ahora, &info_tiempo);

    pthread_mutex_lock(&log->mutex);
    fprintf(log->archivo, "[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, mensaje);
    fflush(log->archivo);

    pthread_mutex_unlock(&log->mutex);
}


void destruir_log(Log *log)
{
    pthread_mutex_destroy(&log->mutex);

    if (log->archivo != NULL && log->archivo != stdout)
    {
        fclose(log->archivo);
    }
}