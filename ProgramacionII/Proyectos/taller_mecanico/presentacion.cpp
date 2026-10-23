#include "presentacion.h"
#include "logica.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuRegistrarVehiculo(SistemaTaller& s) {
    char placa[10], marca[50], modelo[50];
    int anio;

    cout << "\n--- REGISTRO DE NUEVO VEHICULO ---\n";
    cout << "Ingrese la placa: ";
    cin.getline(placa, 10);

    cout << "Ingrese la marca: ";
    cin.getline(marca, 50);

    cout << "Ingrese el modelo: ";
    cin.getline(modelo, 50);

    cout << "Ingrese el ano del vehiculo: ";
    cin >> anio;
    cin.clear();
    cin.ignore(10000, '\n');

    bool exito = registrarVehiculoLogica(s, placa, marca, modelo, anio);
    if (exito) {
        cout << "¡Vehiculo registrado exitosamente!\n";
    } else {
        cout << "[ERROR]: No se pudo registrar el vehiculo.\n";
    }
}

void menuRegistrarMecanico(SistemaTaller& s) {
    Mecanico nuevo;

    cout << "\n--- REGISTRAR NUEVO MECANICO ---\n";
    cout << "Ingrese Nombre: ";
    cin.getline(nuevo.nombre, 100);
    cout << "Ingrese Cedula: ";
    cin.getline(nuevo.cedula, 20);
    cout << "Ingrese Especialidad: ";
    cin.getline(nuevo.especialidad, 20);
    cout << "Ingrese Fecha de Ingreso (YYYY-MM-DD): ";
    cin.getline(nuevo.fechaIngreso, 11);

    Mecanico* resultado = registrarMecanico(s, nuevo);
    if (resultado != nullptr) {
        cout << "¡Mecanico registrado con exito con ID #" << resultado->id << "!\n";
    } else {
        cout << "[ERROR]: La cedula ya esta registrada o el sistema esta lleno.\n";
    }
}

void menuRegistrarRepuesto(SistemaTaller& s) {
    char codigo[20], nombre[100], marca[50];
    float precio;
    int stock, stockMinimo;

    cout << "\n--- REGISTRO DE NUEVO REPUESTO ---\n";
    cout << "Ingrese el codigo de la pieza: ";
    cin.getline(codigo, 20);

    cout << "Ingrese el nombre del repuesto: ";
    cin.getline(nombre, 100);

    cout << "Ingrese la marca: ";
    cin.getline(marca, 50);

    cout << "Ingrese el precio unitario ($): ";
    cin >> precio;

    cout << "Ingrese la cantidad en stock: ";
    cin >> stock;

    cout << "Ingrese el stock minimo de alerta: ";
    cin >> stockMinimo;
    cin.clear();
    cin.ignore(10000, '\n');

    bool exito = registrarRepuestoLogica(s, codigo, nombre, marca, precio, stock, stockMinimo);
    if (exito) {
        cout << "¡Repuesto registrado exitosamente en el inventario!\n";
    } else {
        cout << "[ERROR]: No se pudo registrar el repuesto.\n";
    }
}

void menuMostrarRepuestos(const SistemaTaller& s) {
    cout << "\n--- INVENTARIO DE REPUESTOS ---\n";
    if (s.numRepuestos == 0) {
        cout << "No hay repuestos registrados en el inventario.\n";
        return;
    }

    for (int i = 0; i < s.numRepuestos; i++) {
        cout << "ID: " << s.repuestos[i].id
                  << " | Codigo: " << s.repuestos[i].codigo
                  << " | Nombre: " << s.repuestos[i].nombre
                  << " | Marca: " << s.repuestos[i].marca
                  << " | Precio: $" << s.repuestos[i].precio
                  << " | Stock: " << s.repuestos[i].stock << "\n";
    }
}

void menuCrearOrden(SistemaTaller& s) {
    char fecha[11], falla[200];
    int km, idVehiculo, idMecanico;
    float manoObra;

    cout << "\n--- CREAR NUEVA ORDEN DE SERVICIO ---\n";
    cout << "Ingrese la fecha de apertura (YYYY-MM-DD): ";
    cin.getline(fecha, 11);

    cout << "Ingrese la descripcion de la falla: ";
    cin.getline(falla, 200);

    cout << "Ingrese el kilometraje de ingreso (ej: 200000): ";
    cin >> km;
    cin.clear();
    cin.ignore(10000, '\n');

    cout << "Ingrese el costo de la mano de obra ($): ";
    cin >> manoObra;
    cin.clear();
    cin.ignore(10000, '\n');

    cout << "Ingrese el ID del vehiculo al que se le hara el servicio: ";
    cin >> idVehiculo;
    cin.clear();
    cin.ignore(10000, '\n');

    cout << "Ingrese el ID del mecanico asignado: ";
    cin >> idMecanico;
    cin.clear();
    cin.ignore(10000, '\n');

    int idCreado = crearOrdenServicioLogica(s, fecha, falla, km, manoObra, idVehiculo, idMecanico);
    if (idCreado == -1) {
        cout << "\n[ERROR]: Debe registrar al menos un Vehiculo y un Mecanico antes de crear una orden.\n";
    } else if (idCreado > 0) {
        cout << "¡Orden de servicio creada exitosamente con ID #" << idCreado << "!\n";
    }
}

