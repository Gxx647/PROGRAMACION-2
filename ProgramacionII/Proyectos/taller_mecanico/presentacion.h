#ifndef PRESENTACION_H
#define PRESENTACION_H

#include "estructura.h"

void mostrarVehiculo(const Vehiculo& v);
void mostrarMecanico(const Mecanico& m);
void mostrarRepuesto(const Repuesto& r);
void mostrarOrden(const OrdenServicio& o);

void menuRegistrarVehiculo();
void menuMostrarVehiculos();
void menuActualizarVehiculo();
void menuEliminarVehiculo();

void menuRegistrarMecanico();
void menuMostrarMecanicos();
void menuDarDeBajaMecanico();

void menuRegistrarRepuesto();
void menuMostrarRepuestos();
void menuEntradaMercancia();
void menuCambiarPrecioRepuesto();
void menuEliminarRepuesto();

void menuCrearOrden();
void menuAgregarRepuesto();
void menuQuitarRepuesto();
void menuCerrarOrden();
void menuAnularOrden();
void menuMostrarOrdenes();

void submenuVehiculos();
void submenuMecanicos();
void submenuRepuestos();
void submenuOrdenes();

#endif