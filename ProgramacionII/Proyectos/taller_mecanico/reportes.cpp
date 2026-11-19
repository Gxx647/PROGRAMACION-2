#include "reportes.h"
#include "persistencia.h"
#include "logica.h"
#include <iostream>
#include <cstring>

namespace
{
    double calcularTotalOrden(const OrdenServicio &o)
    {
        double totalRepuestos = 0.0;
        int limite = o.totalLineas;
        if (limite > MAX_LINEAS_DETALLE)
            limite = MAX_LINEAS_DETALLE;
        for (int i = 0; i < limite; i++)
        {
            if (o.lineasDetalle[i].activo)
            {
                totalRepuestos += static_cast<double>(o.lineasDetalle[i].cantidad) *
                                  static_cast<double>(o.lineasDetalle[i].precioHistorico);
            }
        }
        return static_cast<double>(o.manoObra) + totalRepuestos;
    }

    double calcularRepuestosOrden(const OrdenServicio &o)
    {
        double totalRepuestos = 0.0;
        int limite = o.totalLineas;
        if (limite > MAX_LINEAS_DETALLE)
            limite = MAX_LINEAS_DETALLE;
        for (int i = 0; i < limite; i++)
        {
            if (o.lineasDetalle[i].activo)
            {
                totalRepuestos += static_cast<double>(o.lineasDetalle[i].cantidad) *
                                  static_cast<double>(o.lineasDetalle[i].precioHistorico);
            }
        }
        return totalRepuestos;
    }
}

