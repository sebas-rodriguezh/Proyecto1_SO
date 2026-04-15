#ifndef RECURSOS_H
#define RECURSOS_H
 
#include <semaphore.h>

/*

NOTAS: 
Recursos compartidos entre los hilos (camiones) y el planificador: Controla el acceso a los muelles de carga
Encapsula el semáforo que controla el acceso a los muelles.
Se inicializa con la cantidad de muelles disponibles (ej. 3).

sem_wait: Toma un muelle (decrementa el semáforo).
sem_post: Libera un muelle (incrementa el semáforo).

*/

typedef struct
{
    sem_t semaforo_muelles; 
    int num_muelles;

} RecursosCompartidos;

void inicializar_recursos(RecursosCompartidos *recursos, int num_muelles);
void tomar_muelle(RecursosCompartidos *recursos);
void liberar_muelle(RecursosCompartidos *recursos);
void destruir_recursos(RecursosCompartidos *recursos);


#endif

