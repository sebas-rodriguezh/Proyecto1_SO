#ifndef METRICAS_H
#define METRICAS_H

#include <pthread.h>

#define MAX_CAMIONES 30

/*

Notas: 
Acumula los tiempos de cada camión al finalizar su ejecución.
Usa un mutex propio para que múltiples hilos puedan registrar sus resultados de forma segura sin condición de carrera.

Fórmulas:
tiempo_espera = tiempoInicio - tiempoLlegada
tiempo_retorno = tiempoFin - tiempoLlegada

*/

typedef struct
{
    pthread_mutex_t mutex; /* Mutex para que los camiones no corrompan los arreglos y los resultados */

    long tiempos_espera[MAX_CAMIONES];
    long tiempos_retorno[MAX_CAMIONES];
    int cantidad; 
} GestorMetricas;

void inicializar_metricas(GestorMetricas *gestor);
void registrar_metricas(GestorMetricas *gestor, long tiempo_espera, long tiempo_retorno);
void calcular_promedios(GestorMetricas *gestor);
void destruir_metricas(GestorMetricas *gestor);

#endif