#ifndef METRICAS_H
#define METRICAS_H

#include <pthread.h>

#define MAX_CAMIONES 20


typedef struct
{
    pthread_mutex_t mutex;

    long tiempos_espera[MAX_CAMIONES];
    long tiempos_retorno[MAX_CAMIONES];
    int cantidad; 
} GestorMetricas;

void inicializar_metricas(GestorMetricas *gestor);
void registrar_metricas(GestorMetricas *gestor, int id_camion, long tiempo_espera, long tiempo_retorno);
void calcular_promedios(GestorMetricas *gestor);
void destruir_metricas(GestorMetricas *gestor);

#endif