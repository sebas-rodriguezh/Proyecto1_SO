#include "../include/planificador.h"
#include <stdio.h>


static void rr_encolar(Planificador *p, ParametrosCamion *c)
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
        fprintf(stderr, "RR: cola llena, camion %d no encolado.\n", c->id);
    }
}

static ParametrosCamion *rr_seleccionar(Planificador *p)
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

    p->camion_activo = (p->cantidad > 0) ? p->cola[0] : NULL;

    return seleccionado;
}


static void rr_post_ejecucion(Planificador *p, ParametrosCamion *c)
{
    if (c->tiempo_restante > 0)
    {
        if (p->cantidad < MAX_COLA)
        {
            p->cola[p->cantidad] = c;
            p->cantidad++;
        }
        p->camion_activo = p->cola[0];
    }
    pthread_cond_broadcast(&p->cond_turno);
}

void inicializar_planificador_rr (Planificador *p, int quantum)
{
    p->tipo = RR;
    p->quantum = quantum;
    p->encolar = rr_encolar;
    p->seleccionar = rr_seleccionar;
    p->post_ejecucion = rr_post_ejecucion;
}