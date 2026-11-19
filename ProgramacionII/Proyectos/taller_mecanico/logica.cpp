#include "logica.h"
#include "persistencia.h"
#include <cstring>
#include <ctime>

using namespace std;

bool especialidadValida(const char *especialidad)
{
    const char *permitidas[5] = {"Motor", "Frenos", "Electricidad", "Suspension", "General"};
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(especialidad, permitidas[i]) == 0)
        {
            return true;
        }
    }
    return false;
}



int registrarVehiculoLogica(const char *placa, const char *marca, const char *modelo, int anio, int kilometraje, const char *nombrePropietario, const char *cedulaPropietario, const char *telefonoPropietario)
{
    if (anio < 1950 || anio > 2027)
    {
        return -3;
    }
    if (kilometraje < 0)
    {
        return -4;
    }

    
    ArchivoHeader header;
    if (!leerHeader(RUTA_VEHICULOS, header))
    {
        return -1;
    }
    Vehiculo existente;
    for (int i = 1; i <= header.cantidadRegistros; i++)
    {
        if (leerRegistroPorID(RUTA_VEHICULOS, i, existente))
        {
            if (!existente.eliminado && strcmp(existente.placa, placa) == 0)
            {
                return -2;
            }
        }
    }

    Vehiculo nuevo;
    memset(&nuevo, 0, sizeof(Vehiculo)); 

    strncpy(nuevo.placa, placa, 9);
    nuevo.placa[9] = '\0';
    strncpy(nuevo.marca, marca, 49);
    nuevo.marca[49] = '\0';
    strncpy(nuevo.modelo, modelo, 49);
    nuevo.modelo[49] = '\0';
    nuevo.anio = anio;
    nuevo.kilometraje = kilometraje;
    strncpy(nuevo.nombrePropietario, nombrePropietario, 99);
    nuevo.nombrePropietario[99] = '\0';
    strncpy(nuevo.cedulaPropietario, cedulaPropietario, 19);
    nuevo.cedulaPropietario[19] = '\0';
    strncpy(nuevo.telefonoPropietario, telefonoPropietario, 19);
    nuevo.telefonoPropietario[19] = '\0';

    return guardarRegistro(RUTA_VEHICULOS, nuevo); 
}


int registrarMecanico(Mecanico datos)
{
    datos.nombre[99] = '\0';
    datos.cedula[19] = '\0';
    datos.especialidad[19] = '\0';
    datos.fechaIngreso[10] = '\0';

    if (!especialidadValida(datos.especialidad))
    {
        return -3;
    }

    ArchivoHeader header;
    if (!leerHeader(RUTA_MECANICOS, header))
    {
        return -1;
    }

    Mecanico existente;
    for (int i = 1; i <= header.cantidadRegistros; i++)
    {
        if (leerRegistroPorID(RUTA_MECANICOS, i, existente))
        {
            if (!existente.eliminado && strcmp(existente.cedula, datos.cedula) == 0)
            {
                return -2;
            }
        }
    }

    return guardarRegistro(RUTA_MECANICOS, datos); 
}



int registrarRepuestoLogica(const char *codigo, const char *nombre, const char *marca, float precio, int stock, int stockMinimo)
{
    if (precio <= 0)
    {
        return -3;
    }
    if (stock < 0)
    {
        return -4;
    }
    if (stockMinimo < 0)
    {
        return -5;
    }

    ArchivoHeader header;
    if (!leerHeader(RUTA_REPUESTOS, header))
    {
        return -1;
    }

    Repuesto existente;
    for (int i = 1; i <= header.cantidadRegistros; i++)
    {
        if (leerRegistroPorID(RUTA_REPUESTOS, i, existente))
        {
            if (!existente.eliminado && strcmp(existente.codigo, codigo) == 0)
            {
                return -2;
            }
        }
    }

    Repuesto nuevo;
    memset(&nuevo, 0, sizeof(Repuesto));

    strncpy(nuevo.codigo, codigo, 19);
    nuevo.codigo[19] = '\0';
    strncpy(nuevo.nombre, nombre, 99);
    nuevo.nombre[99] = '\0';
    strncpy(nuevo.marca, marca, 49);
    nuevo.marca[49] = '\0';
    nuevo.precio = precio;
    nuevo.stock = stock;
    nuevo.stockMinimo = stockMinimo;

    return guardarRegistro(RUTA_REPUESTOS, nuevo); 
}



