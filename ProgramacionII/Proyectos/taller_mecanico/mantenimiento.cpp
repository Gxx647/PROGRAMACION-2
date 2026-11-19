#include "mantenimiento.h"
#include "persistencia.h"
#include "logica.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cstring>

namespace fs = std::filesystem;

std::string crearBackup()
{
    std::time_t ahora = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "backup_%Y-%m-%d_%H-%M", std::localtime(&ahora));

    std::string rutaBackup = std::string(buffer);
    fs::create_directory(rutaBackup);

    if (fs::exists("datos"))
    {
        for (const auto &entrada : fs::directory_iterator("datos"))
        {
            if (entrada.path().extension() == ".bin")
            {
                fs::path destino = fs::path(rutaBackup) / entrada.path().filename();
                fs::copy_file(entrada.path(), destino, fs::copy_options::overwrite_existing);
            }
        }
    }
    return rutaBackup;
}

namespace
{
    template <typename T>
    int contarRegistrosReales(const std::string &ruta, bool &ok)
    {
        ok = false;
        std::ifstream f(ruta, std::ios::binary);
        if (!f.is_open())
            return 0;

        f.seekg(0, std::ios::end);
        long long tamTotal = f.tellg();
        f.close();

        long long tamHeader = static_cast<long long>(sizeof(ArchivoHeader));
        if (tamTotal < tamHeader)
        {
            ok = false;
            return 0;
        }

        long long bytesDatos = tamTotal - tamHeader;
        if (bytesDatos % static_cast<long long>(sizeof(T)) != 0)
        {
            ok = false;
        }
        else
        {
            ok = true;
        }

        return static_cast<int>(bytesDatos / static_cast<long long>(sizeof(T)));
    }
}

