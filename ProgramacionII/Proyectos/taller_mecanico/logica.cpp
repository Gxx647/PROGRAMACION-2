#include "logica.h"
#include <cstring>

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

bool registrarVehiculoLogica(SistemaTaller& s, const char* placa, const char* marca, const char* modelo, int anio) {
    if (s.numVehiculos >= s.capacidadVehiculos) {
        ampliarVehiculos(s);
    }

    s.vehiculos[s.numVehiculos].id = s.siguienteIdVehiculo;
    s.siguienteIdVehiculo++;

    std::strncpy(s.vehiculos[s.numVehiculos].placa, placa, 9);
    s.vehiculos[s.numVehiculos].placa[9] = '\0';
    std::strncpy(s.vehiculos[s.numVehiculos].marca, marca, 49);
    s.vehiculos[s.numVehiculos].marca[49] = '\0';
    std::strncpy(s.vehiculos[s.numVehiculos].modelo, modelo, 49);
    s.vehiculos[s.numVehiculos].modelo[49] = '\0';
    s.vehiculos[s.numVehiculos].anio = anio;

    s.numVehiculos++;
    return true;
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

bool registrarRepuestoLogica(SistemaTaller& s, const char* codigo, const char* nombre, const char* marca, float precio, int stock, int stockMinimo) {
    if (s.numRepuestos >= s.capacidadRepuestos) {
        ampliarRepuestos(s);
    }

    s.repuestos[s.numRepuestos].id = s.siguienteIdRepuesto;
    s.siguienteIdRepuesto++;

    std::strncpy(s.repuestos[s.numRepuestos].codigo, codigo, 19);
    s.repuestos[s.numRepuestos].codigo[19] = '\0';
    std::strncpy(s.repuestos[s.numRepuestos].nombre, nombre, 99);
    s.repuestos[s.numRepuestos].nombre[99] = '\0';
    std::strncpy(s.repuestos[s.numRepuestos].marca, marca, 49);
    s.repuestos[s.numRepuestos].marca[49] = '\0';
    s.repuestos[s.numRepuestos].precio = precio;
    s.repuestos[s.numRepuestos].stock = stock;
    s.repuestos[s.numRepuestos].stockMinimo = stockMinimo;

    s.numRepuestos++;
    return true;
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

int crearOrdenServicioLogica(SistemaTaller& s, const char* fecha, const char* falla, int km, float manoObra, int idVehiculo, int idMecanico) {
    if (s.numVehiculos == 0 || s.numMecanicos == 0) return -1; 

    if (s.numOrdenes >= s.capacidadOrdenes) {
        ampliarOrdenes(s);
    }

    OrdenServicio& nueva = s.ordenes[s.numOrdenes];
    nueva.id = s.siguienteIdOrden;
    s.siguienteIdOrden++;

    std::strncpy(nueva.fechaApertura, fecha, 10);
    nueva.fechaApertura[10] = '\0';
    std::strncpy(nueva.descripcionFalla, falla, 199);
    nueva.descripcionFalla[199] = '\0';
    nueva.kilometrajeIngreso = km;
    nueva.manoObra = manoObra;
    nueva.idVehiculo = idVehiculo;
    nueva.idMecanico = idMecanico;

    for(int i=0; i<12; i++) nueva.estado[i] = '\0';
    std::strcat(nueva.estado, "ABIERTA");

    nueva.capacidadDetalles = 2;
    nueva.numDetalles = 0;
    nueva.detalles = new DetalleOrden[nueva.capacidadDetalles];

    s.numOrdenes++;
    return nueva.id;
}

int agregarRepuestoAOrdenLogica(SistemaTaller& s, int idOrden, int idRepuesto, int cantidad) {
    if (s.numOrdenes == 0) return -1; 
    if (s.numRepuestos == 0) return -2; 

    OrdenServicio* ordenEncontrada = nullptr;
    for (int i = 0; i < s.numOrdenes; i++) {
        if (s.ordenes[i].id == idOrden) {
            ordenEncontrada = &s.ordenes[i];
            break;
        }
    }
    if (ordenEncontrada == nullptr) return -3; // Orden no encontrada
    if (std::strcmp(ordenEncontrada->estado, "ABIERTA") != 0) return -4; // Orden ya cerrada

    Repuesto* repuestoEncontrado = nullptr;
    for (int i = 0; i < s.numRepuestos; i++) {
        if (s.repuestos[i].id == idRepuesto) {
            repuestoEncontrado = &s.repuestos[i];
            break;
        }
    }
    if (repuestoEncontrado == nullptr) return -5; 
    if (cantidad <= 0) return -6; 
    if (repuestoEncontrado->stock < cantidad) return -7; 

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

    return 1; // Éxito
}

int cerrarOrdenServicioLogica(SistemaTaller& s, int idOrden, double& costoRepuestosOut, double& totalPagarOut, OrdenServicio*& ordenEncontradaOut) {
    if (s.numOrdenes == 0) return -1;

    OrdenServicio* ordenEncontrada = nullptr;
    for (int i = 0; i < s.numOrdenes; i++) {
        if (s.ordenes[i].id == idOrden) {
            ordenEncontrada = &s.ordenes[i];
            break;
        }
    }
    if (ordenEncontrada == nullptr) return -2;
    if (std::strcmp(ordenEncontrada->estado, "CERRADA") == 0) return -3;

    for (int i = 0; i < 12; i++) ordenEncontrada->estado[i] = '\0';
    std::strcat(ordenEncontrada->estado, "CERRADA");

    double costoRepuestos = 0.0;
    for (int i = 0; i < ordenEncontrada->numDetalles; i++) {
        costoRepuestos += ordenEncontrada->detalles[i].cantidad * ordenEncontrada->detalles[i].precioUnitario;
    }

    double totalPagar = ordenEncontrada->manoObra + costoRepuestos;

    costoRepuestosOut = costoRepuestos;
    totalPagarOut = totalPagar;
    ordenEncontradaOut = ordenEncontrada;

    return 1; 
}