int crearOrdenServicioLogica(const char *fecha, const char *falla, int km, int idVehiculo, int idMecanico)
{
    
    Vehiculo veh;
    if (idVehiculo <= 0 || !leerRegistroPorID(RUTA_VEHICULOS, idVehiculo, veh))
    {
        return -2;
    }
    if (veh.eliminado)
    {
        return -3;
    }

    Mecanico mec;
    if (idMecanico <= 0 || !leerRegistroPorID(RUTA_MECANICOS, idMecanico, mec))
    {
        return -4;
    }
    if (mec.eliminado)
    {
        return -5;
    }
    
    ArchivoHeader header;
    if (!leerHeader(RUTA_ORDENES, header))
    {
        return -1;
    }
    int abiertasMecanico = 0;
    OrdenServicio existente;
    for (int i = 1; i <= header.cantidadRegistros; i++)
    {
        if (leerRegistroPorID(RUTA_ORDENES, i, existente))
        {
            if (strcmp(existente.estado, "ABIERTA") == 0)
            {
                if (existente.idVehiculo == idVehiculo)
                {
                    return -6;
                }
                if (existente.idMecanico == idMecanico)
                {
                    abiertasMecanico++;
                }
            }
        }
    }
    if (abiertasMecanico >= MAX_ORDENES_ABIERTAS_MECANICO)
    {
        return -7;
    }
    if (km < veh.kilometraje)
    {
        return -8;
    }
    
    OrdenServicio nueva;
    memset(&nueva, 0, sizeof(OrdenServicio));

    nueva.idVehiculo = idVehiculo;
    nueva.idMecanico = idMecanico;
    strncpy(nueva.fechaApertura, fecha, 10);
    nueva.fechaApertura[10] = '\0';
    strncpy(nueva.descripcionFalla, falla, 199);
    nueva.descripcionFalla[199] = '\0';
    strcpy(nueva.estado, "ABIERTA");
    nueva.kilometrajeIngreso = km;
    nueva.manoObra = 0;
    nueva.totalLineas = 0;

    int idOrden = guardarRegistro(RUTA_ORDENES, nueva);
    if (idOrden < 0)
    {
        return -1;
    }

    veh.kilometraje = km;
    if (!sobrescribirRegistro(RUTA_VEHICULOS, idVehiculo, veh))
    {
        return -1;
    }
    return idOrden;
}

int agregarRepuestoAOrdenLogica(int idOrden, int idRepuesto, int cantidad)
{
    if (cantidad <= 0)
    {
        return -6;
    }

    OrdenServicio orden;
    if (idOrden <= 0 || !leerRegistroPorID(RUTA_ORDENES, idOrden, orden))
    {
        return -3;
    }
    if (strcmp(orden.estado, "ABIERTA") != 0)
    {
        return -4;
    }

    Repuesto rep;
    if (idRepuesto <= 0 || !leerRegistroPorID(RUTA_REPUESTOS, idRepuesto, rep))
    {
        return -5;
    }
    if (rep.eliminado)
    {
        return -5;
    }
    if (rep.stock < cantidad)
    {
        return -7;
    }

 
    int posLinea = -1;
    for (int i = 0; i < orden.totalLineas; i++)
    {
        if (orden.lineasDetalle[i].idRepuesto == idRepuesto)
        {
            posLinea = i;
            break;
        }
    }

    if (posLinea == -1)
    {
        // Linea nueva
        if (orden.totalLineas >= MAX_LINEAS_DETALLE)
        {
            return -8;
        }

        posLinea = orden.totalLineas;
        orden.lineasDetalle[posLinea].idRepuesto = rep.id;
        strncpy(orden.lineasDetalle[posLinea].codigoRepuesto, rep.codigo, 19);
        orden.lineasDetalle[posLinea].codigoRepuesto[19] = '\0';
        strncpy(orden.lineasDetalle[posLinea].nombreRepuesto, rep.nombre, 99);
        orden.lineasDetalle[posLinea].nombreRepuesto[99] = '\0';
        orden.lineasDetalle[posLinea].cantidad = cantidad;
        orden.lineasDetalle[posLinea].precioHistorico = rep.precio;
        orden.lineasDetalle[posLinea].activo = true;
        orden.totalLineas++;
    }
    else
    {
        
        orden.lineasDetalle[posLinea].cantidad += cantidad;
    }

    
    if (!sobrescribirRegistro(RUTA_ORDENES, idOrden, orden))
    {
        return -1;
    }

    rep.stock -= cantidad;
    if (!sobrescribirRegistro(RUTA_REPUESTOS, idRepuesto, rep))
    {
        return -1;
    }

    return 1;
}

