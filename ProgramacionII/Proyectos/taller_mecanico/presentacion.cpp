#include "presentacion.h"
#include "logica.h"
#include "persistencia.h"
#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

namespace {
    
    bool confirmarDatos() {
        char resp[20];
        while (true) {
            cout << "\nLos datos estan correctos? (1 = Si, continuar | 2 = No, escribirlos de nuevo): ";
            cin.getline(resp, 20);
            if (cin.eof()) return false;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            for (int i = 0; resp[i] != '\0'; i++) {
                resp[i] = static_cast<char>(tolower(static_cast<unsigned char>(resp[i])));
            }
            if (strcmp(resp, "1") == 0 || strcmp(resp, "s") == 0 || strcmp(resp, "si") == 0) return true;
            if (strcmp(resp, "2") == 0 || strcmp(resp, "n") == 0 || strcmp(resp, "no") == 0) return false;
            cout << "[ERROR]: Responda 1 (Si) o 2 (No).\n";
        }
    }

    int contarOrdenesAbiertasMecanico(int idMecanico, bool& okArchivo) {
        okArchivo = true;
        if (idMecanico <= 0) return 0;

        ArchivoHeader hOrd;
        if (!leerHeader(RUTA_ORDENES, hOrd)) {
            okArchivo = false;
            return 0;
        }

        int abiertas = 0;
        for (int i = 1; i <= hOrd.cantidadRegistros; i++) {
            OrdenServicio o;
            if (leerRegistroPorID(RUTA_ORDENES, i, o) && !o.eliminado) {
                if (o.idMecanico == idMecanico && strcmp(o.estado, "ABIERTA") == 0) {
                    abiertas++;
                }
            }
        }
        return abiertas;
    }
}

void mostrarVehiculo(const Vehiculo &v) {
    cout << "ID: " << v.id
         << " | Placa: " << v.placa
         << " | " << v.marca << " " << v.modelo
         << " (" << v.anio << ")"
         << " | Km: " << v.kilometraje
         << " | Propietario: " << v.nombrePropietario << "\n";
}

void mostrarMecanico(const Mecanico &m) {
    bool okOrd = true;
    int abiertas = contarOrdenesAbiertasMecanico(m.id, okOrd);

    cout << "ID: " << m.id
         << " | " << m.nombre
         << " | Cedula: " << m.cedula
         << " | Especialidad: " << m.especialidad
         << " | Ordenes ABIERTAS: " << (okOrd ? abiertas : -1);
    if (!okOrd) {
        cout << " (error leyendo " << RUTA_ORDENES << ")";
    }
    if (m.eliminado) {
        cout << "  [BAJA]";
    }
    cout << "\n";
}

void mostrarRepuesto(const Repuesto &r) {
    cout << "ID: " << r.id
         << " | Cod: " << r.codigo
         << " | " << r.nombre
         << " | Marca: " << r.marca
         << " | Precio: $" << r.precio
         << " | Stock: " << r.stock;
    if (r.stock <= r.stockMinimo) {
        cout << "  [BAJO STOCK]";
    }
    cout << "\n";
}

void mostrarOrden(const OrdenServicio &o) {
    Vehiculo v;
    Mecanico m;
    bool okV = leerRegistroPorID(RUTA_VEHICULOS, o.idVehiculo, v);
    bool okM = leerRegistroPorID(RUTA_MECANICOS, o.idMecanico, m);

    double total = o.manoObra;
    for (int i = 0; i < o.totalLineas; i++) {
        if (o.lineasDetalle[i].activo) {
            total += o.lineasDetalle[i].cantidad * o.lineasDetalle[i].precioHistorico;
        }
    }

    cout << "Orden #" << o.id
         << " | Estado: " << o.estado
         << " | Vehiculo: " << (okV ? v.placa : "???")
         << " | Mecanico: " << (okM ? m.nombre : "???")
         << " | Apertura: " << o.fechaApertura
         << " | Lineas: " << o.totalLineas
         << " | Total: $" << total << "\n";
}

void menuRegistrarVehiculo() {
    char placa[10], marca[50], modelo[50];
    char nombreProp[100], cedulaProp[20], telefonoProp[20];
    int anio = 0, km = 0;
    bool confirmado = false;

    cout << "\n--- REGISTRO DE NUEVO VEHICULO ---\n";
    while (!confirmado) {
        cout << "Placa: ";                    cin.getline(placa, 10);
        cout << "Marca: ";                    cin.getline(marca, 50);
        cout << "Modelo: ";                   cin.getline(modelo, 50);
        cout << "Anio (1950-2027): ";         cin >> anio;
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Kilometraje: ";              cin >> km;
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Nombre del propietario: ";   cin.getline(nombreProp, 100);
        cout << "Cedula del propietario: ";   cin.getline(cedulaProp, 20);
        cout << "Telefono del propietario: "; cin.getline(telefonoProp, 20);

        cout << "\n--- RESUMEN DEL VEHICULO ---\n";
        cout << "Placa: " << placa << " | " << marca << " " << modelo << " (" << anio << ") | Km: " << km << "\n";
        cout << "Propietario: " << nombreProp << " | Cedula: " << cedulaProp << " | Telefono: " << telefonoProp << "\n";

        confirmado = confirmarDatos();
        if (!confirmado) {
            if (cin.eof()) return;
            cout << "\nVuelva a escribir los datos del vehiculo.\n";
        }
    }

    int res = registrarVehiculoLogica(placa, marca, modelo, anio, km, nombreProp, cedulaProp, telefonoProp);
    if (res > 0) {
        cout << "Vehiculo registrado con ID #" << res << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: Ya existe un vehiculo ACTIVO con esa placa.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El anio debe estar entre 1950 y 2027.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El kilometraje no puede ser negativo.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar el vehiculo en " << RUTA_VEHICULOS << ".\n";
    }
}

