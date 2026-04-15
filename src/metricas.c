#include "../include/metricas.h"
#include <stdio.h>
#include <string.h>

void inicializar_metricas(GestorMetricas *gestor)
{
    pthread_mutex_init(&gestor->mutex, NULL);

    for (int i = 0; i < MAX_CAMIONES; i++)
    {
        gestor->tiempos_espera[i] = 0;
        gestor->tiempos_retorno[i] = 0;
    }
    
    gestor->cantidad = 0;
}

void registrar_metricas(GestorMetricas *gestor, int id_camion, long tiempo_espera, long tiempo_retorno)
{
    pthread_mutex_lock(&gestor->mutex);

    if (gestor->cantidad < MAX_CAMIONES)
    {
        gestor->tiempos_espera[id_camion] = tiempo_espera;
        gestor->tiempos_retorno[id_camion] = tiempo_retorno;
        gestor->cantidad++;
    }
    else
    {
        fprintf(stderr, "Error: Se ha alcanzado el límite de camiones para registrar métricas.\n");
    }

    pthread_mutex_unlock(&gestor->mutex);
}

/* Calcula los promedios de los tiempos de espera y retorno, los imprime y usa el ns como unidad de medida (Podría cambiar), e incluso su retorno podría no ser void. */
void calcular_promedios(GestorMetricas *gestor)
{
    if (gestor->cantidad == 0)
    {
        printf("No se han registrado métricas para calcular promedios.\n");
        return;
    }

    long suma_espera = 0;
    long suma_retorno = 0;

    
    printf("\n==============================\n");
    printf("   REGISTROS DE METRICAS\n");
    printf("==============================\n");

    for (int i = 0; i < gestor->cantidad; i++)
    {
        printf("Camion %d | Espera: %ld ms | Retorno: %ld ms\n", i, gestor->tiempos_espera[i], gestor->tiempos_retorno[i]);

        suma_espera += gestor->tiempos_espera[i];
        suma_retorno += gestor->tiempos_retorno[i];
    }


    printf("\n==============================\n");
    printf("   RESULTADOS DE PLANIFICACION\n");
    printf("==============================\n");
    printf("Camiones procesados: %d\n", gestor->cantidad);
    printf("Espera  promedio: %ld ms\n", suma_espera/gestor->cantidad);
    printf("Retorno promedio: %ld ms\n", suma_retorno/gestor->cantidad);
    printf("==============================\n\n");
}


void destruir_metricas(GestorMetricas *gestor)
{
    pthread_mutex_destroy(&gestor->mutex);
}
