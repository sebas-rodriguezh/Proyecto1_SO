#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H
 
#include <pthread.h>
#include "parametros_camion.h"

#define MAX_COLA 20

typedef enum {
    FIFO = 0, 
    RR = 1
} tipoAlgoritmo;

typedef struct Planificador
{
    tipoAlgoritmo tipo; 
    int quantum;
    ParametrosCamion *cola[MAX_COLA];
    int cantidad;
    pthread_mutex_t mutex_cola; 
    pthread_cond_t cond_turno; //Señal para revisar si es el turno. 
    ParametrosCamion *camion_activo; //Quién tiene permiso ahora. 

    void (*encolar)(struct Planificador *p, ParametrosCamion *c);
    ParametrosCamion *(*seleccionar)(struct Planificador *p);
    void (*post_ejecucion)(struct Planificador *p, ParametrosCamion *c);

} Planificador;

void inicializar_planificador(Planificador *p, tipoAlgoritmo tipo, int quantum);
void destruir_planificador(Planificador *p);


#endif
