#include "presentacion.h"
#include "estructura.h"
#include "logica.h"
#include <iostream>

using namespace std;

void menuRegistrarMecanico(SistemaTaller& s) {
    Mecanico nuevo;

    cout << "\n--- REGISTRAR NUEVO MECANICO ---\n";
    cout << "Ingrese nombre completo: ";
    cin.ignore();
    cin.getline(nuevo.nombre, 100);

    cout << "Ingrese cedula: ";
    cin >> nuevo.cedula;

    cout << "Ingrese especialidad (MOTOR, FRENOS, ELECTRICIDAD, SUSPENSION, GENERAL): ";
    cin >> nuevo.especialidad;

    cout << "Ingrese fecha de ingreso (YYYY-MM-DD): ";
    cin >> nuevo.fechaIngreso;

    Mecanico* resultado = registrarMecanico(s, nuevo);

    if (resultado == nullptr) {
        cout << "ERROR: La cedula '" << nuevo.cedula << "' ya esta registrada o los datos son invalidos.\n";
    } else {
        cout << "¡Exito! Mecanico registrado correctamente con ID #" << resultado->id << ".\n";
    }
}