void menuMostrarVehiculos() {
    cout << "\n--- LISTA DE VEHICULOS ACTIVOS ---\n";
    int totalActivos = cantidadVehiculosActivosLogica();
    if (totalActivos >= 0) {
        cout << "Total de vehiculos activos: " << totalActivos << " (leido del header de " << RUTA_VEHICULOS << ")\n";
    }
    ArchivoHeader h;
    if (!leerHeader(RUTA_VEHICULOS, h)) {
        cout << "[ERROR]: No se pudo leer " << RUTA_VEHICULOS << ".\n";
        return;
    }

    int mostrados = 0;
    for (int i = 1; i <= h.cantidadRegistros; i++) {
        Vehiculo v;
        if (leerRegistroPorID(RUTA_VEHICULOS, i, v) && !v.eliminado) {
            mostrarVehiculo(v);
            mostrados++;
        }
    }
    if (mostrados == 0) {
        cout << "No hay vehiculos activos registrados.\n";
    }
}

void menuActualizarVehiculo() {
    int id = 0, anio = 0;
    char marca[50], modelo[50], nombreProp[100], cedulaProp[20], telefonoProp[20];
    bool confirmado = false;

    cout << "\n--- ACTUALIZAR VEHICULO ---\n";
    menuMostrarVehiculos();
    while (!confirmado) {
        cout << "\nID del vehiculo a actualizar: ";
        cin >> id; cin.clear(); cin.ignore(10000, '\n');
        cout << "Nueva marca: ";
        cin.getline(marca, 50);
        cout << "Nuevo modelo: ";
        cin.getline(modelo, 50);
        cout << "Nuevo anio (1950-2027): ";
        cin >> anio; cin.clear(); cin.ignore(10000, '\n');
        cout << "Nombre del propietario: ";
        cin.getline(nombreProp, 100);
        cout << "Cedula del propietario: ";
        cin.getline(cedulaProp, 20);
        cout << "Telefono del propietario: ";
        cin.getline(telefonoProp, 20);

        Vehiculo actual;
        long long posicion = 0;
        bool existe = (obtenerVehiculoPorIDLogica(id, actual, posicion) == 1);
        cout << "\n--- RESUMEN DE LA ACTUALIZACION ---\n";
        cout << "Vehiculo a actualizar: " << (existe ? actual.placa : "(no existe o esta eliminado)") << "\n";
        cout << "Nuevos datos: " << marca << " " << modelo << " (" << anio << ")\n";
        cout << "Propietario: " << nombreProp << " | Cedula: " << cedulaProp << " | Telefono: " << telefonoProp << "\n";

        confirmado = confirmarDatos();
        if (!confirmado) {
            if (cin.eof()) return;
            cout << "\nVuelva a escribir los datos.\n";
        }
    }

    int res = actualizarVehiculoLogica(id, marca, modelo, anio, nombreProp, cedulaProp, telefonoProp);
    if (res == 1) {
        cout << "Vehiculo actualizado correctamente en " << RUTA_VEHICULOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el vehiculo no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El vehiculo esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: Anio fuera de rango (1950-2027).\n";
    } else {
        cout << "[ERROR]: Fallo al actualizar el vehiculo en " << RUTA_VEHICULOS << ".\n";
    }
}

void menuEliminarVehiculo() {
    int id;
    cout << "\n--- ELIMINAR VEHICULO (BORRADO LOGICO) ---\n";
    menuMostrarVehiculos();
    cout << "\nID del vehiculo a eliminar: ";
    cin >> id; cin.clear(); cin.ignore(10000, '\n');

    int res = eliminarVehiculoLogica(id);
    if (res == 1) {
        cout << "Vehiculo marcado como eliminado en " << RUTA_VEHICULOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el vehiculo no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El vehiculo ya estaba eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: No se puede eliminar; el vehiculo tiene ordenes registradas.\n";
    } else {
        cout << "[ERROR]: Fallo al eliminar el vehiculo en " << RUTA_VEHICULOS << " o leer " << RUTA_ORDENES << ".\n";
    }
}

