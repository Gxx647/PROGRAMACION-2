#ifndef LOGICA_H
#define LOGICA_H

#include "estructura.h"


void inicializarSistema(SistemaTaller& s, Taller datosTaller);
void liberarSistema(SistemaTaller& s);
void ampliarVehiculos(SistemaTaller& s); 


void registrarVehiculo(SistemaTaller& s);
void mostrarVehiculos(const SistemaTaller& s);

#endif