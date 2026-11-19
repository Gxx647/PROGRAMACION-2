#include "presentacion.h"
#include "logica.h"
#include "persistencia.h"
#include <iostream>
#include <cstring>

using namespace std;

void mostrarVehiculo(const Vehiculo &v) {
    cout << "ID: " << v.id
         << " | Placa: " << v.placa
         << " | " << v.marca << " " << v.modelo
         << " (" << v.anio << ")"
         << " | Km: " << v.kilometraje
         << " | Propietario: " << v.nombrePropietario << "\n";
}

void mostrarMecanico(const Mecanico &m) {
    cout << "ID: " << m.id
         << " | " << m.nombre
         << " | Cedula: " << m.cedula
         << " | Especialidad: " << m.especialidad;
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
    int anio, km;

    cout << "\n--- REGISTRO DE NUEVO VEHICULO ---\n";
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
        cout << "[ERROR]: Fallo al registrar el vehiculo en el archivo.\n";
    }
}

void menuMostrarVehiculos() {
    cout << "\n--- LISTA DE VEHICULOS ACTIVOS ---\n";
    ArchivoHeader h;
    if (!leerHeader(RUTA_VEHICULOS, h)) {
        cout << "[ERROR] No se pudo leer el archivo.\n";
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
    int id, anio;
    char marca[50], modelo[50], nombreProp[100], cedulaProp[20], telefonoProp[20];

    cout << "\n--- ACTUALIZAR VEHICULO ---\n";
    menuMostrarVehiculos();
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

    int res = actualizarVehiculoLogica(id, marca, modelo, anio, nombreProp, cedulaProp, telefonoProp);
    if (res == 1) {
        cout << "Vehiculo actualizado correctamente.\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el vehiculo no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El vehiculo esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: Anio fuera de rango (1950-2027).\n";
    } else {
        cout << "[ERROR]: Fallo al actualizar el vehiculo.\n";
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
        cout << "Vehiculo marcado como eliminado.\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el vehiculo no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El vehiculo ya estaba eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: No se puede eliminar; el vehiculo tiene ordenes registradas.\n";
    } else {
        cout << "[ERROR]: Fallo al eliminar el vehiculo.\n";
    }
}

void menuRegistrarMecanico() {
    Mecanico nuevo;
    memset(&nuevo, 0, sizeof(Mecanico));

    cout << "\n--- REGISTRAR NUEVO MECANICO ---\n";
    cout << "Nombre: "; cin.getline(nuevo.nombre, 100);
    cout << "Cedula: "; cin.getline(nuevo.cedula, 20);
    cout << "Especialidad (Motor/Frenos/Electricidad/Suspension/General): "; cin.getline(nuevo.especialidad, 20);
    cout << "Fecha de ingreso (YYYY-MM-DD): "; cin.getline(nuevo.fechaIngreso, 11);

    int res = registrarMecanico(nuevo);
    if (res > 0) {
        cout << "Mecanico registrado con ID #" << res << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: Ya existe un mecanico con esa cedula.\n";
    } else if (res == -3) {
        cout << "[ERROR]: Especialidad no permitida.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar el mecanico.\n";
    }
}

void menuMostrarMecanicos() {
    cout << "\n--- LISTA DE MECANICOS (activos y dados de baja) ---\n";
    ArchivoHeader h;
    if (!leerHeader(RUTA_MECANICOS, h)) {
        cout << "[ERROR] No se pudo leer el archivo.\n";
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
        cout << "Mecanico dado de baja correctamente.\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el mecanico no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El mecanico ya estaba dado de baja.\n";
    } else if (res == -4) {
        cout << "[ERROR]: No se puede dar de baja; tiene ordenes ABIERTAS.\n";
    } else {
        cout << "[ERROR]: Fallo al dar de baja al mecanico.\n";
    }
}

void menuRegistrarRepuesto() {
    char codigo[20], nombre[100], marca[50];
    float precio;
    int stock, stockMinimo;

    cout << "\n--- REGISTRO DE NUEVO REPUESTO ---\n";
    cout << "Codigo: ";             cin.getline(codigo, 20);
    cout << "Nombre: ";             cin.getline(nombre, 100);
    cout << "Marca: ";              cin.getline(marca, 50);
    cout << "Precio unitario ($): "; cin >> precio;
    cout << "Stock inicial: ";      cin >> stock;
    cout << "Stock minimo: ";       cin >> stockMinimo;
    cin.clear(); cin.ignore(10000, '\n');

    int res = registrarRepuestoLogica(codigo, nombre, marca, precio, stock, stockMinimo);
    if (res > 0) {
        cout << "Repuesto registrado con ID #" << res << ".\n";
    } else if (res == -2) {
        cout << "[ERROR]: Ya existe un repuesto ACTIVO con ese codigo.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El precio debe ser mayor a 0.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El stock no puede ser negativo.\n";
    } else if (res == -5) {
        cout << "[ERROR]: El stock minimo no puede ser negativo.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar el repuesto.\n";
    }
}

void menuMostrarRepuestos() {
    cout << "\n--- INVENTARIO DE REPUESTOS ACTIVOS ---\n";
    ArchivoHeader h;
    if (!leerHeader(RUTA_REPUESTOS, h)) {
        cout << "[ERROR] No se pudo leer el archivo.\n";
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
        cout << "Stock actualizado correctamente.\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el repuesto no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El repuesto esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: La cantidad debe ser mayor a 0.\n";
    } else {
        cout << "[ERROR]: Fallo al registrar la entrada de mercancia.\n";
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
        cout << "Precio actualizado (las ordenes ya registradas conservan su precio historico).\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el repuesto no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El repuesto esta eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El precio debe ser mayor a 0.\n";
    } else {
        cout << "[ERROR]: Fallo al cambiar el precio.\n";
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
        cout << "Repuesto eliminado logicamente.\n";
    } else if (res == -2) {
        cout << "[ERROR]: ID invalido o el repuesto no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: El repuesto ya estaba eliminado.\n";
    } else if (res == -4) {
        cout << "[ERROR]: No se puede eliminar; el repuesto aparece en alguna orden.\n";
    } else {
        cout << "[ERROR]: Fallo al eliminar el repuesto.\n";
    }
}

void menuCrearOrden() {
    char fecha[11], falla[200];
    int km, idVehiculo, idMecanico;

    cout << "\n--- CREAR NUEVA ORDEN DE SERVICIO ---\n";
    menuMostrarVehiculos();
    menuMostrarMecanicos();

    cout << "\nFecha de apertura (YYYY-MM-DD): "; cin.getline(fecha, 11);
    cout << "Descripcion de la falla: ";          cin.getline(falla, 200);
    cout << "Kilometraje de ingreso: ";           cin >> km;
    cin.clear(); cin.ignore(10000, '\n');
    cout << "ID del vehiculo: ";                  cin >> idVehiculo;
    cout << "ID del mecanico: ";                  cin >> idMecanico;
    cin.clear(); cin.ignore(10000, '\n');

    int res = crearOrdenServicioLogica(fecha, falla, km, idVehiculo, idMecanico);
    if (res > 0) {
        cout << "Orden creada con ID #" << res << ".\n";
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
        cout << "[ERROR]: Fallo al crear la orden.\n";
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
        cout << "Repuesto agregado exitosamente. Stock actualizado.\n";
    } else if (res == -3) {
        cout << "[ERROR]: No existe la orden.\n";
    } else if (res == -4) {
        cout << "[ERROR]: La orden no esta ABIERTA.\n";
    } else if (res == -5) {
        cout << "[ERROR]: No existe el repuesto (o esta eliminado).\n";
    } else if (res == -6) {
        cout << "[ERROR]: La cantidad debe ser mayor a 0.\n";
    } else if (res == -7) {
        cout << "[ERROR]: Stock insuficiente.\n";
    } else if (res == -8) {
        cout << "[ERROR]: La orden ya tiene 15 lineas de detalle (limite alcanzado).\n";
    } else {
        cout << "[ERROR]: Fallo al agregar el repuesto.\n";
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
        cout << "Linea retirada y stock devuelto al inventario.\n";
    } else if (res == -2) {
        cout << "[ERROR]: La orden no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: Solo se pueden quitar repuestos en ordenes ABIERTAS.\n";
    } else if (res == -4) {
        cout << "[ERROR]: El repuesto no figura en la orden.\n";
    } else {
        cout << "[ERROR]: Fallo al quitar el repuesto de la orden.\n";
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
        cout << "\n=========== FACTURA ===========\n";
        cout << "Orden #"      << ordenEn.id << "\n";
        cout << "Vehiculo ID: "  << ordenEn.idVehiculo << "\n";
        cout << "Mecanico ID: "  << ordenEn.idMecanico << "\n";
        cout << "Falla: "        << ordenEn.descripcionFalla << "\n";
        cout << "Fecha cierre: " << ordenEn.fechaCierre << "\n";
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
        cout << "[ERROR]: Fallo al cerrar la orden.\n";
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
        cout << "Orden #" << ordenOut.id << " ANULADA. Se devolvio el stock de sus lineas.\n";
    } else if (res == -2) {
        cout << "[ERROR]: La orden no existe.\n";
    } else if (res == -3) {
        cout << "[ERROR]: No se puede anular una orden en estado " << ordenOut.estado << ".\n";
    } else {
        cout << "[ERROR]: Fallo al anular la orden.\n";
    }
}

void menuMostrarOrdenes() {
    cout << "\n--- LISTA DE ORDENES DE SERVICIO ---\n";
    ArchivoHeader h;
    if (!leerHeader(RUTA_ORDENES, h)) {
        cout << "[ERROR] No se pudo leer el archivo.\n";
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

void submenuVehiculos() {
    int op = 0;
    do {
        cout << "\n--- GESTION DE VEHICULOS ---\n";
        cout << "1. Registrar vehiculo\n";
        cout << "2. Listar vehiculos\n";
        cout << "3. Actualizar vehiculo\n";
        cout << "4. Eliminar vehiculo\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuRegistrarVehiculo();  break;
            case 2: menuMostrarVehiculos();   break;
            case 3: menuActualizarVehiculo(); break;
            case 4: menuEliminarVehiculo();   break;
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
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuRegistrarMecanico(); break;
            case 2: menuMostrarMecanicos();  break;
            case 3: menuDarDeBajaMecanico(); break;
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
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuRegistrarRepuesto();     break;
            case 2: menuMostrarRepuestos();      break;
            case 3: menuEntradaMercancia();      break;
            case 4: menuCambiarPrecioRepuesto(); break;
            case 5: menuEliminarRepuesto();      break;
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
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; cin.clear(); cin.ignore(10000, '\n');
        switch (op) {
            case 1: menuCrearOrden();      break;
            case 2: menuAgregarRepuesto(); break;
            case 3: menuQuitarRepuesto();  break;
            case 4: menuCerrarOrden();     break;
            case 5: menuAnularOrden();     break;
            case 6: menuMostrarOrdenes();  break;
        }
    } while (op != 0);
}