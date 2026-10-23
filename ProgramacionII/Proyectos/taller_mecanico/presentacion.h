#ifndef PRESENTACION_H
#define PRESENTACION_H

#include "estructura.h"

void menuRegistrarVehiculo(SistemaTaller& s);
void menuRegistrarMecanico(SistemaTaller& s);
void menuRegistrarRepuesto(SistemaTaller& s);
void menuMostrarRepuestos(const SistemaTaller& s);
void menuCrearOrden(SistemaTaller& s);
void menuAgregarRepuesto(SistemaTaller& s);
void menuCerrarOrden(SistemaTaller& s);
void menuMostrarOrdenes(const SistemaTaller& s);
void menuMostrarVehiculos(const SistemaTaller& s);

#endif