void menuAgregarRepuesto(SistemaTaller& s) {
    int idOrden, idRepuesto, cantidad;

    cout << "\n--- AGREGAR REPUESTO A ORDEN DE SERVICIO ---\n";
    cout << "Ingrese el ID de la orden de servicio: ";
    cin >> idOrden;

    cout << "Ingrese el ID del repuesto del inventario: ";
    cin >> idRepuesto;

    cout << "Ingrese la cantidad de unidades a utilizar: ";
    cin >> cantidad;
    cin.clear();
    cin.ignore(10000, '\n');

    int resultado = agregarRepuestoAOrdenLogica(s, idOrden, idRepuesto, cantidad);
    if (resultado == 1) {
        cout << "¡Repuesto agregado exitosamente a la orden! Stock actualizado correctamente.\n";
    } else if (resultado == -3) {
        cout << "[ERROR]: No se encontro ninguna orden con ese ID.\n";
    } else if (resultado == -4) {
        cout << "[ERROR]: Esta orden ya esta cerrada y no se le pueden agregar mas repuestos.\n";
    } else if (resultado == -5) {
        cout << "[ERROR]: No se encontro ningun repuesto con ese ID.\n";
    } else if (resultado == -7) {
        cout << "[ERROR]: Stock insuficiente.\n";
    } else {
        cout << "[ERROR]: Operacion invalida.\n";
    }
}

void menuCerrarOrden(SistemaTaller& s) {
    int idOrden;
    cout << "\n--- CERRAR Y FACTURAR ORDEN DE SERVICIO ---\n";
    cout << "Ingrese el ID de la orden que desea cerrar: ";
    cin >> idOrden;
    cin.clear();
    cin.ignore(10000, '\n');

    double costoRepuestos = 0.0, totalPagar = 0.0;
    OrdenServicio* ordenEn = nullptr;

    int resultado = cerrarOrdenServicioLogica(s, idOrden, costoRepuestos, totalPagar, ordenEn);
    if (resultado == 1) {
        cout << "          FACTURA DE REPARACION         \n";
        cout << " Orden ID     : #" << ordenEn->id << "\n";
        cout << " Estado       : CERRADA\n";
        cout << " Vehiculo ID  : #" << ordenEn->idVehiculo << "\n";
        cout << " Mecanico ID  : #" << ordenEn->idMecanico << "\n";
        cout << " Falla reportada: " << ordenEn->descripcionFalla << "\n";
        cout << "----------------------------------------\n";
        cout << " Mano de Obra : $" << ordenEn->manoObra << "\n";
        cout << " Repuestos Usados ($): $" << costoRepuestos << "\n";
        cout << "----------------------------------------\n";
        cout << " TOTAL A PAGAR: $" << totalPagar << "\n";
        cout << "========================================\n";
        cout << "¡Orden cerrada y facturada con exito!\n";
    } else if (resultado == -3) {
        cout << "[ERROR]: Esta orden ya se encuentra cerrada.\n";
    } else {
        cout << "[ERROR]: No se encontro la orden de servicio.\n";
    }
}

void menuMostrarOrdenes(const SistemaTaller& s) {
    cout << "\n--- LISTA DE ORDENES DE SERVICIO ---\n";
    if (s.numOrdenes == 0) {
        cout << "No hay ordenes de servicio registradas.\n";
        return;
    }

    for (int i = 0; i < s.numOrdenes; i++) {
        cout << "ID Orden: " << s.ordenes[i].id
                  << " | Estado: " << s.ordenes[i].estado
                  << " | Vehiculo ID: " << s.ordenes[i].idVehiculo
                  << " | Mecanico ID: " << s.ordenes[i].idMecanico
                  << " | Falla: " << s.ordenes[i].descripcionFalla
                  << " | Mano de Obra: $" << s.ordenes[i].manoObra << "\n";
    }
}

void menuMostrarVehiculos(const SistemaTaller& s) {
    cout << "\n--- LISTA DE VEHICULOS REGISTRADOS ---\n";
    if (s.numVehiculos == 0) {
        cout << "No hay vehiculos registrados en el sistema.\n";
        return;
    }

    for (int i = 0; i < s.numVehiculos; i++) {
        cout << "ID: " << s.vehiculos[i].id
                  << " | Placa: " << s.vehiculos[i].placa
                  << " | Marca: " << s.vehiculos[i].marca
                  << " | Modelo: " << s.vehiculos[i].modelo
                  << " | Ano: " << s.vehiculos[i].anio << "\n";
    }
}