void menuRegistrarMecanico() {
    Mecanico nuevo;
    bool confirmado = false;

    cout << "\n--- REGISTRAR NUEVO MECANICO ---\n";
    while (!confirmado) {
        memset(&nuevo, 0, sizeof(Mecanico));
        cout << "Nombre: "; cin.getline(nuevo.nombre, 100);
        cout << "Cedula: "; cin.getline(nuevo.cedula, 20);
        cout << "Especialidad (Motor/Frenos/Electricidad/Suspension/General): "; cin.getline(nuevo.especialidad, 20);
        cout << "Fecha de ingreso (YYYY-MM-DD): "; cin.getline(nuevo.fechaIngreso, 11);

        cout << "\n--- RESUMEN DEL MECANICO ---\n";
        cout << "Nombre: " << nuevo.nombre << " | Cedula: " << nuevo.cedula
             << " | Especialidad: " << nuevo.especialidad << " | Ingreso: " << nuevo.fechaIngreso << "\n";

        confirmado = confirmarDatos();
        if (!confirmado) {
            if (cin.eof()) return;
            cout << "\nVuelva a escribir los datos del mecanico.\n";
        }
    }

    int res = registrarMecanico(nuevo);
    if (res > 0) {
        cout << "Mecanico registrado con ID #" << res << " en " << RUTA_MECANICOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: Ya existe un mecanico con esa cedula.\n";
    } else if (res == -3) {
        cout << "[ERROR]: Especialidad no permitida.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar el mecanico en " << RUTA_MECANICOS << ".\n";
    }
}

void menuMostrarMecanicos() {
    cout << "\n--- LISTA DE MECANICOS (activos y dados de baja) ---\n";
    ArchivoHeader h;
    if (!leerHeader(RUTA_MECANICOS, h)) {
        cout << "[ERROR]: No se pudo leer " << RUTA_MECANICOS << ".\n";
        return;
    }

    for (int i = 1; i <= h.cantidadRegistros; i++) {
        Mecanico m;
        if (leerRegistroPorID(RUTA_MECANICOS, i, m)) {
            mostrarMecanico(m);
        }
    }
}

void menuDarDeBajaMecanico() {
    int id;
    cout << "\n--- DAR DE BAJA MECANICO ---\n";
    menuMostrarMecanicos();
    cout << "\nID del mecanico a dar de baja: ";
    cin >> id; cin.clear(); cin.ignore(10000, '\n');

    int res = darDeBajaMecanicoLogica(id);
    if (res == 1) {
        cout << "Mecanico dado de baja correctamente en " << RUTA_MECANICOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el mecanico no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El mecanico ya estaba dado de baja.\n";
    } else if (res == -4) {
        cout << "[ERROR]: No se puede dar de baja; tiene ordenes ABIERTAS.\n";
    } else {
        cout << "[ERROR]: Fallo al dar de baja en " << RUTA_MECANICOS << " o leer " << RUTA_ORDENES << ".\n";
    }
}

void menuRegistrarRepuesto() {
    char codigo[20], nombre[100], marca[50];
    float precio = 0;
    int stock = 0, stockMinimo = 0;
    bool confirmado = false;

    cout << "\n--- REGISTRO DE NUEVO REPUESTO ---\n";
    while (!confirmado) {
        cout << "Codigo: ";             cin.getline(codigo, 20);
        cout << "Nombre: ";             cin.getline(nombre, 100);
        cout << "Marca: ";              cin.getline(marca, 50);
        cout << "Precio unitario ($): "; cin >> precio;
        cout << "Stock inicial: ";      cin >> stock;
        cout << "Stock minimo: ";       cin >> stockMinimo;
        cin.clear(); cin.ignore(10000, '\n');

        cout << "\n--- RESUMEN DEL REPUESTO ---\n";
        cout << "Codigo: " << codigo << " | " << nombre << " | Marca: " << marca << "\n";
        cout << "Precio: $" << precio << " | Stock inicial: " << stock << " | Stock minimo: " << stockMinimo << "\n";

        confirmado = confirmarDatos();
        if (!confirmado) {
            if (cin.eof()) return;
            cout << "\nVuelva a escribir los datos del repuesto.\n";
        }
    }

    int res = registrarRepuestoLogica(codigo, nombre, marca, precio, stock, stockMinimo);
    if (res > 0) {
        cout << "Repuesto registrado con ID #" << res << " en " << RUTA_REPUESTOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: Ya existe un repuesto ACTIVO con ese codigo.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El precio debe ser mayor a 0.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El stock no puede ser negativo.\n";
    } else if (res == -5) {
        cout << "[ERROR]: El stock minimo no puede ser negativo.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar el repuesto en " << RUTA_REPUESTOS << ".\n";
    }
}