int cerrarOrdenServicioLogica(int idOrden, float manoObra, double &costoRepuestosOut, double &totalPagarOut, OrdenServicio &ordenOut)
{
    if (idOrden <= 0 || !leerRegistroPorID(RUTA_ORDENES, idOrden, ordenOut))
    {
        return -2;
    }
    if (strcmp(ordenOut.estado, "ABIERTA") != 0)
    {
        return -3;
    }
    if (manoObra < 0)
    {
        return -4;
    }
    if (ordenOut.totalLineas == 0 && manoObra <= 0)
    {
        return -5;
    }

    double costoRepuestos = 0.0;
    for (int i = 0; i < ordenOut.totalLineas; i++)
    {
        costoRepuestos += ordenOut.lineasDetalle[i].cantidad * ordenOut.lineasDetalle[i].precioHistorico;
    }

    ordenOut.manoObra = manoObra;
    strcpy(ordenOut.estado, "CERRADA");

    time_t ahora = time(nullptr);
    strftime(ordenOut.fechaCierre, 11, "%Y-%m-%d", localtime(&ahora));

    if (!sobrescribirRegistro(RUTA_ORDENES, idOrden, ordenOut))
    {
        return -1;
    }

    costoRepuestosOut = costoRepuestos;
    totalPagarOut = manoObra + costoRepuestos;
    return 1;
}

int actualizarVehiculoLogica(int idVehiculo, const char *marca, const char *modelo, int anio,
                             const char *nombrePropietario, const char *cedulaPropietario,
                             const char *telefonoPropietario)
{
    if (anio < 1950 || anio > 2027)
    {
        return -4;
    }

    Vehiculo veh;
    if (idVehiculo <= 0 || !leerRegistroPorID(RUTA_VEHICULOS, idVehiculo, veh))
    {
        return -2;
    }
    if (veh.eliminado)
    {
        return -3;
    }

   
    strncpy(veh.marca, marca, 49);
    veh.marca[49] = '\0';

    strncpy(veh.modelo, modelo, 49);
    veh.modelo[49] = '\0';

    veh.anio = anio;

    strncpy(veh.nombrePropietario, nombrePropietario, 99);
    veh.nombrePropietario[99] = '\0';

    strncpy(veh.cedulaPropietario, cedulaPropietario, 19);
    veh.cedulaPropietario[19] = '\0';

    strncpy(veh.telefonoPropietario, telefonoPropietario, 19);
    veh.telefonoPropietario[19] = '\0';

    if (!sobrescribirRegistro(RUTA_VEHICULOS, idVehiculo, veh))
    {
        return -1;
    }
    return 1;
}

int eliminarVehiculoLogica(int idVehiculo)
{
    Vehiculo veh;
    if (idVehiculo <= 0 || !leerRegistroPorID(RUTA_VEHICULOS, idVehiculo, veh))
    {
        return -2;
    }
    if (veh.eliminado)
    {
        return -3;
    }

    ArchivoHeader headerOrdenes;
    if (!leerHeader(RUTA_ORDENES, headerOrdenes))
    {
        return -1;
    }

  
    OrdenServicio orden;
    for (int i = 1; i <= headerOrdenes.cantidadRegistros; i++)
    {
        if (leerRegistroPorID(RUTA_ORDENES, i, orden))
        {
            if (orden.idVehiculo == idVehiculo)
            {
                return -4;
            }
        }
    }

    if (!marcarComoEliminado<Vehiculo>(RUTA_VEHICULOS, idVehiculo))
    {
        return -1;
    }
    return 1;
}

int darDeBajaMecanicoLogica(int idMecanico)
{
    Mecanico mec;
    if (idMecanico <= 0 || !leerRegistroPorID(RUTA_MECANICOS, idMecanico, mec))
    {
        return -2;
    }
    if (mec.eliminado)
    {
        return -3;
    }

    ArchivoHeader headerOrdenes;
    if (!leerHeader(RUTA_ORDENES, headerOrdenes))
    {
        return -1;
    }

  
    OrdenServicio orden;
    for (int i = 1; i <= headerOrdenes.cantidadRegistros; i++)
    {
        if (leerRegistroPorID(RUTA_ORDENES, i, orden))
        {
            if (orden.idMecanico == idMecanico && strcmp(orden.estado, "ABIERTA") == 0)
            {
                return -4;
            }
        }
    }

    if (!marcarComoEliminado<Mecanico>(RUTA_MECANICOS, idMecanico))
    {
        return -1;
    }
    return 1;
}
