#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "estructura.h"
#include <fstream>
#include <string>
#include <ctime>

const std::string RUTA_TALLER = "datos/taller.bin";
const std::string RUTA_VEHICULOS = "datos/vehiculos.bin";
const std::string RUTA_MECANICOS = "datos/mecanicos.bin";
const std::string RUTA_REPUESTOS = "datos/repuestos.bin";
const std::string RUTA_ORDENES = "datos/ordenes.bin";

bool inicializarSistemaArchivos();
bool inicializarArchivoHeader(const std::string& ruta);
bool leerHeader(const std::string& ruta, ArchivoHeader& headerOut);
bool actualizarHeader(const std::string& ruta, const ArchivoHeader& header);

bool existeTaller();
bool guardarTaller(const Taller& t);
bool leerTaller(Taller& tOut);

// Templates de persistencia para acceso aleatorio
template <typename T>
int guardarRegistro(const std::string& rutaEntidad, T& registro) {
    ArchivoHeader header;
    if (!leerHeader(rutaEntidad, header)) return -1;

    registro.id = header.proximoID;
    registro.eliminado = false;
    registro.fechaCreacion = std::time(nullptr);
    registro.fechaUltimaModificacion = std::time(nullptr);

    std::ofstream archivo(rutaEntidad, std::ios::binary | std::ios::app);
    if (!archivo.is_open()) return -1;

    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
    archivo.close();

    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID++;
    actualizarHeader(rutaEntidad, header);

    return registro.id;
}

template <typename T>
bool leerRegistroPorID(const std::string& rutaEntidad, int id, T& registroOut) {
    std::ifstream archivo(rutaEntidad, std::ios::binary);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    if (!leerHeader(rutaEntidad, header)) return false;

    long long posicion = sizeof(ArchivoHeader) + static_cast<long long>(id - 1) * sizeof(T);
    archivo.seekg(posicion, std::ios::beg);
    
    if (archivo.peek() == EOF) {
        archivo.close();
        return false;
    }

    archivo.read(reinterpret_cast<char*>(&registroOut), sizeof(T));
    archivo.close();
    return true;
}

template <typename T>
bool sobrescribirRegistro(const std::string& rutaEntidad, int id, const T& registroModificado) {
    std::fstream archivo(rutaEntidad, std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;

    long long posicion = sizeof(ArchivoHeader) + static_cast<long long>(id - 1) * sizeof(T);
    archivo.seekp(posicion, std::ios::beg);
    
    T regCopia = registroModificado;
    regCopia.fechaUltimaModificacion = std::time(nullptr);

    archivo.write(reinterpret_cast<const char*>(&regCopia), sizeof(T));
    archivo.close();
    return true;
}

template <typename T>
bool marcarComoEliminado(const std::string& rutaEntidad, int id) {
    T registro;
    if (!leerRegistroPorID(rutaEntidad, id, registro)) return false;
    if (registro.eliminado) return true;

    registro.eliminado = true;
    if (!sobrescribirRegistro(rutaEntidad, id, registro)) return false;

    ArchivoHeader header;
    if (!leerHeader(rutaEntidad, header)) return false;
    
    header.registrosActivos--;
    actualizarHeader(rutaEntidad, header);
    return true;
}

#endif