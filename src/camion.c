#define _POSIX_C_SOURCE 199309L
#include "../include/camion.h"
#include "../include/planificador.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

static long tiempo_actual_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}


void *ejecutar_camion(void *arg)
{
    ParametrosCamion *p  = (ParametrosCamion *) arg;
    Planificador *pl = (Planificador *) p->planificador;
    char mensaje[160];
    int  i;
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | NUEVO | prioridad=%d burst_total=%d", p->id, p->prioridad, p->burst_total);
    escribir_log(p->log, mensaje);
 
    pthread_mutex_lock(&pl->mutex_cola);
    pl->encolar(pl, p);
    pthread_mutex_unlock(&pl->mutex_cola);
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | LISTO | encolado, esperando turno.", p->id);
    escribir_log(p->log, mensaje);
 
    while (p->tiempo_restante > 0)
    {
        pthread_mutex_lock(&pl->mutex_cola);
 
        while (pl->camion_activo != p)
        {
            pthread_cond_wait(&pl->cond_turno, &pl->mutex_cola);
        }
        pthread_mutex_unlock(&pl->mutex_cola);

        snprintf(mensaje, sizeof(mensaje), "Camion %d | BLOQUEADO | buscando muelle disponible.", p->id);
        escribir_log(p->log, mensaje);
        tomar_muelle(p->recursos);

        pthread_mutex_lock(&pl->mutex_cola);

        pl->seleccionar(pl);

        pthread_mutex_unlock(&pl->mutex_cola);
 
 
        if (p->tiempo_inicio == 0)
        {
            p->tiempo_inicio = tiempo_actual_ms();
        }
 
        int unidades;
        if (pl->tipo == FIFO)
        {
            unidades = p->tiempo_restante;
        }
        else
        {
            if (p->tiempo_restante < pl->quantum)
            {
                unidades = p->tiempo_restante;
            }
            else 
            {
                unidades = pl->quantum;
            }
        }
 
        snprintf(mensaje, sizeof(mensaje), "Camion %d | EJECUCION | ejecutando %d unidad(es) (restante antes=%d)", p->id, unidades, p->tiempo_restante);
        escribir_log(p->log, mensaje);
 
        for (i = 0; i < unidades; i++)
        {
            sleep(1);
            p->tiempo_restante--;
 
            snprintf(mensaje, sizeof(mensaje), "Camion %d | EJECUCION | unidad completada (restante=%d)", p->id, p->tiempo_restante);
            escribir_log(p->log, mensaje);
        }
 
        liberar_muelle(p->recursos);
 
        //Acá hace FIFO y RR diferente. 
        pthread_mutex_lock(&pl->mutex_cola);
        pl->post_ejecucion(pl, p);
        pthread_mutex_unlock(&pl->mutex_cola);
 
        if (p->tiempo_restante > 0)
        {
            snprintf(mensaje, sizeof(mensaje), "Camion %d | LISTO | quantum agotado, vuelve a cola (restante=%d)", p->id, p->tiempo_restante);
            escribir_log(p->log, mensaje);
        }
    }
 
    // Terminado.
    p->tiempo_fin = tiempo_actual_ms();
 
    long espera = p->tiempo_inicio - p->tiempo_llegada; // Haciendo "cola" desde que llegó hasta que empezó a cargar. (Cuanto tiempo esperó para usar un muelle)
    long retorno = p->tiempo_fin - p->tiempo_llegada; // Tiempo TOTAL desde que llegó a la "cola" hasta que terminó de cargar. (Desde que llegó hasta que terminó)
 
    // FIFO: Malos tiempos de espera y buenos tiempos de retorno.
    // RR: Mejores tiempos de espera pero peores tiempos de retorno.

    registrar_metricas(p->metricas, p->id, espera, retorno);
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | TERMINADO | espera=%ld ms  retorno=%ld ms", p->id, espera, retorno);
    escribir_log(p->log, mensaje);
 
    pthread_exit(NULL);
}