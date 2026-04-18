#ifndef RECURSOS_H
#define RECURSOS_H
 
#include <semaphore.h>

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