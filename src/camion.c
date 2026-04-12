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


const char* estado_a_texto(Estado estado)
{
    switch (estado)
    {
        case NUEVO: return "NUEVO";
        case LISTO: return "LISTO";
        case EJECUCION: return "EJECUCION";
        case BLOQUEADO: return "BLOQUEADO";
        case TERMINADO: return "TERMINADO";
        default: return "DESCONOCIDO";
    }
}

/*

Ciclo de vida:
 1. Castea el argumento a ParametrosCamion*
 2. Registra estado LISTO y tiempo de llegada
 3. Llama tomar_muelle() -> puede bloquearse aquí (estado BLOQUEADO)
 4. Al obtener el muelle -> estado EJECUCION, registra tiempo_inicio
 5. Simula trabajo con sleep() por cada unidad de burst
 6. Registra tiempo_fin, libera el muelle
 7. Registra métricas, escribe en log → estado TERMINADO

*/

void *ejecutar_camion(void *arg)
{
    ParametrosCamion *p  = (ParametrosCamion *) arg;
    Planificador *pl = (Planificador *) p->planificador;
    char mensaje[160];
    int  i;
 
    // Nuevo. 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | NUEVO | prioridad=%d burst_total=%d", p->id, p->prioridad, p->burst_total);
    escribir_log(p->log, mensaje);
 
    pthread_mutex_lock(&pl->mutex_cola);
    pl->encolar(pl, p);
    pthread_mutex_unlock(&pl->mutex_cola);
 
    // listo.
    snprintf(mensaje, sizeof(mensaje), "Camion %d | LISTO | encolado, esperando turno.", p->id);
    escribir_log(p->log, mensaje);
 
    while (p->tiempo_restante > 0)
    {
        pthread_mutex_lock(&pl->mutex_cola);
 
        while (pl->camion_activo != p)
        {
            pthread_cond_wait(&pl->cond_turno, &pl->mutex_cola);
        }
 
        // Ya le toca. Sale a correr.
        pl->seleccionar(pl);
 
        // Suelta el mutex antes del semáforo. 
        pthread_mutex_unlock(&pl->mutex_cola);
 
        // Toma el muelle. Puede bloquearse aquí.
        snprintf(mensaje, sizeof(mensaje), "Camion %d | ESPERANDO | buscando muelle disponible.", p->id);
        escribir_log(p->log, mensaje);
 
        tomar_muelle(p->recursos);
 
        // Registra tiempo_inicio al obtener el muelle (si es la primera vez).
        if (p->tiempo_inicio == 0)
        {
            p->tiempo_inicio = tiempo_actual_ms();
        }
 
       //Calcula cuántas unidades de trabajo va a hacer en esta ejecución (quantum o lo que quede).
        int unidades;
        if (pl->tipo == FIFO)
        {
            unidades = p->tiempo_restante;
        }
        else
        {
            unidades = (p->tiempo_restante < pl->quantum) ? p->tiempo_restante : pl->quantum;
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
 
        //Libera muelle antes de tocar la cola. 
        liberar_muelle(p->recursos);
 

        //Post-Ejecución.
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
 
    long espera = p->tiempo_inicio - p->tiempo_llegada;
    long retorno = p->tiempo_fin - p->tiempo_llegada;
 
    registrar_metricas(p->metricas, espera, retorno);
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | TERMINADO | espera=%ld ms  retorno=%ld ms", p->id, espera, retorno);
    escribir_log(p->log, mensaje);
 
    pthread_exit(NULL);
}