void menuMostrarRepuestos() {
    cout << "\n--- INVENTARIO DE REPUESTOS ACTIVOS ---\n";
    int totalRepActivos = cantidadRepuestosActivosLogica();
    if (totalRepActivos >= 0) {
        cout << "Total de repuestos activos: " << totalRepActivos << " (leido del header de " << RUTA_REPUESTOS << ")\n";
    }
    ArchivoHeader h;
    if (!leerHeader(RUTA_REPUESTOS, h)) {
        cout << "[ERROR]: No se pudo leer " << RUTA_REPUESTOS << ".\n";
        return;
    }

    int mostrados = 0;
    for (int i = 1; i <= h.cantidadRegistros; i++) {
        Repuesto r;
        if (leerRegistroPorID(RUTA_REPUESTOS, i, r) && !r.eliminado) {
            mostrarRepuesto(r);
            mostrados++;
        }
    }
    if (mostrados == 0) {
        cout << "No hay repuestos activos.\n";
    }
}

void menuEntradaMercancia() {
    int idRep, cantidad;
    cout << "\n--- ENTRADA DE MERCANCIA ---\n";
    menuMostrarRepuestos();
    cout << "\nID del repuesto: ";
    cin >> idRep;
    cout << "Cantidad a ingresar: ";
    cin >> cantidad; cin.clear(); cin.ignore(10000, '\n');

    int res = entradaMercanciaLogica(idRep, cantidad);
    if (res == 1) {
        cout << "Stock actualizado correctamente en " << RUTA_REPUESTOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el repuesto no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El repuesto esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: La cantidad debe ser mayor a 0.\n";
    } else {
        cout << "[ERROR]: Fallo al actualizar el stock en " << RUTA_REPUESTOS << ".\n";
    }
}

void menuCambiarPrecioRepuesto() {
    int idRep;
    float nuevoPrecio;
    cout << "\n--- CAMBIAR PRECIO DE REPUESTO ---\n";
    menuMostrarRepuestos();
    cout << "\nID del repuesto: ";
    cin >> idRep;
    cout << "Nuevo precio ($): ";
    cin >> nuevoPrecio; cin.clear(); cin.ignore(10000, '\n');

    int res = cambiarPrecioRepuestoLogica(idRep, nuevoPrecio);
    if (res == 1) {
        cout << "Precio actualizado en " << RUTA_REPUESTOS << " (las ordenes ya registradas conservan su precio historico).\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el repuesto no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El repuesto esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El precio debe ser mayor a 0.\n";
    } else {
        cout << "[ERROR]: Fallo al cambiar el precio en " << RUTA_REPUESTOS << ".\n";
    }
}

void menuEliminarRepuesto() {
    int idRep;
    cout << "\n--- ELIMINAR REPUESTO (BORRADO LOGICO) ---\n";
    menuMostrarRepuestos();
    cout << "\nID del repuesto a eliminar: ";
    cin >> idRep; cin.clear(); cin.ignore(10000, '\n');

    int res = eliminarRepuestoLogica(idRep);
    if (res == 1) {
        cout << "Repuesto eliminado logicamente en " << RUTA_REPUESTOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el repuesto no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El repuesto ya estaba eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: No se puede eliminar; el repuesto aparece en alguna orden.\n";
    } else {
        cout << "[ERROR]: Fallo al eliminar en " << RUTA_REPUESTOS << " o leer " << RUTA_ORDENES << ".\n";
    }
}

void menuCrearOrden() {
    char fecha[11], falla[200];
    int km = 0, idVehiculo = 0, idMecanico = 0;
    bool confirmado = false;

    cout << "\n--- CREAR NUEVA ORDEN DE SERVICIO ---\n";
    menuMostrarVehiculos();
    menuMostrarMecanicos();

    while (!confirmado) {
        cout << "\nFecha de apertura (YYYY-MM-DD): "; cin.getline(fecha, 11);
        cout << "Descripcion de la falla: ";          cin.getline(falla, 200);
        cout << "Kilometraje de ingreso: ";           cin >> km;
        cin.clear(); cin.ignore(10000, '\n');
        cout << "ID del vehiculo: ";                  cin >> idVehiculo;
        cout << "ID del mecanico: ";                  cin >> idMecanico;
        cin.clear(); cin.ignore(10000, '\n');

        Vehiculo veh;
        Mecanico mec;
        long long posicion = 0;
        bool okV = (obtenerVehiculoPorIDLogica(idVehiculo, veh, posicion) == 1);
        bool okM = (obtenerMecanicoPorIDLogica(idMecanico, mec) == 1);
        cout << "\n--- RESUMEN DE LA ORDEN ---\n";
        cout << "Fecha: " << fecha << " | Km de ingreso: " << km << "\n";
        cout << "Falla: " << falla << "\n";
        cout << "Vehiculo: " << (okV ? veh.placa : "(no existe o esta eliminado)")
             << " | Mecanico: " << (okM ? mec.nombre : "(no existe o esta dado de baja)") << "\n";

        confirmado = confirmarDatos();
        if (!confirmado) {
            if (cin.eof()) return;
            cout << "\nVuelva a escribir los datos de la orden.\n";
        }
    }

    int res = crearOrdenServicioLogica(fecha, falla, km, idVehiculo, idMecanico);
    if (res > 0) {
        cout << "Orden creada con ID #" << res << " en " << RUTA_ORDENES << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: El vehiculo no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El vehiculo esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El mecanico no existe.\n";
    } else if (res == -5) {
        cout << "[ERROR]: El mecanico esta dado de baja.\n";
    } else if (res == -6) {
        cout << "[ERROR]: El vehiculo ya tiene una orden ABIERTA.\n";
    } else if (res == -7) {
        cout << "[ERROR]: El mecanico ya tiene 3 ordenes ABIERTAS.\n";
    } else if (res == -8) {
        cout << "[ERROR]: El kilometraje de ingreso es menor al del vehiculo.\n";
    } else {
        cout << "[ERROR]: Fallo al escribir en " << RUTA_ORDENES << " o actualizar " << RUTA_VEHICULOS << ".\n";
    }
}

