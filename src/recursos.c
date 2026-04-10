#include "../include/recursos.h"
#include <stdio.h>

void inicializar_recursos(RecursosCompartidos *recursos, int num_muelles) 
{
    recursos->num_muelles = num_muelles;
    sem_init(&recursos->semaforo_muelles, 0, num_muelles);
}


void tomar_muelle(RecursosCompartidos *recursos) 
{
    sem_wait(&recursos->semaforo_muelles);
}


void liberar_muelle(RecursosCompartidos *recursos) 
{
    sem_post(&recursos->semaforo_muelles);
}

void destruir_recursos(RecursosCompartidos *recursos) 
{
    sem_destroy(&recursos->semaforo_muelles);
}