void verificarIntegridad()
{
    std::cout << "\n=== VERIFICACION DE INTEGRIDAD DEL SISTEMA ===\n";

    int problemas = 0;
    int vehiculosVerificados = 0;
    int mecanicosVerificados = 0;
    int repuestosVerificados = 0;
    int ordenesVerificadas = 0;

    ArchivoHeader hVeh;
    int realesVeh = 0;
    int activosRealesVeh = 0;
    bool estructuraVehOk = false;

    if (!leerHeader(RUTA_VEHICULOS, hVeh))
    {
        std::cout << "ROTO: No se pudo leer el header de " << RUTA_VEHICULOS << "\n";
        problemas++;
    }
    else
    {
        realesVeh = contarRegistrosReales<Vehiculo>(RUTA_VEHICULOS, estructuraVehOk);
        for (int i = 1; i <= realesVeh; i++)
        {
            Vehiculo v;
            if (leerRegistroPorID(RUTA_VEHICULOS, i, v))
            {
                vehiculosVerificados++;
                if (!v.eliminado)
                    activosRealesVeh++;
            }
        }
        if (hVeh.cantidadRegistros != realesVeh)
        {
            std::cout << "ROTO: Header de vehiculos.bin indica " << hVeh.cantidadRegistros
                      << " registros; el archivo contiene " << realesVeh << "\n";
            problemas++;
        }
        if (hVeh.registrosActivos != activosRealesVeh)
        {
            std::cout << "ROTO: Header de vehiculos.bin indica " << hVeh.registrosActivos
                      << " activos; el archivo contiene " << activosRealesVeh << "\n";
            problemas++;
        }
    }

    ArchivoHeader hMec;
    int realesMec = 0;
    int activosRealesMec = 0;
    bool estructuraMecOk = false;

    if (!leerHeader(RUTA_MECANICOS, hMec))
    {
        std::cout << "ROTO: No se pudo leer el header de " << RUTA_MECANICOS << "\n";
        problemas++;
    }
    else
    {
        realesMec = contarRegistrosReales<Mecanico>(RUTA_MECANICOS, estructuraMecOk);
        for (int i = 1; i <= realesMec; i++)
        {
            Mecanico m;
            if (leerRegistroPorID(RUTA_MECANICOS, i, m))
            {
                mecanicosVerificados++;
                if (!m.eliminado)
                    activosRealesMec++;
            }
        }
        if (hMec.cantidadRegistros != realesMec)
        {
            std::cout << "ROTO: Header de mecanicos.bin indica " << hMec.cantidadRegistros
                      << " registros; el archivo contiene " << realesMec << "\n";
            problemas++;
        }
        if (hMec.registrosActivos != activosRealesMec)
        {
            std::cout << "ROTO: Header de mecanicos.bin indica " << hMec.registrosActivos
                      << " activos; el archivo contiene " << activosRealesMec << "\n";
            problemas++;
        }
    }

    ArchivoHeader hRep;
    int realesRep = 0;
    int activosRealesRep = 0;
    bool estructuraRepOk = false;

    if (!leerHeader(RUTA_REPUESTOS, hRep))
    {
        std::cout << "ROTO: No se pudo leer el header de " << RUTA_REPUESTOS << "\n";
        problemas++;
    }
    else
    {
        realesRep = contarRegistrosReales<Repuesto>(RUTA_REPUESTOS, estructuraRepOk);
        for (int i = 1; i <= realesRep; i++)
        {
            Repuesto r;
            if (leerRegistroPorID(RUTA_REPUESTOS, i, r))
            {
                repuestosVerificados++;
                if (!r.eliminado)
                    activosRealesRep++;
                if (r.stock < 0)
                {
                    std::cout << "ROTO: Repuesto ID " << r.id << " (" << r.codigo
                              << ") tiene stock negativo (" << r.stock << ")\n";
                    problemas++;
                }
            }
        }
        if (hRep.cantidadRegistros != realesRep)
        {
            std::cout << "ROTO: Header de repuestos.bin indica " << hRep.cantidadRegistros
                      << " registros; el archivo contiene " << realesRep << "\n";
            problemas++;
        }
        if (hRep.registrosActivos != activosRealesRep)
        {
            std::cout << "ROTO: Header de repuestos.bin indica " << hRep.registrosActivos
                      << " activos; el archivo contiene " << activosRealesRep << "\n";
            problemas++;
        }
    }

    ArchivoHeader hOrd;
    int realesOrd = 0;
    int activosRealesOrd = 0;
    bool estructuraOrdOk = false;

    if (!leerHeader(RUTA_ORDENES, hOrd))
    {
        std::cout << "ROTO: No se pudo leer el header de " << RUTA_ORDENES << "\n";
        problemas++;
    }
    else
    {
        realesOrd = contarRegistrosReales<OrdenServicio>(RUTA_ORDENES, estructuraOrdOk);
        for (int i = 1; i <= realesOrd; i++)
        {
            OrdenServicio o;
            if (leerRegistroPorID(RUTA_ORDENES, i, o))
            {
                ordenesVerificadas++;
                if (!o.eliminado)
                    activosRealesOrd++;

                bool esAbierta = (std::strcmp(o.estado, "ABIERTA") == 0);

                Vehiculo v;
                bool existeV = (o.idVehiculo > 0 && leerRegistroPorID(RUTA_VEHICULOS, o.idVehiculo, v));
                if (!existeV)
                {
                    std::cout << "ROTO: Orden #" << o.id << " apunta al vehiculo ID "
                              << o.idVehiculo << " (no existe en vehiculos.bin)\n";
                    problemas++;
                }
                else if (esAbierta && v.eliminado)
                {
                    std::cout << "ROTO: Orden ABIERTA #" << o.id << " apunta al vehiculo ID "
                              << o.idVehiculo << " que esta eliminado\n";
                    problemas++;
                }

                Mecanico m;
                bool existeM = (o.idMecanico > 0 && leerRegistroPorID(RUTA_MECANICOS, o.idMecanico, m));
                if (!existeM)
                {
                    std::cout << "ROTO: Orden #" << o.id << " apunta al mecanico ID "
                              << o.idMecanico << " (no existe en mecanicos.bin)\n";
                    problemas++;
                }
                else if (esAbierta && m.eliminado)
                {
                    std::cout << "ROTO: Orden ABIERTA #" << o.id << " apunta al mecanico ID "
                              << o.idMecanico << " que esta dado de baja\n";
                    problemas++;
                }

                int limiteLineas = o.totalLineas;
                if (limiteLineas > MAX_LINEAS_DETALLE)
                    limiteLineas = MAX_LINEAS_DETALLE;
                for (int j = 0; j < limiteLineas; j++)
                {
                    if (o.lineasDetalle[j].activo)
                    {
                        Repuesto r;
                        int idRep = o.lineasDetalle[j].idRepuesto;
                        if (idRep <= 0 || !leerRegistroPorID(RUTA_REPUESTOS, idRep, r))
                        {
                            std::cout << "ROTO: Orden #" << o.id << " linea " << (j + 1)
                                      << " apunta al repuesto ID " << idRep
                                      << " (no existe en repuestos.bin)\n";
                            problemas++;
                        }
                    }
                }
            }
        }
        if (hOrd.cantidadRegistros != realesOrd)
        {
            std::cout << "ROTO: Header de ordenes.bin indica " << hOrd.cantidadRegistros
                      << " registros; el archivo contiene " << realesOrd << "\n";
            problemas++;
        }
        if (hOrd.registrosActivos != activosRealesOrd)
        {
            std::cout << "ROTO: Header de ordenes.bin indica " << hOrd.registrosActivos
                      << " activos; el archivo contiene " << activosRealesOrd << "\n";
            problemas++;
        }
    }

    (void)estructuraVehOk;
    (void)estructuraMecOk;
    (void)estructuraRepOk;
    (void)estructuraOrdOk;

    std::cout << "\nREPORTE DE INTEGRIDAD DEL SISTEMA\n";
    std::cout << "Vehiculos verificados : " << vehiculosVerificados << "\n";
    std::cout << "Mecanicos verificados: " << mecanicosVerificados << "\n";
    std::cout << "Repuestos verificados: " << repuestosVerificados << "\n";
    std::cout << "Ordenes verificadas : " << ordenesVerificadas << "\n";
    std::cout << "Problemas encontrados : " << problemas << "\n";
    std::cout << "Estado: " << (problemas == 0 ? "SISTEMA INTEGRO" : "CON INCONSISTENCIAS") << "\n";
}