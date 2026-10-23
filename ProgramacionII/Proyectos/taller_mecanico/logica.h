#ifndef LOGICA_H
#define LOGICA_H
#include "estructura.h"

void inicializarSistema(SistemaTaller& s, Taller datosTaller);
void liberarSistema(SistemaTaller& s);
void ampliarVehiculos(SistemaTaller& s);
void ampliarRepuestos(SistemaTaller& s);
void ampliarOrdenes(SistemaTaller& s);

bool registrarVehiculoLogica(SistemaTaller& s, const char* placa, const char* marca, const char* modelo, int anio);
Mecanico* registrarMecanico(SistemaTaller& s, Mecanico datos);
bool registrarRepuestoLogica(SistemaTaller& s, const char* codigo, const char* nombre, const char* marca, float precio, int stock, int stockMinimo);

int crearOrdenServicioLogica(SistemaTaller& s, const char* fecha, const char* falla, int km, float manoObra, int idVehiculo, int idMecanico);
int agregarRepuestoAOrdenLogica(SistemaTaller& s, int idOrden, int idRepuesto, int cantidad);
int cerrarOrdenServicioLogica(SistemaTaller& s, int idOrden, double& costoRepuestosOut, double& totalPagarOut, OrdenServicio*& ordenEncontradaOut);

#endif