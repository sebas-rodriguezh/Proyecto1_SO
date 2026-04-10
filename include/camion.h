#ifndef CAMION_H
#define CAMION_H
 
#include <pthread.h>
#include "parametros_camion.h"

/*

NOTAS: 
Representa un hilo-camión del simulador.
thread_id es el identificador del hilo POSIX.
estado refleja en qué fase del ciclo de vida se encuentra.
params apunta a los datos del camión (compartidos con el hilo).

*/


typedef enum
{
    NUEVO, 
    LISTO, 
    EJECUCION,
    BLOQUEADO,
    TERMINADO
} Estado;

typedef struct
{
    pthread_t thread_id; 
    Estado estado; 
    ParametrosCamion *params; 

} Camion;

const char* estado_a_texto(Estado estado);

/*

Función principal del hilo. Recibe un ParametrosCamion* casteado a void*.
Contiene el cilo de vida completo. 
Es la función que se pasa a pthread_create.

*/
 

void *ejecutar_camion(void *arg);


#endif