#ifndef LOGICA_H
#define LOGICA_H
#include "estructura.h"

const int MAX_LINEAS_DETALLE = 15;
const int MAX_ORDENES_ABIERTAS_MECANICO = 3;


bool especialidadValida(const char *especialidad);


int registrarVehiculoLogica(const char *placa, const char *marca, const char *modelo, int anio, int kilometraje, const char *nombrePropietario, const char *cedulaPropietario, const char *telefonoPropietario);

int registrarMecanico(Mecanico datos);


int registrarRepuestoLogica(const char *codigo, const char *nombre, const char *marca, float precio, int stock, int stockMinimo);


int crearOrdenServicioLogica(const char *fecha, const char *falla, int km, int idVehiculo, int idMecanico);

int agregarRepuestoAOrdenLogica(int idOrden, int idRepuesto, int cantidad);


int cerrarOrdenServicioLogica(int idOrden, float manoObra, double &costoRepuestosOut, double &totalPagarOut, OrdenServicio &ordenOut);

int actualizarVehiculoLogica(int idVehiculo, const char *marca, const char *modelo, int anio,
                             const char *nombrePropietario, const char *cedulaPropietario,
                             const char *telefonoPropietario);


int eliminarVehiculoLogica(int idVehiculo);


int darDeBajaMecanicoLogica(int idMecanico);

#endif
