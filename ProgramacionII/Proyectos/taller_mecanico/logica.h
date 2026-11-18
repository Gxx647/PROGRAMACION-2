#ifndef LOGICA_H
#define LOGICA_H
#include "estructura.h"

const int MAX_LINEAS_DETALLE = 15;
const int MAX_ORDENES_ABIERTAS_MECANICO = 3;

// Ajusten esta lista a las especialidades que usaron en el Proyecto 1
bool especialidadValida(const char* especialidad);

// ---------- VEHICULOS ----------
// Retorna el ID asignado (> 0) o:
//  -1 error de archivo | -2 placa duplicada (activos) | -3 anio fuera de rango | -4 kilometraje negativo
int registrarVehiculoLogica(const char* placa, const char* marca, const char* modelo, int anio, int kilometraje, const char* nombrePropietario, const char* cedulaPropietario, const char* telefonoPropietario);

// ---------- MECANICOS ----------
// Retorna el ID asignado (> 0) o:
//  -1 error de archivo | -2 cedula duplicada | -3 especialidad no permitida
int registrarMecanico(Mecanico datos);

// ---------- REPUESTOS ----------
// Retorna el ID asignado (> 0) o:
//  -1 error de archivo | -2 codigo duplicado (activos) | -3 precio <= 0 | -4 stock < 0 | -5 stock minimo < 0
int registrarRepuestoLogica(const char* codigo, const char* nombre, const char* marca, float precio, int stock, int stockMinimo);

// ---------- ORDENES ----------
// Retorna el ID de la orden (> 0) o:
//  -1 error de archivo | -2 vehiculo no existe | -3 vehiculo eliminado | -4 mecanico no existe
//  -5 mecanico dado de baja | -6 vehiculo ya tiene orden ABIERTA | -7 mecanico con 3 ordenes ABIERTAS
//  -8 kilometraje menor al del vehiculo
int crearOrdenServicioLogica(const char* fecha, const char* falla, int km, int idVehiculo, int idMecanico);

// Retorna 1 si se agrego o:
//  -1 error de archivo | -3 orden no encontrada | -4 orden no esta ABIERTA | -5 repuesto no encontrado
//  -6 cantidad invalida | -7 stock insuficiente | -8 limite de lineas alcanzado
int agregarRepuestoAOrdenLogica(int idOrden, int idRepuesto, int cantidad);

// Retorna 1 si se cerro o:
//  -1 error de archivo | -2 orden no encontrada | -3 orden no esta ABIERTA (ordenOut trae el estado actual)
//  -4 mano de obra negativa | -5 sin lineas y mano de obra = 0
// ordenOut se llena siempre que la orden exista (es una copia, no un puntero).
int cerrarOrdenServicioLogica(int idOrden, float manoObra, double& costoRepuestosOut, double& totalPagarOut, OrdenServicio& ordenOut);

#endif
