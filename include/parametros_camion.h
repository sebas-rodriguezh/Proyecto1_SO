#ifndef PARAMETROS_CAMION_H
#define PARAMETROS_CAMION_H
 
#include "log.h"
#include "metricas.h"
#include "recursos.h"

struct Planificador;

typedef struct
{
    int id;
    int prioridad;
    int burst_total;
    int tiempo_restante;
    long tiempo_llegada;
    long tiempo_inicio;
    long tiempo_fin;

    // Todas son instancias compartidas. (Puntero al mismo objeto en la terminal).
    RecursosCompartidos *recursos; 
    Log *log; 
    GestorMetricas *metricas; 
    struct Planificador *planificador;
    
} ParametrosCamion;


#endif