void menuAgregarRepuesto() {
    int idOrden, idRepuesto, cantidad;

    cout << "\n--- AGREGAR REPUESTO A ORDEN ---\n";
    menuMostrarOrdenes();
    menuMostrarRepuestos();

    cout << "\nID de la orden: "; cin >> idOrden;
    cout << "ID del repuesto: ";  cin >> idRepuesto;
    cout << "Cantidad: ";         cin >> cantidad;
    cin.clear(); cin.ignore(10000, '\n');

    int res = agregarRepuestoAOrdenLogica(idOrden, idRepuesto, cantidad);
    if (res == 1) {
        cout << "Repuesto agregado exitosamente. Stock actualizado en " << RUTA_REPUESTOS << ".\n";
    } else if (res == -3) {
        cout << "[ERROR]: No existe la orden.\n";
    } else if (res == -4) {
        OrdenServicio ordActual;
        if (obtenerOrdenPorIDLogica(idOrden, ordActual) == 1) {
            cout << "[ERROR]: La orden no esta ABIERTA (estado actual: " << ordActual.estado << ").\n";
        } else {
            cout << "[ERROR]: La orden no esta ABIERTA.\n";
        }
    } else if (res == -5) {
        cout << "[ERROR]: No existe el repuesto (o esta eliminado).\n";
    } else if (res == -6) {
        cout << "[ERROR]: La cantidad debe ser mayor a 0.\n";
    } else if (res == -7) {
        cout << "[ERROR]: Stock insuficiente.\n";
    } else if (res == -8) {
        cout << "[ERROR]: La orden ya tiene 15 lineas de detalle (limite alcanzado).\n";
    } else {
        cout << "[ERROR]: Fallo al actualizar " << RUTA_ORDENES << " o " << RUTA_REPUESTOS << ".\n";
    }
}

void menuQuitarRepuesto() {
    int idOrden, idRep;
    cout << "\n--- QUITAR REPUESTO DE ORDEN ---\n";
    menuMostrarOrdenes();
    cout << "\nID de la orden: ";
    cin >> idOrden;
    cout << "ID del repuesto a quitar: ";
    cin >> idRep; cin.clear(); cin.ignore(10000, '\n');

    int res = quitarRepuestoDeOrdenLogica(idOrden, idRep);
    if (res == 1) {
        cout << "Linea retirada y stock devuelto en " << RUTA_REPUESTOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: La orden no existe.\n";
    } else if (res == -3) {
        OrdenServicio ordActual;
        if (obtenerOrdenPorIDLogica(idOrden, ordActual) == 1) {
            cout << "[ERROR]: Solo se pueden quitar repuestos en ordenes ABIERTAS (estado actual: " << ordActual.estado << ").\n";
        } else {
            cout << "[ERROR]: Solo se pueden quitar repuestos en ordenes ABIERTAS.\n";
        }
    } else if (res == -4) {
        cout << "[ERROR]: El repuesto no figura en la orden.\n";
    } else {
        cout << "[ERROR]: Fallo al actualizar " << RUTA_ORDENES << " o " << RUTA_REPUESTOS << ".\n";
    }
}

