#include <stdio.h>
#include "include/algo.h"
#include "include/ejemplo.h"

int main() {
    printf("🚀 --- INICIO DE SIMULACION MODULAR --- 🚀\n");
    
    // 1. Usamos el primer módulo
    saludar_terminal("Camion_Principal");
    
    printf("\n--- Intercalando modulos ---\n");
    
    // 2. Usamos el segundo módulo
    asignar_muelle(101);
    
    printf("\n🏁 --- FIN DE LA PRUEBA --- 🏁\n");
    return 0;
}