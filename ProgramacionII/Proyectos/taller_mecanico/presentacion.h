#ifndef PRESENTACION_H
#define PRESENTACION_H

#include "estructura.h"

// --- Submenus Principales ---

void submenuVehiculos();
void submenuMecanicos();
void submenuRepuestos();
void submenuOrdenes();

// --- Funciones individuales de Vehiculos ---

void menuRegistrarVehiculos();
void menuMotrarVehiculo();

// --- Funciones individuales de los Mecanicos ---

void menuRegistrarMecanico();
void menuMostrarMecanico();

// --- Funciones individuales de Repuestos ---

void menuRegistrarRepuesto();
void mostrarRepuestos();

// --- Funciones individuales de Ordenes ---

void menuCrearOrden();
void menuAgregarRepuestos();
void menuCerrarOrden();
void menuMostrarOrdenes();

#endif