void menuCerrarOrden() {
    int idOrden;
    float manoObra;

    cout << "\n--- CERRAR ORDEN DE SERVICIO ---\n";
    menuMostrarOrdenes();

    cout << "\nID de la orden a cerrar: "; cin >> idOrden;
    cout << "Mano de obra ($): ";        cin >> manoObra;
    cin.clear(); cin.ignore(10000, '\n');

    double costoRepuestos = 0.0;
    double totalPagar = 0.0;
    OrdenServicio ordenEn;

    int res = cerrarOrdenServicioLogica(idOrden, manoObra, costoRepuestos, totalPagar, ordenEn);
    if (res == 1) {
        Vehiculo vehFactura;
        Mecanico mecFactura;
        bool okVeh = leerRegistroPorID(RUTA_VEHICULOS, ordenEn.idVehiculo, vehFactura);
        bool okMec = leerRegistroPorID(RUTA_MECANICOS, ordenEn.idMecanico, mecFactura);

        cout << "\n=========== FACTURA ===========\n";
        cout << "Orden #"       << ordenEn.id << "\n";
        cout << "Vehiculo     : " << (okVeh ? vehFactura.placa : "No disponible")
             << (okVeh ? (string(" - ") + vehFactura.marca + " " + vehFactura.modelo) : "") << "\n";
        cout << "Mecanico     : " << (okMec ? mecFactura.nombre : "No disponible")
             << (okMec && mecFactura.eliminado ? " [BAJA]" : "") << "\n";
        cout << "Falla        : " << ordenEn.descripcionFalla << "\n";
        cout << "Fecha cierre : " << ordenEn.fechaCierre << "\n";
        cout << "--------------------------------\n";
        cout << "Mano de obra : $" << ordenEn.manoObra << "\n";
        cout << "Repuestos    : $" << costoRepuestos << "\n";
        cout << "--------------------------------\n";
        cout << "TOTAL        : $" << totalPagar << "\n";
        cout << "================================\n";
    } else if (res == -2) {
        cout << "[ERROR]: No existe la orden.\n";
    } else if (res == -3) {
        cout << "[ERROR]: La orden no esta ABIERTA (estado actual: " << ordenEn.estado << ").\n";
    } else if (res == -4) {
        cout << "[ERROR]: La mano de obra no puede ser negativa.\n";
    } else if (res == -5) {
        cout << "[ERROR]: La orden no tiene repuestos y la mano de obra es 0.\n";
    } else {
        cout << "[ERROR]: Fallo al cerrar la orden en " << RUTA_ORDENES << ".\n";
    }
}

void menuAnularOrden() {
    int idOrden;
    cout << "\n--- ANULAR ORDEN DE SERVICIO ---\n";
    menuMostrarOrdenes();
    cout << "\nID de la orden a anular: ";
    cin >> idOrden; cin.clear(); cin.ignore(10000, '\n');

    OrdenServicio ordenOut;
    int res = anularOrdenLogica(idOrden, ordenOut);
    if (res == 1) {
        cout << "Orden #" << ordenOut.id << " ANULADA en " << RUTA_ORDENES << ". Se devolvio el stock en " << RUTA_REPUESTOS << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: La orden no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: No se puede anular una orden en estado " << ordenOut.estado << ".\n";
    } else {
        cout << "[ERROR]: Fallo al anular en " << RUTA_ORDENES << " o actualizar " << RUTA_REPUESTOS << ".\n";
    }
}

void menuMostrarOrdenes() {
    cout << "\n--- LISTA DE ORDENES DE SERVICIO ---\n";
    ArchivoHeader h;
    if (!leerHeader(RUTA_ORDENES, h)) {
        cout << "[ERROR]: No se pudo leer " << RUTA_ORDENES << ".\n";
        return;
    }

    int mostradas = 0;
    for (int i = 1; i <= h.cantidadRegistros; i++) {
        OrdenServicio o;
        if (leerRegistroPorID(RUTA_ORDENES, i, o) && !o.eliminado) {
            mostrarOrden(o);
            mostradas++;
        }
    }
    if (mostradas == 0) {
        cout << "No hay ordenes registradas.\n";
    }
}

void mostrarDetalleOrden(const OrdenServicio &o) {
    cout << "  Falla: " << o.descripcionFalla << " | Km ingreso: " << o.kilometrajeIngreso << "\n";
    if (strcmp(o.estado, "CERRADA") == 0) {
        cout << "  Fecha de cierre: " << o.fechaCierre << "\n";
    }
    if (o.totalLineas == 0) {
        cout << "  (sin lineas de detalle)\n";
    }
    for (int i = 0; i < o.totalLineas; i++) {
        if (o.lineasDetalle[i].activo) {
            cout << "  " << (i + 1) << ". [" << o.lineasDetalle[i].codigoRepuesto << "] "
                 << o.lineasDetalle[i].nombreRepuesto
                 << " x" << o.lineasDetalle[i].cantidad
                 << " @ $" << o.lineasDetalle[i].precioHistorico
                 << " = $" << o.lineasDetalle[i].cantidad * o.lineasDetalle[i].precioHistorico << "\n";
        }
    }
    cout << "  Mano de obra: $" << o.manoObra << "\n";
}

