# Sistema de Gestion de Taller Mecanico

Aplicacion modular desarrollada en C++17 orientada a la administracion integral de un taller mecanico[cite: 10, 14]. El sistema gestiona vehiculos, personal tecnico, inventario de repuestos, ordenes de servicio, reportes y tareas de mantenimiento con persistencia binaria directa en disco mediante cabeceras (`ArchivoHeader`) y posicionamiento absoluto (`seekg`/`seekp`)[cite: 10, 14, 17, 20].

## Autores
* Ricardo Sobalvarro
* Guillermo Arias

## Caracteristicas Principales
* **Persistencia Binaria en Disco:** Almacenamiento en archivos binarios dentro de `datos/` (`taller.bin`, `vehiculos.bin`, `mecanicos.bin`, `repuestos.bin` y `ordenes.bin`), con `ArchivoHeader` (`cantidadRegistros`, `proximoID`, `registrosActivos`, `version`)[cite: 10, 16, 17].
* **Acceso Aleatorio y Borrado Logico:** Calculo directo de desplazamiento en bytes para lectura y sobrescritura por ID, conservando registros eliminados o dados de baja (`eliminado = true`) para no alterar posiciones ni referencias historicas[cite: 17].
* **Estructuras de Tamano Fijo:** Uso de arreglos de caracteres (`char`) con `<cstring>` y un arreglo fijo de 15 lineas de detalle (`DetalleLineaOrden lineasDetalle[15]`) en `OrdenServicio` para mantener un tamano constante en disco[cite: 10, 11, 12].
* **Ciclo de Vida de Ordenes e Inventario:** Creacion, agregado y retiro de repuestos (con ajuste y devolucion de stock), cierre con calculo de mano de obra, repuestos a precio historico y factura, y anulacion con reintegro de inventario[cite: 11, 18].
* **Consultas, Reportes y Mantenimiento:** Busquedas por ID, placa, propietario, codigo/nombre o estado; reportes de historial por placa, top de repuestos en ordenes cerradas y facturacion por mecanico; respaldo (`crearBackup`) y revision de consistencia (`verificarIntegridad`)[cite: 14, 18, 20].
* **Blindaje de Entrada:** Validacion de flujos con `std::cin`, limpieza de buffer y confirmacion previa de datos antes de registrar o actualizar[cite: 13, 18].

## Estructura del Proyecto
* `estructura.h`: Definicion de `ArchivoHeader`, `Taller`, `Vehiculo`, `Mecanico`, `Repuesto`, `DetalleLineaOrden` y `OrdenServicio`[cite: 10].
* `persistencia.h` y `persistencia.cpp`: Inicializacion de archivos y cabeceras, lectura/escritura del taller, calculo de posicion por ID y operaciones binarias genericas (`guardarRegistro`, `leerRegistroPorID`, `sobrescribirRegistro`, `marcarComoEliminado`)[cite: 16, 17].
* `logica.h` y `logica.cpp`: Reglas de negocio, validaciones, CRUD, gestion de ordenes y funciones de consulta y busqueda[cite: 11, 12].
* `presentacion.h` y `presentacion.cpp`: Menus, submenus, confirmacion de datos, listados (incluyendo ordenes abiertas por mecanico) y visualizacion de facturas y estados[cite: 5, 6, 18, 19].
* `reportes.h` y `reportes.cpp`: Reportes de historial de vehiculo, repuestos mas utilizados en ordenes cerradas y facturacion por mecanico[cite: 20, 21].
* `mantenimiento.h` y `mantenimiento.cpp`: Verificacion de integridad del sistema (`verificarIntegridad`) y creacion de respaldos (`crearBackup`)[cite: 7, 8, 14].
* `main.cpp`: Inicializacion del sistema de archivos, configuracion inicial del taller y bucle del menu principal[cite: 13].

## Requisitos de Compilacion y Ejecucion
Se requiere un compilador de C++ compatible con el estandar **C++17** (como GCC/MinGW)[cite: 14].

Compila desde la carpeta raiz del proyecto:
```bash
g++ -std=c++17 main.cpp persistencia.cpp logica.cpp mantenimiento.cpp reportes.cpp presentacion.cpp -o taller