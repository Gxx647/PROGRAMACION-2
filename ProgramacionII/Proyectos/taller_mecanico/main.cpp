#include <iostream>
#include "estructura.h"
#include "logica.h"

int main() {
    SistemaTaller s;
    Taller datosIniciales = {"Taller Central Uru", "Maracaibo"}; // Datos de ejemplo para el taller

    
    inicializarSistema(s, datosIniciales);

    int opcion = 0;

    do {
         std::cout << "  TALLER MECANICO \n";
       
        std::cout << "1. Registrar Vehiculo\n";
        std::cout << "2. Mostrar Vehiculos\n";
        std::cout << "3. Salir y liberar memoria\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                registrarVehiculo(s);
                break;
            case 2:
                mostrarVehiculos(s);
                break;
            case 3:
                
                liberarSistema(s);
                std::cout << "\n¡Saliendo del sistema y liberando memoria RAM!\n";
                break;
            default:
                std::cout << "\nOpción invalida. Intente de nuevo.\n";
        }

    } while (opcion != 3);

    return 0;
}