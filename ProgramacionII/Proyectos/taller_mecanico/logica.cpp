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

   
    s.numVehiculos = 0; 
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



void registrarVehiculo(SistemaTaller& s) {
    if (s.numVehiculos >= s.capacidadVehiculos) {
        ampliarVehiculos(s);
    }

    s.vehiculos[s.numVehiculos].id = s.siguienteIdVehiculo;
    s.siguienteIdVehiculo++;

    std::cout << "\n--- REGISTRO DE NUEVO VEHICULO ---\n";
    std::cout << "Ingrese la placa: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].placa, 10);

    std::cout << "Ingrese la marca: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].marca, 50);

    std::cout << "Ingrese el modelo: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].modelo, 50);

    std::cout << "Ingrese el año del vehiculo: ";
    std::cin >> s.vehiculos[s.numVehiculos].anio;

    s.numVehiculos++;
    std::cout << "¡Vehiculo registrado exitosamente!\n";
}


Mecanico* registrarMecanico(SistemaTaller& s, Mecanico datos) {
    
    for (int i = 0; i < s.numMecanicos; i++) {
        if (strcmp(s.mecanicos[i].cedula, datos.cedula) == 0) {
            return nullptr; 
        }
    }

    
    if (s.numMecanicos >= s.capacidadMecanicos) {
        s.capacidadMecanicos *= 2; 
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

void ampliarRepuestos(SistemaTaller& s) {
   
    int nuevaCapacidad = s.capacidadRepuestos * 2;
    Repuesto* nuevoArreglo = new Repuesto[nuevaCapacidad];

    
    for (int i = 0; i < s.numRepuestos; i++) {
        nuevoArreglo[i] = s.repuestos[i];
    }

    
    delete[] s.repuestos;

   
    s.repuestos = nuevoArreglo;
    s.capacidadRepuestos = nuevaCapacidad;
}



void registrarRepuesto(SistemaTaller& s) {
    if (s.numRepuestos >= s.capacidadRepuestos) {
        ampliarRepuestos(s);
    }

    s.repuestos[s.numRepuestos].id = s.siguienteIdRepuesto;
    s.siguienteIdRepuesto++;

    std::cout << "\n--- REGISTRO DE NUEVO REPUESTO ---\n";
    
    std::cout << "Ingrese el codigo de la pieza: ";
    std::cin.getline(s.repuestos[s.numRepuestos].codigo, 20);

    std::cout << "Ingrese el nombre del repuesto: ";
    std::cin.getline(s.repuestos[s.numRepuestos].nombre, 100);

    std::cout << "Ingrese la marca: ";
    std::cin.getline(s.repuestos[s.numRepuestos].marca, 50);

    std::cout << "Ingrese el precio unitario ($): ";
    std::cin >> s.repuestos[s.numRepuestos].precio;

    std::cout << "Ingrese la cantidad en stock: ";
    std::cin >> s.repuestos[s.numRepuestos].stock;

    std::cout << "Ingrese el stock minimo de alerta: ";
    std::cin >> s.repuestos[s.numRepuestos].stockMinimo;

    s.numRepuestos++;
    std::cout << "¡Repuesto registrado exitosamente en el inventario!\n";
}

void mostrarRepuestos(const SistemaTaller& s) {
    std::cout << "\n--- INVENTARIO DE REPUESTOS ---\n";
    if (s.numRepuestos == 0) {
        std::cout << "No hay repuestos registrados en el inventario.\n";
        return;
    }

   
    for (int i = 0; i < s.numRepuestos; i++) {
        std::cout << "ID: " << s.repuestos[i].id
                  << " | Codigo: " << s.repuestos[i].codigo
                  << " | Nombre: " << s.repuestos[i].nombre
                  << " | Marca: " << s.repuestos[i].marca
                  << " | Precio: $" << s.repuestos[i].precio
                  << " | Stock: " << s.repuestos[i].stock << "\n";
    }
}




void ampliarOrdenes(SistemaTaller& s) {
   
    int nuevaCapacidad = s.capacidadOrdenes * 2;
    OrdenServicio* nuevoArreglo = new OrdenServicio[nuevaCapacidad];


    for (int i = 0; i < s.numOrdenes; i++) {
        nuevoArreglo[i] = s.ordenes[i];
    }

   
    delete[] s.ordenes;

   
    s.ordenes = nuevoArreglo;
    s.capacidadOrdenes = nuevaCapacidad;
}



void crearOrdenServicio(SistemaTaller& s) {
    if (s.numVehiculos == 0 || s.numMecanicos == 0) {
        std::cout << "\n[ERROR]: Debe registrar al menos un Vehiculo y un Mecanico antes de crear una orden.\n";
        return;
    }

    if (s.numOrdenes >= s.capacidadOrdenes) {
        ampliarOrdenes(s);
    }

    OrdenServicio& nueva = s.ordenes[s.numOrdenes];

    nueva.id = s.siguienteIdOrden;
    s.siguienteIdOrden++;

    std::cout << "\n--- CREAR NUEVA ORDEN DE SERVICIO ---\n";

    std::cout << "Ingrese la fecha de apertura (YYYY-MM-DD): ";
    std::cin.getline(nueva.fechaApertura, 11);

    std::cout << "Ingrese la descripcion de la falla: ";
    std::cin.getline(nueva.descripcionFalla, 200);

    std::cout << "Ingrese el kilometraje de ingreso: ";
    std::cin >> nueva.kilometrajeIngreso;

    std::cout << "Ingrese el costo de la mano de obra ($): ";
    std::cin >> nueva.manoObra;

    std::cout << "Ingrese el ID del vehiculo al que se le hara el servicio: ";
    std::cin >> nueva.idVehiculo;

    std::cout << "Ingrese el ID del mecanico asignado: ";
    std::cin >> nueva.idMecanico;

    for(int i=0; i<12; i++) nueva.estado[i] = '\0';
    std::strcat(nueva.estado, "ABIERTA");

    nueva.capacidadDetalles = 2;
    nueva.numDetalles = 0;
    nueva.detalles = new DetalleOrden[nueva.capacidadDetalles];

    s.numOrdenes++;
    std::cout << "¡Orden de servicio creada exitosamente con ID #" << nueva.id << "!\n";
}

void mostrarOrdenes(const SistemaTaller& s) {
    std::cout << "\n--- LISTA DE ORDENES DE SERVICIO ---\n";
    if (s.numOrdenes == 0) {
        std::cout << "No hay ordenes de servicio registradas.\n";
        return;
    }

    
    for (int i = 0; i < s.numOrdenes; i++) {
        std::cout << "ID Orden: " << s.ordenes[i].id
                  << " | Estado: " << s.ordenes[i].estado
                  << " | Vehiculo ID: " << s.ordenes[i].idVehiculo
                  << " | Mecanico ID: " << s.ordenes[i].idMecanico
                  << " | Falla: " << s.ordenes[i].descripcionFalla
                  << " | Mano de Obra: $" << s.ordenes[i].manoObra << "\n";
    }
}