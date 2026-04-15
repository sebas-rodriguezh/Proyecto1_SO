#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/terminal.h"

/*
 * main
 * ----
 * Punto de entrada del programa.
 *
 * Uso:
 *   ./terminal fifo <num_camiones> <num_muelles>
 *   ./terminal rr   <num_camiones> <num_muelles> <quantum>
 *
 * Ejemplos:
 *   ./terminal fifo 5 3
 *   ./terminal rr   6 3 2
 *
 * Si no se pasan argumentos, usa valores por defecto.
 */
int main(int argc, char *argv[])
{
    /* Valores por defecto */
    char         *algoritmo    = "fifo";
    int           num_camiones = 5;
    int           num_muelles  = 3;
    int           quantum      = 2;
    tipoAlgoritmo tipo         = FIFO;

    /* Leer argumentos si se pasaron */
    if (argc >= 2)
    {
        algoritmo = argv[1];
    }

    if (argc >= 3)
    {
        num_camiones = atoi(argv[2]);
    }

    if (argc >= 4)
    {
        num_muelles = atoi(argv[3]);
    }

    if (argc >= 5)
    {
        quantum = atoi(argv[4]);
    }

    /* Validaciones básicas */
    if (num_camiones <= 0 || num_camiones > MAX_CAMIONES)
    {
        fprintf(stderr, "Error: num_camiones debe estar entre 1 y %d\n", MAX_CAMIONES);
        return 1;
    }

    if (num_muelles <= 0)
    {
        fprintf(stderr, "Error: num_muelles debe ser mayor que 0\n");
        return 1;
    }

    if (quantum <= 0)
    {
        fprintf(stderr, "Error: quantum debe ser mayor que 0\n");
        return 1;
    }

    /* Determinar tipo de algoritmo */
    if (strcmp(algoritmo, "rr") == 0)
    {
        tipo = RR;
    }
    else if (strcmp(algoritmo, "fifo") == 0)
    {
        tipo = FIFO;
    }
    else
    {
        fprintf(stderr, "Error: algoritmo no reconocido '%s'. Use 'fifo' o 'rr'\n", algoritmo);
        return 1;
    }

    /* Imprimir configuración */
    printf("\n=================================\n");
    printf("   TERMINAL DE CARGA - SIMULADOR\n");
    printf("=================================\n");
    printf("Algoritmo : %s\n", algoritmo);
    printf("Camiones  : %d\n", num_camiones);
    printf("Muelles   : %d\n", num_muelles);

    if (tipo == RR)
    {
        printf("Quantum   : %d unidades\n", quantum);
    }

    printf("=================================\n\n");

    /* Crear, ejecutar y destruir la terminal */
    Terminal terminal;

    inicializar_terminal(&terminal, num_camiones, num_muelles, tipo, quantum);
    ejecutar_terminal(&terminal);
    calcular_promedios(&terminal.metricas);
    destruir_terminal(&terminal);

    return 0;
}