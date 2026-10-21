#include "logica.h"
#include <iostream>
#include <cstring>
#include "estructura.h"

void inicializarSistema(SistemaTaller& s, Taller datosTaller) {

    s.taller = datosTaller;

    
    s.capacidadVehiculos = 4;
    s.capacidadMecanicos = 4;
    s.capacidadRepuestos = 4;
    s.capacidadOrdenes = 4;


    s.vehiculos = new Vehiculo[s.capacidadVehiculos];
    s.mecanicos = new Mecanico[s.capacidadMecanicos];
    s.repuestos = new Repuesto[s.capacidadRepuestos];
    s.ordenes = new OrdenServicio[s.capacidadOrdenes];

    
    s.numMecanicos = 0;
    s.numRepuestos = 0;
    s.numOrdenes = 0;

  
    s.siguienteIdVehiculo = 1;
    s.siguienteIdMecanico = 1;
    s.siguienteIdRepuesto = 1;
    s.siguienteIdOrden = 1;
}


void liberarSistema(SistemaTaller& s) {
  
    for (int i = 0; i < s.numOrdenes; i++) {
        delete[] s.ordenes[i].detalles;
    }

   
    delete[] s.vehiculos;
    delete[] s.mecanicos;
    delete[] s.repuestos;
    delete[] s.ordenes;
}

Mecanico* registrarMecanico(SistemaTaller& s, Mecanico datos) {
    // 1. Validar que la cédula sea única
    for (int i = 0; i < s.numMecanicos; i++) {
        if (strcmp(s.mecanicos[i].cedula, datos.cedula) == 0) {
            return nullptr; // Retorna nullptr si ya existe
        }
    }

    if (s.numMecanicos >= s.capacidadMecanicos) {
        s.capacidadMecanicos *= 2; // Duplica la capacidad
        Mecanico* nuevoArreglo = new Mecanico[s.capacidadMecanicos];

        for (int i = 0; i < s.numMecanicos; i++) {
            nuevoArreglo[i] = s.mecanicos[i];
        }

        delete[] s.mecanicos;
        s.mecanicos = nuevoArreglo;
    }

    datos.id = s.siguienteIdMecanico;
    s.siguienteIdMecanico++;

    s.mecanicos[s.numMecanicos] = datos;
    s.numMecanicos++;

    return &s.mecanicos[s.numMecanicos - 1];
}