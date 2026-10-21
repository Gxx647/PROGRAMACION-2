#include <iostream>
#include "estructura.h"
#include "logica.h"

using namespace std;

int main() {
    SistemaTaller sistema;
    Taller datosTaller = {"Taller Central", "Av. Principal 123"}; // Ajusta según los campos de tu struct Taller

    // 1. Inicializar el sistema
    inicializarSistema(sistema, datosTaller);
    cout << "Sistema inicializado correctamente." << endl;

    // 2. Probar el registro de un mecánico
    Mecanico nuevoMecanico;
    cout << "\n--- Registrar Mecanico ---" << endl;
    cout << "Ingrese ID del mecanico: ";
    cin >> nuevoMecanico.id; // Ajusta el campo según tu struct Mecanico
    cout << "Ingrese Nombre del mecanico: ";
    cin >> nuevoMecanico.nombre; // Ajusta el campo según tu struct Mecanico

    Mecanico* resultado = registrarMecanico(sistema, nuevoMecanico);
    if (resultado != nullptr) {
        cout << "¡Mecanico registrado con exito!" << endl;
    } else {
        cout << "Error: No se pudo registrar el mecanico (sistema lleno o duplicado)." << endl;
    }

    // 3. Liberar memoria antes de salir
    liberarSistema(sistema);
    cout << "\nSistema cerrado correctamente." << endl;

    return 0;
}