#include <iostream>
#include <fstream>
#include "estructura.h"
#include "persistencia.h"
#include "mantenimiento.h"
#include "reportes.h"
#include "presentacion.h"

using namespace std;

int main() {
    system("chcp 65001 > nul"); 

    
    if (!inicializarSistemaArchivos()) {
        cout << "[ERROR FATAL]: No se pudo inicializar el sistema de archivos en disco.\n";
        return 1;
    }

    
    Taller datosTaller;
    if (!existeTaller()) {
        cout << "=== CONFIGURACION INICIAL DEL TALLER ===\n";
        cout << "Ingrese nombre del taller: ";
        cin.getline(datosTaller.nombre, 100);
        cout << "Ingrese RIF: ";
        cin.getline(datosTaller.rif, 20);
        cout << "Ingrese direccion: ";
        cin.getline(datosTaller.direccion, 200);
        cout << "Ingrese telefono: ";
        cin.getline(datosTaller.telefono, 20);
        
        guardarTaller(datosTaller);
        cout << "¡Datos del taller guardados en disco exitosamente!\n";
    } else {
        leerTaller(datosTaller);
    }

    int opcion = 0;

    do {
        
        cout << "\n========================================\n";
        cout << "      SISTEMA DE GESTION DE TALLER      \n";
        cout << " Taller: " << datosTaller.nombre << "\n";
        cout << "========================================\n";
        cout << "1. Gestion de Vehiculos\n";
        cout << "2. Gestion de Mecanicos (Incluye baja)\n"; 
        cout << "3. Inventario de Repuestos\n";
        cout << "4. Ordenes de Servicio\n";
        cout << "5. Reportes\n";          
        cout << "6. Mantenimiento\n";      
        cout << "0. Salir\n";
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
                submenuVehiculos();
                break;
            case 2:
                submenuMecanicos();
                break;
            case 3:
                submenuRepuestos();
                break;
            case 4:
                submenuOrdenes();
                break;
            case 5:
                cout << "[En desarrollo] Modulo De Reportes.\n";
                break;
            case 6:
                
                {
                    int opMant = 0;
                    cout << "\n--- MENU DE MANTENIMIENTO ---\n";
                    cout << "1. Verificar Integridad\n";
                    cout << "2. Crear Backup\n";
                    cout << "Seleccione: ";
                    cin >> opMant;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    if (opMant == 1) {
                        verificarIntegridad();
                    } else if (opMant == 2) {
                        string rutaBkp = crearBackup();
                        cout << "Respaldo creado con exito en la carpeta: " << rutaBkp << "\n";
                    }
                }
                break;
            case 0:
                cout << "\n¡Saliendo del programa! Los datos ya estan persistidos en disco.\n";
                break;
            default:
                cout << "\nOpcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}