#ifndef PARAMETROS_CAMION_H
#define PARAMETROS_CAMION_H
 
#include "log.h"
#include "metricas.h"
#include "recursos.h"

/*
NOTAS: 
Struct de datos puros que describe a un camión.
Es el único argumento que recibe la función del hilo
al arrancar via pthread_create (como void*).

Datos generados al crear el camión (antes de pthread_create):
 id: índice del arreglo (0, 1, 2...)
 prioridad: rand() % 3 + 1  →  valores 1, 2 o 3
 burst_total: rand() % 5 + 2  →  entre 2 y 6 unidades. Cantidad total de trabajo (tiempo de carga o tiempo de CPU)
 tiempo_restante: empieza igual que burst_total
 tiempo_llegada: capturado con clock_gettime() antes del create
 
Datos registrados durante la ejecución del hilo:
 tiempo_inicio: cuando el hilo pasa el sem_wait (obtiene muelle)
 tiempo_fin: cuando el hilo termina su trabajo (antes del sem_post)
 
Punteros compartidos (todos los camiones apuntan a las mismas instancias):
 recursos: para tomar y liberar muelles
 log: para escribir eventos
 metrica: para registrar tiempos al terminar

*/

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

    RecursosCompartidos *recursos; 
    Log *log; 
    GestorMetricas *metricas; 
    struct Planificador  *planificador;
    
} ParametrosCamion;


#endif