void menuBuscarVehiculoPorID() {
    int id;
    cout << "\n--- BUSCAR VEHICULO POR ID ---\n";
    cout << "ID del vehiculo: ";
    cin >> id; cin.clear(); cin.ignore(10000, '\n');

    Vehiculo veh;
    long long posicion = -1;
    int res = obtenerVehiculoPorIDLogica(id, veh, posicion);

    if (res == 1 || res == -3) {
        cout << "[DEBUG] Vehiculo ID " << id << " leido directamente en el byte " << posicion
             << "  (" << sizeof(ArchivoHeader) << " + (" << id << " - 1) * " << sizeof(Vehiculo) << ")\n";
    }
    if (res == 1) {
        mostrarVehiculo(veh);
    } else if (res == -3) {
        cout << "El vehiculo " << veh.placa << " (" << veh.marca << " " << veh.modelo
             << ", propietario " << veh.nombrePropietario << ") fue ELIMINADO.\n";
    } else if (res == -2) {
        cout << "[ERROR]: No existe ningun vehiculo con ese ID.\n";
    } else {
        cout << "[ERROR]: No se pudo leer " << RUTA_VEHICULOS << ".\n";
    }
}

void menuBuscarVehiculoPorPlaca() {
    char placa[10];
    cout << "\n--- BUSCAR VEHICULO POR PLACA ---\n";
    cout << "Placa exacta: ";
    cin.getline(placa, 10);

    Vehiculo veh;
    int res = obtenerVehiculoPorPlacaLogica(placa, veh);
    if (res == 1) {
        mostrarVehiculo(veh);
    } else if (res == -2) {
        cout << "No hay ningun vehiculo ACTIVO con esa placa.\n";
    } else {
        cout << "[ERROR]: No se pudo leer " << RUTA_VEHICULOS << ".\n";
    }
}

void menuBuscarVehiculosPorPropietario() {
    char texto[100];
    cout << "\n--- BUSCAR VEHICULOS POR PROPIETARIO ---\n";
    cout << "Nombre (o parte del nombre): ";
    cin.getline(texto, 100);

    Vehiculo resultados[MAX_RESULTADOS_BUSQUEDA];
    int cantidad = 0;
    bool hayMas = false;
    int res = buscarVehiculosPorPropietarioLogica(texto, resultados, cantidad, hayMas);

    if (res == -2) {
        cout << "[ERROR]: Debe escribir al menos un caracter.\n";
    } else if (res != 1) {
        cout << "[ERROR]: No se pudo leer " << RUTA_VEHICULOS << ".\n";
    } else if (cantidad == 0) {
        cout << "No se encontraron vehiculos activos con ese propietario.\n";
    } else {
        for (int i = 0; i < cantidad; i++) {
            mostrarVehiculo(resultados[i]);
        }
        if (hayMas) {
            cout << "(Hay mas coincidencias: se muestran solo las primeras " << MAX_RESULTADOS_BUSQUEDA
                 << ". Escriba un nombre mas especifico.)\n";
        }
    }
}

void menuBuscarMecanicoPorID() {
    int id;
    cout << "\n--- BUSCAR MECANICO POR ID ---\n";
    cout << "ID del mecanico: ";
    cin >> id; cin.clear(); cin.ignore(10000, '\n');

    Mecanico mec;
    int res = obtenerMecanicoPorIDLogica(id, mec);
    if (res == 1 || res == -3) {
        mostrarMecanico(mec);
    } else {
        cout << "[ERROR]: No existe ningun mecanico con ese ID.\n";
    }
}

void menuBuscarRepuestoPorID() {
    int id;
    cout << "\n--- BUSCAR REPUESTO POR ID ---\n";
    cout << "ID del repuesto: ";
    cin >> id; cin.clear(); cin.ignore(10000, '\n');

    Repuesto rep;
    int res = obtenerRepuestoPorIDLogica(id, rep);
    if (res == 1) {
        mostrarRepuesto(rep);
    } else {
        cout << "[ERROR]: No existe un repuesto activo con ese ID.\n";
    }
}

void menuBuscarRepuestos() {
    char texto[100];
    cout << "\n--- BUSCAR REPUESTOS POR CODIGO O NOMBRE ---\n";
    cout << "Texto a buscar: ";
    cin.getline(texto, 100);

    Repuesto resultados[MAX_RESULTADOS_BUSQUEDA];
    int cantidad = 0;
    bool hayMas = false;
    int res = buscarRepuestosLogica(texto, resultados, cantidad, hayMas);

    if (res == -2) {
        cout << "[ERROR]: Debe escribir al menos un caracter.\n";
    } else if (res != 1) {
        cout << "[ERROR]: No se pudo leer " << RUTA_REPUESTOS << ".\n";
    } else if (cantidad == 0) {
        cout << "No se encontraron repuestos activos con ese codigo o nombre.\n";
    } else {
        for (int i = 0; i < cantidad; i++) {
            mostrarRepuesto(resultados[i]);
        }
        if (hayMas) {
            cout << "(Hay mas coincidencias: se muestran solo las primeras " << MAX_RESULTADOS_BUSQUEDA
                 << ". Escriba un texto mas especifico.)\n";
        }
    }
}

