#include "mantenimiento.h"
#include "persistencia.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

std::string crearBackup() {
    
    std::time_t ahora = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "backup_%Y-%m-%d_%H-%M", std::localtime(&ahora));
    
    std::string rutaBackup = std::string(buffer);
    fs::create_directory(rutaBackup);

    
    if (fs::exists("datos")) {
        for (const auto& entrada : fs::directory_iterator("datos")) {
            if (entrada.path().extension() == ".bin") {
                fs::path destino = fs::path(rutaBackup) / entrada.path().filename();
                fs::copy_file(entrada.path(), destino, fs::copy_options::overwrite_existing);
            }
        }
    }
    return rutaBackup;
}

void verificarIntegridad() {

    std::cout << "Iniciando verificacion de integridad...\n";
}