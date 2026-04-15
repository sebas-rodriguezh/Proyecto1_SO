#ifndef TERMINAL_H
#define TERMINAL_H

#include "camion.h"
#include "planificador.h"
#include "recursos.h"
#include "log.h"
#include "metricas.h"


/*
 * Terminal
 * --------
 * Struct central que contiene todos los recursos del simulador.
 * main crea una sola instancia de Terminal, la inicializa y la ejecuta.
 *
 * camiones[]  → arreglo de structs Camion (thread_id + estado)
 * params[]    → arreglo de ParametrosCamion (los datos que recibe cada hilo)
 *               Son arreglos separados porque params debe vivir en memoria
 *               estable mientras los hilos corren.
 */
typedef struct
{
    Camion           camiones[MAX_CAMIONES];
    ParametrosCamion params[MAX_CAMIONES];
    int              num_camiones;

    Planificador       planificador;
    RecursosCompartidos recursos;
    Log                log;
    GestorMetricas     metricas;

} Terminal;

/*
 * inicializar_terminal
 * --------------------
 * Prepara todos los recursos: log, metricas, recursos y planificador.
 * No crea hilos todavía.
 */
void inicializar_terminal(Terminal *t, int num_camiones, int num_muelles,
                          tipoAlgoritmo tipo, int quantum);

/*
 * ejecutar_terminal
 * -----------------
 * Genera datos aleatorios de cada camión, lanza los hilos con
 * pthread_create y espera que todos terminen con pthread_join.
 */
void ejecutar_terminal(Terminal *t);

/*
 * destruir_terminal
 * -----------------
 * Libera todos los recursos en orden inverso a la inicialización.
 */
void destruir_terminal(Terminal *t);

#endif