void menuBuscarOrdenPorID() {
    int id;
    cout << "\n--- BUSCAR ORDEN POR ID ---\n";
    cout << "ID de la orden: ";
    cin >> id; cin.clear(); cin.ignore(10000, '\n');

    OrdenServicio orden;
    int res = obtenerOrdenPorIDLogica(id, orden);
    if (res == 1) {
        mostrarOrden(orden);
        mostrarDetalleOrden(orden);
    } else {
        cout << "[ERROR]: No existe ninguna orden con ese ID.\n";
    }
}

void menuListarOrdenesPorEstado() {
    int op;
    cout << "\n--- LISTAR ORDENES POR ESTADO ---\n";
    cout << "1. ABIERTA\n2. CERRADA\n3. ANULADA\nEstado: ";
    cin >> op; cin.clear(); cin.ignore(10000, '\n');

    const char *estado = (op == 1) ? "ABIERTA" : (op == 2) ? "CERRADA" : (op == 3) ? "ANULADA" : "";

    OrdenServicio resultados[MAX_RESULTADOS_BUSQUEDA];
    int cantidad = 0;
    bool hayMas = false;
    int res = listarOrdenesPorEstadoLogica(estado, resultados, cantidad, hayMas);

    if (res == -2) {
        cout << "[ERROR]: Estado invalido.\n";
    } else if (res != 1) {
        cout << "[ERROR]: No se pudo leer " << RUTA_ORDENES << ".\n";
    } else if (cantidad == 0) {
        cout << "No hay ordenes en estado " << estado << ".\n";
    } else {
        for (int i = 0; i < cantidad; i++) {
            mostrarOrden(resultados[i]);
        }
        if (hayMas) {
            cout << "(Hay mas ordenes " << estado << ": se muestran solo las primeras "
                 << MAX_RESULTADOS_BUSQUEDA << ".)\n";
        }
    }
}

void submenuVehiculos() {
    int op = 0;
    do {
        cout << "\n--- GESTION DE VEHICULOS ---\n";
        cout << "1. Registrar vehiculo\n";
        cout << "2. Listar vehiculos\n";
        cout << "3. Actualizar vehiculo\n";
        cout << "4. Eliminar vehiculo\n";
        cout << "5. Buscar por ID\n";
        cout << "6. Buscar por placa\n";
        cout << "7. Buscar por propietario\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuRegistrarVehiculo();             break;
            case 2: menuMostrarVehiculos();              break;
            case 3: menuActualizarVehiculo();            break;
            case 4: menuEliminarVehiculo();              break;
            case 5: menuBuscarVehiculoPorID();           break;
            case 6: menuBuscarVehiculoPorPlaca();        break;
            case 7: menuBuscarVehiculosPorPropietario(); break;
        }
    } while (op != 0);
}

void submenuMecanicos() {
    int op = 0;
    do {
        cout << "\n--- GESTION DE MECANICOS ---\n";
        cout << "1. Registrar mecanico\n";
        cout << "2. Listar mecanicos (activos y bajas)\n";
        cout << "3. Dar de baja mecanico\n";
        cout << "4. Buscar por ID\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuRegistrarMecanico(); break;
            case 2: menuMostrarMecanicos();  break;
            case 3: menuDarDeBajaMecanico(); break;
            case 4: menuBuscarMecanicoPorID(); break;
        }
    } while (op != 0);
}

void submenuRepuestos() {
    int op = 0;
    do {
        cout << "\n--- GESTION DE REPUESTOS ---\n";
        cout << "1. Registrar repuesto\n";
        cout << "2. Listar repuestos\n";
        cout << "3. Entrada de mercancia\n";
        cout << "4. Cambiar precio\n";
        cout << "5. Eliminar repuesto\n";
        cout << "6. Buscar por ID\n";
        cout << "7. Buscar por codigo o nombre\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuRegistrarRepuesto();     break;
            case 2: menuMostrarRepuestos();      break;
            case 3: menuEntradaMercancia();      break;
            case 4: menuCambiarPrecioRepuesto(); break;
            case 5: menuEliminarRepuesto();      break;
            case 6: menuBuscarRepuestoPorID();   break;
            case 7: menuBuscarRepuestos();       break;
        }
    } while (op != 0);
}

void submenuOrdenes() {
    int op = 0;
    do {
        cout << "\n--- GESTION DE ORDENES ---\n";
        cout << "1. Crear orden\n";
        cout << "2. Agregar repuesto a orden\n";
        cout << "3. Quitar repuesto de orden\n";
        cout << "4. Cerrar orden\n";
        cout << "5. Anular orden\n";
        cout << "6. Listar ordenes\n";
        cout << "7. Buscar orden por ID (con su detalle)\n";
        cout << "8. Listar ordenes por estado\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuCrearOrden();            break;
            case 2: menuAgregarRepuesto();       break;
            case 3: menuQuitarRepuesto();        break;
            case 4: menuCerrarOrden();           break;
            case 5: menuAnularOrden();           break;
            case 6: menuMostrarOrdenes();        break;
            case 7: menuBuscarOrdenPorID();      break;
            case 8: menuListarOrdenesPorEstado(); break;
        }
    } while (op != 0);
}