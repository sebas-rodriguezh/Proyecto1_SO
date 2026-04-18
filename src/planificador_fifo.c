#include "../include/planificador.h"
#include <stdio.h>


static void fifo_encolar(Planificador *p, ParametrosCamion *c)
{
    if (p->cantidad < MAX_COLA)
    {
        p->cola[p->cantidad] = c;
        p->cantidad++;

        if (p->camion_activo == NULL)
        {
            p->camion_activo = p->cola[0];
            pthread_cond_broadcast(&p->cond_turno);
        }
    }
    else
    {
        fprintf(stderr, "FIFO: cola llena, camion %d no encolado.\n", c->id);
    }
}

static ParametrosCamion *fifo_seleccionar(Planificador *p)
{
    if (p->cantidad == 0)
    {
        return NULL;
    }

    ParametrosCamion *seleccionado = p->cola[0];

    for (int i = 0; i < p->cantidad-1; i++)
    {
       p->cola[i] = p->cola[i+1];
    }

    p->cola[p->cantidad-1] = NULL;
    p->cantidad--;

    if (p->cantidad > 0)
    {
        p->camion_activo = p->cola[0];
    }
    else 
    {
        p->camion_activo = NULL;
    }

    return seleccionado;
}

static void fifo_post_ejecucion(Planificador *p, ParametrosCamion *c)
{
    (void) c; 
    pthread_cond_broadcast(&p->cond_turno);
}

void inicializar_planificador_fifo (Planificador *p)
{
    p->tipo = FIFO;
    p->quantum = 0; 
    p->encolar = fifo_encolar;
    p->seleccionar = fifo_seleccionar;
    p->post_ejecucion = fifo_post_ejecucion;
}