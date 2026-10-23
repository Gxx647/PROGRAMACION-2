#include <iostream>
#include "estructura.h"
#include "logica.h"
#include "presentacion.h"

using namespace std;

int main() {
    system("chcp 65001 > nul"); // Soporte UTF-8 para ñ y acentos

    SistemaTaller sistema;
    Taller datosTaller = {"Taller Central Uru", "J-12345678-9", "Maracaibo", "0414-1234567"};

    inicializarSistema(sistema, datosTaller);
    cout << "¡Sistema de Taller Mecanico inicializado correctamente!\n";

    int opcion = 0;

    do {
        cout << "\n    TALLER MECANICO    \n";
        cout << "1. Registrar Vehiculo\n";
        cout << "2. Mostrar Vehiculos\n";
        cout << "3. Registrar Mecanico\n";
        cout << "4. Registrar Repuesto\n";
        cout << "5. Mostrar Repuestos\n";
        cout << "6. Crear Orden de Servicio\n";
        cout << "7. Agregar Repuesto a Orden\n";
        cout << "8. Cerrar y Facturar Orden\n";
        cout << "9. Mostrar Ordenes de Servicio\n";
        cout << "10. Salir y liberar memoria\n";
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n[ERROR]: Entrada invalida. Ingrese un numero del menu.\n";
            continue;
        }

        cin.clear();
        cin.ignore(10000, '\n');

        switch (opcion) {
            case 1:
                menuRegistrarVehiculo(sistema);
                break;
            case 2:
                menuMostrarVehiculos(sistema);
                break;
            case 3:
                menuRegistrarMecanico(sistema);
                break;
            case 4:
                menuRegistrarRepuesto(sistema);
                break;
            case 5:
                menuMostrarRepuestos(sistema);
                break;
            case 6:
                menuCrearOrden(sistema);
                break;
            case 7:
                menuAgregarRepuesto(sistema);
                break;
            case 8:
                menuCerrarOrden(sistema);
                break;
            case 9:
                menuMostrarOrdenes(sistema);
                break;
            case 10:
                liberarSistema(sistema);
                cout << "\n¡Memoria liberada con exito. Saliendo del programa!\n";
                break;
            default:
                cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 10);

    return 0;
}