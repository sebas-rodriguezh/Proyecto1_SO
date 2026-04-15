#define _POSIX_C_SOURCE 199309L
#include "../include/terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
 * inicializar_terminal
 * --------------------
 * Inicializa todos los recursos en orden.
 * El orden importa: log y metricas primero porque los camiones
 * los necesitan desde el momento en que nacen.
 */
void inicializar_terminal(Terminal *t, int num_camiones, int num_muelles,
                          tipoAlgoritmo tipo, int quantum)
{
    t->num_camiones = num_camiones;

    inicializar_log(&t->log, NULL);
    inicializar_metricas(&t->metricas);
    inicializar_recursos(&t->recursos, num_muelles);
    inicializar_planificador(&t->planificador, tipo, quantum);
}

/*
 * ejecutar_terminal
 * -----------------
 * Fase 1: genera datos y lanza un hilo por camión.
 * Fase 2: espera que todos los hilos terminen.
 *
 * Se usa srand con time(NULL) para que cada ejecución genere
 * datos distintos y la simulación no sea siempre igual.
 */
void ejecutar_terminal(Terminal *t)
{
    int i;
    struct timespec ts;

    srand((unsigned int) time(NULL));

    /* Fase 1: crear hilos */
    for (i = 0; i < t->num_camiones; i++)
    {
        /* Capturar tiempo de llegada antes de pthread_create */
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t->params[i].tiempo_llegada  = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

        /* Generar datos aleatorios del camión */
        t->params[i].id              = i;
        t->params[i].prioridad       = rand() % 3 + 1;
        t->params[i].burst_total     = rand() % 5 + 2;
        t->params[i].tiempo_restante = t->params[i].burst_total;
        t->params[i].tiempo_inicio   = 0;
        t->params[i].tiempo_fin      = 0;

        /* Asignar punteros compartidos — todos apuntan a las mismas instancias */
        t->params[i].recursos     = &t->recursos;
        t->params[i].log          = &t->log;
        t->params[i].metricas     = &t->metricas;
        t->params[i].planificador = (struct Planificador *) &t->planificador;

        /* Vincular el struct Camion con sus params */
        t->camiones[i].params = &t->params[i];
        t->camiones[i].estado = NUEVO;

        /* Lanzar el hilo */
        if (pthread_create(&t->camiones[i].thread_id, NULL,
                           ejecutar_camion, &t->params[i]) != 0)
        {
            fprintf(stderr, "Error al crear hilo para camion %d\n", i);
        }

        /*
         * Pequeño delay entre creaciones para que tiempo_llegada
         * sea distinto en cada camión y el orden de llegada tenga sentido.
         */
        struct timespec espera = {0, 10000000}; nanosleep(&espera, NULL); /* 10 ms */
    }

    /* Fase 2: esperar que todos los hilos terminen */
    for (i = 0; i < t->num_camiones; i++)
    {
        pthread_join(t->camiones[i].thread_id, NULL);
    }
}

/*
 * destruir_terminal
 * -----------------
 * Libera recursos en orden inverso a la inicialización.
 * Se llama después de pthread_join, cuando ningún hilo está corriendo.
 */
void destruir_terminal(Terminal *t)
{
    destruir_planificador(&t->planificador);
    destruir_recursos(&t->recursos);
    destruir_metricas(&t->metricas);
    destruir_log(&t->log);
}