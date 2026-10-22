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

    std::cout << "\n REGISTRO DE NUEVO VEHICULO \n";
    std::cout << "Ingrese la placa: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].placa, 10);

    std::cout << "Ingrese la marca: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].marca, 50);

    std::cout << "Ingrese el modelo: ";
    std::cin.getline(s.vehiculos[s.numVehiculos].modelo, 50);

    std::cout << "Ingrese el año del vehiculo: ";
    std::cin >> s.vehiculos[s.numVehiculos].anio;

    s.numVehiculos++;
    std::cout << "Vehiculo registrado \n";
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

    std::cout << "\n REGISTRO DE NUEVO REPUESTO \n";
    
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
    std::cout << "Repuesto registrado exitosamente en el inventario\n";
}

void mostrarRepuestos(const SistemaTaller& s) {
    std::cout << "\n INVENTARIO DE REPUESTOS \n";
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

    std::cout << "\n CREAR NUEVA ORDEN DE SERVICIO \n";

    std::cout << "Ingrese la fecha de apertura (DD/MM/YYYY): ";
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
    std::cout << "Orden de servicio creada exitosamente con ID #" << nueva.id << "\n";
}

void mostrarOrdenes(const SistemaTaller& s) {
    std::cout << "\nLISTA DE ORDENES DE SERVICIO \n";
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

void agregarRepuestoAOrden(SistemaTaller& s) {
    if (s.numOrdenes == 0) {
        std::cout << "\n[ERROR]: No hay ordenes de servicio registradas.\n";
        return;
    }
    if (s.numRepuestos == 0) {
        std::cout << "\n[ERROR]: No hay repuestos registrados en el inventario.\n";
        return;
    }

    int idOrden, idRepuesto, cantidad;

    std::cout << "\n AGREGAR REPUESTO A ORDEN DE SERVICIO\n";
    std::cout << "Ingrese el ID de la orden de servicio: ";
    std::cin >> idOrden;

   
    OrdenServicio* ordenEncontrada = nullptr;
    for (int i = 0; i < s.numOrdenes; i++) {
        if (s.ordenes[i].id == idOrden) {
            ordenEncontrada = &s.ordenes[i];
            break;
        }
    }

    if (ordenEncontrada == nullptr) {
        std::cout << "[ERROR]: No se encontro ninguna orden con el ID #" << idOrden << ".\n";
        return;
    }

    
    if (std::strcmp(ordenEncontrada->estado, "ABIERTA") != 0) {
        std::cout << "[ERROR]: Esta orden ya esta cerrada y no se le pueden agregar mas repuestos.\n";
        return;
    }

    std::cout << "Ingrese el ID del repuesto del inventario: ";
    std::cin >> idRepuesto;

   
    Repuesto* repuestoEncontrado = nullptr;
    for (int i = 0; i < s.numRepuestos; i++) {
        if (s.repuestos[i].id == idRepuesto) {
            repuestoEncontrado = &s.repuestos[i];
            break;
        }
    }

    if (repuestoEncontrado == nullptr) {
        std::cout << "[ERROR]: No se encontro ningun repuesto con el ID #" << idRepuesto << ".\n";
        return;
    }

    std::cout << "Ingrese la cantidad de unidades a utilizar: ";
    std::cin >> cantidad;

    if (cantidad <= 0) {
        std::cout << "[ERROR]: La cantidad debe ser mayor a cero.\n";
        return;
    }

    
    if (repuestoEncontrado->stock < cantidad) {
        std::cout << "[ERROR]: Stock insuficiente. Solo quedan " << repuestoEncontrado->stock << " unidades disponibles.\n";
        return;
    }

    
    if (ordenEncontrada->numDetalles >= ordenEncontrada->capacidadDetalles) {
        ordenEncontrada->capacidadDetalles *= 2;
        DetalleOrden* nuevosDetalles = new DetalleOrden[ordenEncontrada->capacidadDetalles];

        for (int i = 0; i < ordenEncontrada->numDetalles; i++) {
            nuevosDetalles[i] = ordenEncontrada->detalles[i];
        }

        delete[] ordenEncontrada->detalles;
        ordenEncontrada->detalles = nuevosDetalles;
    }

   
    DetalleOrden& det = ordenEncontrada->detalles[ordenEncontrada->numDetalles];
    det.idRepuesto = repuestoEncontrado->id;
    det.cantidad = cantidad;
    det.precioUnitario = repuestoEncontrado->precio;

   
    repuestoEncontrado->stock -= cantidad;

    ordenEncontrada->numDetalles++;

    std::cout << "Repuesto agregado. Stock actualizado correctamente.\n";
}

void cerrarOrdenServicio(SistemaTaller& s) {
    if (s.numOrdenes == 0) {
        std::cout << "\n[ERROR]: No hay ordenes de servicio registradas.\n";
        return;
    }

    int idOrden;
    std::cout << "\n--- CERRAR Y FACTURAR ORDEN DE SERVICIO ---\n";
    std::cout << "Ingrese el ID de la orden que desea cerrar: ";
    std::cin >> idOrden;

  
    OrdenServicio* ordenEncontrada = nullptr;
    for (int i = 0; i < s.numOrdenes; i++) {
        if (s.ordenes[i].id == idOrden) {
            ordenEncontrada = &s.ordenes[i];
            break;
        }
    }

    if (ordenEncontrada == nullptr) {
        std::cout << "[ERROR]: No se encontro ninguna orden con el ID #" << idOrden << ".\n";
        return;
    }

   
    if (std::strcmp(ordenEncontrada->estado, "CERRADA") == 0) {
        std::cout << "[ERROR]: Esta orden ya se encuentra cerrada.\n";
        return;
    }

    
    for (int i = 0; i < 12; i++) ordenEncontrada->estado[i] = '\0';
    std::strcat(ordenEncontrada->estado, "CERRADA");

    
    double costoRepuestos = 0.0;
    for (int i = 0; i < ordenEncontrada->numDetalles; i++) {
        costoRepuestos += ordenEncontrada->detalles[i].cantidad * ordenEncontrada->detalles[i].precioUnitario;
    }

    
    double totalPagar = ordenEncontrada->manoObra + costoRepuestos;

    
    
    std::cout << "          FACTURA DE REPARACION         \n";
    
    std::cout << " Orden ID     : #" << ordenEncontrada->id << "\n";
    std::cout << " Estado       : CERRADA\n";
    std::cout << " Vehiculo ID  : #" << ordenEncontrada->idVehiculo << "\n";
    std::cout << " Mecanico ID  : #" << ordenEncontrada->idMecanico << "\n";
    std::cout << " Falla reportada: " << ordenEncontrada->descripcionFalla << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << " Mano de Obra : $" << ordenEncontrada->manoObra << "\n";
    std::cout << " Repuestos Usados ($): $" << costoRepuestos << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << " TOTAL A PAGAR: $" << totalPagar << "\n";
    std::cout << "========================================\n";
    std::cout << "¡Orden cerrada y facturada con exito!\n";
}

void mostrarVehiculos(const SistemaTaller& s) {
    std::cout << "\nLISTA DE VEHICULOS REGISTRADOS \n";
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