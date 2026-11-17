#include "persistencia.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

bool inicializarArchivoHeader(const std::string& ruta) {
    std::ifstream archivoPrueba(ruta, std::ios::binary);
    if (archivoPrueba.is_open()) {
        archivoPrueba.close();
        return true;
    }

    std::ofstream archivoSalida(ruta, std::ios::binary);
    if (!archivoSalida.is_open()) return false;

    ArchivoHeader headerInicial = {0, 1, 0, 1};
    archivoSalida.write(reinterpret_cast<const char*>(&headerInicial), sizeof(ArchivoHeader));
    archivoSalida.close();
    return true;
}

bool leerHeader(const std::string& ruta, ArchivoHeader& headerOut) {
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo.is_open()) return false;

    archivo.read(reinterpret_cast<char*>(&headerOut), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

bool actualizarHeader(const std::string& ruta, const ArchivoHeader& header) {
    std::fstream archivo(ruta, std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;

    archivo.seekp(0, std::ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

bool inicializarSistemaArchivos() {
    if (!fs::exists("datos")) {
        fs::create_directory("datos");
    }

    bool ok = true;
    ok &= inicializarArchivoHeader(RUTA_VEHICULOS);
    ok &= inicializarArchivoHeader(RUTA_MECANICOS);
    ok &= inicializarArchivoHeader(RUTA_REPUESTOS);
    ok &= inicializarArchivoHeader(RUTA_ORDENES);
    return ok;
}

bool existeTaller() {
    std::ifstream f(RUTA_TALLER, std::ios::binary);
    return f.is_open();
}

bool guardarTaller(const Taller& t) {
    std::ofstream f(RUTA_TALLER, std::ios::binary);
    if (!f.is_open()) return false;
    f.write(reinterpret_cast<const char*>(&t), sizeof(Taller));
    return true;
}

bool leerTaller(Taller& tOut) {
    std::ifstream f(RUTA_TALLER, std::ios::binary);
    if (!f.is_open()) return false;
    f.read(reinterpret_cast<char*>(&tOut), sizeof(Taller));
    return true;
}