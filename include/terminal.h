#ifndef TERMINAL_H
#define TERMINAL_H

#include "camion.h"
#include "planificador.h"
#include "recursos.h"
#include "log.h"
#include "metricas.h"


typedef struct
{
    int num_camiones;

    Planificador planificador;
    RecursosCompartidos recursos;
    Log log;
    GestorMetricas metricas;

    Camion camiones[MAX_CAMIONES];
    ParametrosCamion params[MAX_CAMIONES];

} Terminal;


void inicializar_terminal(Terminal *t, int num_camiones, int num_muelles, tipoAlgoritmo tipo, int quantum);
void ejecutar_terminal(Terminal *t);
void destruir_terminal(Terminal *t);

#endif