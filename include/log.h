#ifndef LOG_H
#define LOG_H
 
#include <pthread.h>
#include <stdio.h>

/* Notas: */
/* Struct que representa el registro global de eventos (Los camiones van a hacer funciones de escritura para esta Struct). */
/* Todos los hilos comparten una sola instancia via puntero. */
/* El mutex garantiza que solo un hilo escribe a la vez, evitando que los mensajes se mezclen. */

typedef struct
{
    pthread_mutex_t mutex; 
    FILE *archivo;
    
} Log;

/* Estos serían como los "métodos" de la Struct Log. */

void inicializar_log(Log *log, const char *ruta_archivo); 
void escribir_log(Log *log, const char *mensaje);
void destruir_log(Log *log);

/* Qué hace cada método:
 * - inicializar_log: Abre el archivo de log y inicializa el mutex. Debe ser llamado antes de usar el log.
 * - escribir_log: Escribe un mensaje (con timestamp) en el archivo de log. Usa el mutex para internamente.  
 * - destruir_log: Cierra el archivo de log y destruye el mutex. Debe ser llamado al finalizar el uso del log para liberar recursos.
 */

#endif
