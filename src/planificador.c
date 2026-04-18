#include "../include/planificador.h"
#include <string.h>
#include <stdio.h>


void inicializar_planificador_fifo (Planificador *p);
void inicializar_planificador_rr (Planificador *p, int quantum);

void inicializar_planificador(Planificador *p, tipoAlgoritmo tipo, int quantum)
{
    for (int i = 0; i < MAX_COLA; i++)
    {
        p->cola[i] = NULL;
    }
    p->cantidad = 0;
    p->camion_activo = NULL;

    pthread_mutex_init(&p->mutex_cola, NULL);
    pthread_cond_init(&p->cond_turno, NULL);

    if (tipo == FIFO)
    {
        inicializar_planificador_fifo(p);
        printf("Planificador iniciado: FIFO\n");
    }
    else if (tipo == RR)
    {
        inicializar_planificador_rr(p, quantum);
        printf("Planificador iniciado: Round Robin (quantum=%d)\n", quantum);
    }
}

void destruir_planificador (Planificador *p)
{
    pthread_mutex_destroy(&p->mutex_cola);
    pthread_cond_destroy(&p->cond_turno);
}