void reporteHistorialVehiculo(const char *placa)
{
    std::cout << "\n=== REPORTE 1: HISTORIAL DE VEHICULO ===\n";

    ArchivoHeader hVeh;
    if (!leerHeader(RUTA_VEHICULOS, hVeh))
    {
        std::cout << "[ERROR]: No se pudo leer vehiculos.bin\n";
        return;
    }

    bool encontrado = false;
    Vehiculo veh;
    for (int i = 1; i <= hVeh.cantidadRegistros; i++)
    {
        Vehiculo actual;
        if (leerRegistroPorID(RUTA_VEHICULOS, i, actual) && !actual.eliminado)
        {
            if (std::strcmp(actual.placa, placa) == 0)
            {
                veh = actual;
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado)
    {
        std::cout << "No se encontro un vehiculo activo con la placa: " << placa << "\n";
        return;
    }

    std::cout << "Vehiculo ID #" << veh.id << " | Placa: " << veh.placa
              << " | " << veh.marca << " " << veh.modelo << " (" << veh.anio << ")\n";
    std::cout << "Propietario: " << veh.nombrePropietario
              << " | Cedula: " << veh.cedulaPropietario
              << " | Km actual: " << veh.kilometraje << "\n";
    std::cout << "----------------------------------------\n";

    ArchivoHeader hOrd;
    if (!leerHeader(RUTA_ORDENES, hOrd))
    {
        std::cout << "[ERROR]: No se pudo leer ordenes.bin\n";
        return;
    }

    double totalFacturadoCerradas = 0.0;
    int cantidadOrdenes = 0;

    for (int i = 1; i <= hOrd.cantidadRegistros; i++)
    {
        OrdenServicio o;
        if (leerRegistroPorID(RUTA_ORDENES, i, o) && !o.eliminado && o.idVehiculo == veh.id)
        {
            cantidadOrdenes++;

            Mecanico m;
            bool okMec = (o.idMecanico > 0 && leerRegistroPorID(RUTA_MECANICOS, o.idMecanico, m));
            double totalOrden = calcularTotalOrden(o);

            std::cout << "Orden #" << o.id
                      << " | Fecha: " << o.fechaApertura
                      << " | Mecanico: " << (okMec ? m.nombre : "Desconocido")
                      << (okMec && m.eliminado ? " [BAJA]" : "")
                      << " | Estado: " << o.estado
                      << " | Total: $" << totalOrden << "\n";

            if (std::strcmp(o.estado, "CERRADA") == 0)
            {
                totalFacturadoCerradas += totalOrden;
            }
        }
    }

    if (cantidadOrdenes == 0)
    {
        std::cout << "El vehiculo no tiene ordenes registradas.\n";
    }
    std::cout << "----------------------------------------\n";
    std::cout << "Total facturado en ordenes CERRADAS: $" << totalFacturadoCerradas << "\n";
}

void reporteRepuestosMasUtilizados()
{
    std::cout << "\n=== REPORTE 2: REPUESTOS MAS UTILIZADOS (ORDENES CERRADAS) ===\n";

    ArchivoHeader hRep, hOrd;
    if (!leerHeader(RUTA_REPUESTOS, hRep) || !leerHeader(RUTA_ORDENES, hOrd))
    {
        std::cout << "[ERROR]: No se pudieron leer los archivos necesarios.\n";
        return;
    }

    if (hRep.cantidadRegistros <= 0)
    {
        std::cout << "No hay repuestos registrados.\n";
        return;
    }

    struct TopItem
    {
        int idRepuesto;
        char codigo[20];
        char nombre[100];
        long long unidades;
    };

    const int TOP_N = 10;
    TopItem top[TOP_N];
    for (int k = 0; k < TOP_N; k++)
    {
        top[k].idRepuesto = -1;
        top[k].codigo[0] = '\0';
        top[k].nombre[0] = '\0';
        top[k].unidades = 0;
    }

    for (int rIdx = 1; rIdx <= hRep.cantidadRegistros; rIdx++)
    {
        Repuesto rep;
        if (!leerRegistroPorID(RUTA_REPUESTOS, rIdx, rep))
            continue;

        long long acumulado = 0;
        for (int oIdx = 1; oIdx <= hOrd.cantidadRegistros; oIdx++)
        {
            OrdenServicio ord;
            if (leerRegistroPorID(RUTA_ORDENES, oIdx, ord) && !ord.eliminado &&
                std::strcmp(ord.estado, "CERRADA") == 0)
            {
                int limite = ord.totalLineas;
                if (limite > MAX_LINEAS_DETALLE)
                    limite = MAX_LINEAS_DETALLE;
                for (int l = 0; l < limite; l++)
                {
                    if (ord.lineasDetalle[l].activo && ord.lineasDetalle[l].idRepuesto == rep.id)
                    {
                        acumulado += ord.lineasDetalle[l].cantidad;
                    }
                }
            }
        }

        if (acumulado > 0)
        {
            TopItem candidato = {rep.id, "", "", acumulado};
            std::strncpy(candidato.codigo, rep.codigo, 19);
            candidato.codigo[19] = '\0';
            std::strncpy(candidato.nombre, rep.nombre, 99);
            candidato.nombre[99] = '\0';

            int pos = -1;
            for (int k = 0; k < TOP_N; k++)
            {
                if (candidato.unidades > top[k].unidades)
                {
                    pos = k;
                    break;
                }
            }
            if (pos != -1)
            {
                for (int k = TOP_N - 1; k > pos; k--)
                {
                    top[k] = top[k - 1];
                }
                top[pos] = candidato;
            }
        }
    }

    int mostrados = 0;
    for (int k = 0; k < TOP_N; k++)
    {
        if (top[k].unidades > 0)
        {
            std::cout << (mostrados + 1) << ". [" << top[k].codigo << "] "
                      << top[k].nombre << " - Unidades: " << top[k].unidades << "\n";
            mostrados++;
        }
    }
    if (mostrados == 0)
    {
        std::cout << "No hay consumos de repuestos en ordenes CERRADAS.\n";
    }
}

void reporteFacturacionMecanicos()
{
    std::cout << "\n=== REPORTE 3: FACTURACION POR MECANICO ===\n";

    ArchivoHeader hMec, hOrd;
    if (!leerHeader(RUTA_MECANICOS, hMec) || !leerHeader(RUTA_ORDENES, hOrd))
    {
        std::cout << "[ERROR]: No se pudieron leer los archivos necesarios.\n";
        return;
    }

    std::cout << "Mecanico | Ordenes | M. obra | Repuestos\n";
    std::cout << "-------------------------------------------\n";

    int totalMecs = 0;
    for (int mIdx = 1; mIdx <= hMec.cantidadRegistros; mIdx++)
    {
        Mecanico mec;
        if (!leerRegistroPorID(RUTA_MECANICOS, mIdx, mec))
            continue;
        totalMecs++;

        int ordenesCerradas = 0;
        double totalManoObra = 0.0;
        double totalRepuestos = 0.0;

        for (int oIdx = 1; oIdx <= hOrd.cantidadRegistros; oIdx++)
        {
            OrdenServicio ord;
            if (leerRegistroPorID(RUTA_ORDENES, oIdx, ord) && !ord.eliminado &&
                ord.idMecanico == mec.id && std::strcmp(ord.estado, "CERRADA") == 0)
            {
                ordenesCerradas++;
                totalManoObra += static_cast<double>(ord.manoObra);
                totalRepuestos += calcularRepuestosOrden(ord);
            }
        }

        std::cout << mec.nombre << (mec.eliminado ? " (BAJA)" : "")
                  << " | " << ordenesCerradas
                  << " | " << totalManoObra
                  << " | " << totalRepuestos << "\n";
    }

    if (totalMecs == 0)
    {
        std::cout << "No hay mecanicos registrados.\n";
    }
}