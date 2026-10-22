#include <iostream>
#include "estructura.h"
#include "logica.h"

using namespace std;

int main() {
    SistemaTaller sistema;
    Taller datosTaller = {"Taller Central Uru", "J-12345678-9", "Maracaibo", "0414-1234567"};

    inicializarSistema(sistema, datosTaller);
    cout << "¡Sistema de Taller Mecanico inicializado correctamente!\n";

    int opcion = 0;

    do {
        cout << "    SISTEMA DE TALLER MECANICO    \n";
    
        cout << "1. Registrar Vehiculo\n";
        cout << "2. Mostrar Vehiculos\n";
        cout << "3. Registrar Mecanico\n";
        cout << "4. Registrar Repuesto\n";
        cout << "5. Mostrar Repuestos\n";
        cout << "6. Crear Orden de Servicio\n";
        cout << "7. Mostrar Ordenes de Servicio\n";
        cout << "8. Salir y liberar memoria\n";
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
                registrarVehiculo(sistema);
                break;

            case 2:
                mostrarVehiculos(sistema);
                break;

            case 3: {
                Mecanico nuevo;
                cout << "\n--- REGISTRAR MECANICO ---\n";
                cout << "Ingrese Nombre: ";
                cin.getline(nuevo.nombre, 100);
                cout << "Ingrese Cedula: ";
                cin.getline(nuevo.cedula, 20);
                cout << "Ingrese Especialidad: ";
                cin.getline(nuevo.especialidad, 20);
                cout << "Ingrese Fecha de Ingreso (YYYY-MM-DD): ";
                cin.getline(nuevo.fechaIngreso, 11);

                Mecanico* resultado = registrarMecanico(sistema, nuevo);
                if (resultado != nullptr) {
                    cout << "¡Mecanico registrado con exito con ID #" << resultado->id << "!\n";
                } else {
                    cout << "Error: La cedula ya esta registrada o el sistema esta lleno.\n";
                }
                break;
            }

            case 4:
                registrarRepuesto(sistema);
                break;

            case 5:
                mostrarRepuestos(sistema);
                break;

            case 6:
                crearOrdenServicio(sistema);
                break;

            case 7:
                mostrarOrdenes(sistema);
                break;

            case 8:
                liberarSistema(sistema);
                cout << "\n¡Memoria liberada con exito. Saliendo del programa!\n";
                break;

            default:
                cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 8);

    return 0;
}