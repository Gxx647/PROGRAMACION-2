#ifndef LOGICA_H
#define LOGICA_H
#include "estructura.h"


void inicializarSistema(SistemaTaller& s, Taller datosTaller);
void liberarSistema(SistemaTaller& s);
void ampliarVehiculos(SistemaTaller& s);

void registrarVehiculo(SistemaTaller& s);
void mostrarVehiculos(const SistemaTaller& s);


Mecanico* registrarMecanico(SistemaTaller& s, Mecanico datos);


void ampliarRepuestos(SistemaTaller& s);
void registrarRepuesto(SistemaTaller& s);
void mostrarRepuestos(const SistemaTaller& s);


void ampliarOrdenes(SistemaTaller& s);
void crearOrdenServicio(SistemaTaller& s);
void mostrarOrdenes(const SistemaTaller& s); 
void agregarRepuestoAOrden(SistemaTaller& s);

void cerrarOrdenServicio(SistemaTaller& s);

void mostrarVehiculos(const SistemaTaller& s);

#endif