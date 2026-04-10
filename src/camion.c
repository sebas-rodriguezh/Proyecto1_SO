#define _POSIX_C_SOURCE 199309L
#include "../include/camion.h"
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
    ParametrosCamion *p = (ParametrosCamion *) arg;
    char mensaje[128];
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | LISTO | prioridad=%d burst=%d", p->id, p->prioridad, p->burst_total);
    escribir_log(p->log, mensaje);
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | ESPERANDO | esperando muelle disponible...", p->id);
    escribir_log(p->log, mensaje);
 
    tomar_muelle(p->recursos); /* Se bloquea si no hay muelles disponibles, pero queda esperando. */
 
    p->tiempo_inicio = tiempo_actual_ms();
 
    snprintf(mensaje, sizeof(mensaje), "Camion %d | EJECUCION | inicio en muelle (burst=%d)", p->id, p->burst_total);
    escribir_log(p->log, mensaje);
 
    /* Simular el trabajo en segundos. */

    for (int i = 0; i < p->burst_total; i++)
    {
        sleep(1);
        p->tiempo_restante--;
 
        snprintf(mensaje, sizeof(mensaje), "Camion %d | EJECUCION | unidad %d/%d completada (restante=%d)", p->id, i + 1, p->burst_total, p->tiempo_restante);
        escribir_log(p->log, mensaje);
    }
 
    p->tiempo_fin = tiempo_actual_ms();
 
    liberar_muelle(p->recursos);
 
    long espera = p->tiempo_inicio - p->tiempo_llegada;
    long retorno = p->tiempo_fin - p->tiempo_llegada;
 
    registrar_metricas(p->metricas, espera, retorno);
 
    snprintf(mensaje, sizeof(mensaje),"Camion %d | TERMINADO | espera=%ld ms  retorno=%ld ms", p->id, espera, retorno);
    escribir_log(p->log, mensaje);
 
    pthread_exit(NULL);
}