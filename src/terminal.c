#define _POSIX_C_SOURCE 199309L
#include "../include/terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


void inicializar_terminal(Terminal *t, int num_camiones, int num_muelles, tipoAlgoritmo tipo, int quantum)
{
    t->num_camiones = num_camiones;

    inicializar_log(&t->log, NULL);
    inicializar_metricas(&t->metricas);
    inicializar_recursos(&t->recursos, num_muelles);
    inicializar_planificador(&t->planificador, tipo, quantum);
}


void ejecutar_terminal(Terminal *t)
{
    int i;
    struct timespec ts;

    srand((unsigned int) time(NULL));

    for (i = 0; i < t->num_camiones; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t->params[i].tiempo_llegada  = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

        t->params[i].id = i;
        t->params[i].prioridad = rand() % 3 + 1;
        t->params[i].burst_total = rand() % 5 + 2;
        t->params[i].tiempo_restante = t->params[i].burst_total;
        t->params[i].tiempo_inicio = 0;
        t->params[i].tiempo_fin = 0;

        t->params[i].recursos = &t->recursos;
        t->params[i].log = &t->log;
        t->params[i].metricas = &t->metricas;
        t->params[i].planificador = (struct Planificador *) &t->planificador;

        t->camiones[i].params = &t->params[i];
        t->camiones[i].estado = NUEVO;

        if (pthread_create(&t->camiones[i].thread_id, NULL, ejecutar_camion, &t->params[i]) != 0)
        {
            fprintf(stderr, "Error al crear hilo para camion %d\n", i);
        }
        struct timespec espera = {0, 10000000}; nanosleep(&espera, NULL); 
    }

    for (i = 0; i < t->num_camiones; i++)
    {
        pthread_join(t->camiones[i].thread_id, NULL);
    }
}

void destruir_terminal(Terminal *t)
{
    destruir_planificador(&t->planificador);
    destruir_recursos(&t->recursos);
    destruir_metricas(&t->metricas);
    destruir_log(&t->log);
}