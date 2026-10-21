#ifndef LOGICA_H
#define LOGICA_H
#include "estructura.h"

void inicializarSistema(SistemaTaller& s, Taller datosTaller);
void liberarSistema(SistemaTaller& s);

Mecanico* registrarMecanico(SistemaTaller& s, Mecanico datos);

#endif