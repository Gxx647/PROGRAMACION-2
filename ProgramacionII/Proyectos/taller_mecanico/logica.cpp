#include "logica.h"
#include <iostream>


// inicio


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

  
    s.numVehiculos = 0; 
    s.numMecanicos = 0;
    s.numRepuestos = 0;
    s.numOrdenes = 0;

    
    s.siguienteIdVehiculo = 1;
    s.siguienteIdMecanico = 1;
    s.siguienteIdRepuesto = 1;
    s.siguienteIdOrden = 1;
}

// liberar memoria

void liberarSistema(SistemaTaller& s) {
    
    for (int i = 0; i < s.numOrdenes; i++) {
        delete[] s.ordenes[i].detalles;
    }

    
    delete[] s.vehiculos;
    delete[] s.mecanicos;
    delete[] s.repuestos;
    delete[] s.ordenes;
}


//ESPACIO DE VEHÍCULOS


void ampliarVehiculos(SistemaTaller& s) {
  
    int nuevaCapacidad = s.capacidadVehiculos * 2;

    
    Vehiculo* nuevoArreglo = new Vehiculo[nuevaCapacidad];

  
    for (int i = 0; i < s.numVehiculos; i++) {
        nuevoArreglo[i] = s.vehiculos[i];
    }

   
    delete[] s.vehiculos;


    s.vehiculos = nuevoArreglo;
    s.capacidadVehiculos = nuevaCapacidad;
}


// registro 
void registrarVehiculo(SistemaTaller& s) {
    
    if (s.numVehiculos >= s.capacidadVehiculos) {
        ampliarVehiculos(s); 
    }

    
    s.vehiculos[s.numVehiculos].id = s.siguienteIdVehiculo;
    s.siguienteIdVehiculo++;


    std::cin.ignore();

    std::cout << "\nREGISTRO DE  VEHICULO \n";
    
    std::cout << "Ingrese la placa: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].placa, 20);

    std::cout << "Ingrese la marca: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].marca, 20);

    std::cout << "Ingrese el modelo: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].modelo, 20);

    std::cout << "Ingrese el año del vehiculo: ";
    std::cin >> s.vehiculos[s.numVehiculos].anio;

   
    s.numVehiculos++;

    std::cout << "¡Vehiculo registrado exitosamente!\n";
}

void mostrarVehiculos(const SistemaTaller& s) {
    std::cout << "\nLISTA DE VEHICULOS REGISTRADOS\n";
    
    
    if (s.numVehiculos == 0) {
        std::cout << "No hay vehiculos registrados en el sistema.\n";
        return;
    }

  
    for (int i = 0; i < s.numVehiculos; i++) {
        std::cout << "ID: " << s.vehiculos[i].id
                  << " | Placa: " << s.vehiculos[i].placa
                  << " | Marca: " << s.vehiculos[i].marca
                  << " | Modelo: " << s.vehiculos[i].modelo
                  << " | Año: " << s.vehiculos[i].anio << "\n";
    }
}