#include <stdio.h>
#include "../include/ejemplo.h"
#include "../include/algo.h" // Relación: usamos el otro módulo

void asignar_muelle(int id_camion) 
{
    printf("⚓ [MODULO MUELLES]: Buscando muelle disponible para el camion #%d...\n", id_camion);
    printf("✅ [MODULO MUELLES]: Muelle 1 asignado correctamente.\n");
    printf("Este es un cambio que se hizo antes de guardar en el Makefile.\n");

    
    // Llamamos a una función del otro módulo para demostrar la relación
    saludar_terminal("Camion_Desde_Muelle");
}