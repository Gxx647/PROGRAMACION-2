- Sistema de Gestion de Taller Mecanico

Este proyecto consiste en una aplicacion modular desarrollada en C++ orientada a la administracion integral de un taller mecanico. El sistema esta disenado para gestionar vehiculos, personal tecnico, inventario de repuestos y ordenes de servicio utilizando estructuras de datos avanzadas y asignacion dinamica de memoria en el Heap.

Autores
Ricardo Sobalvarro
Guillermo Arias

## Caracteristicas Principales
* Gestion de Memoria Dinamica: Uso de operadores `new` y `delete[]` mediante un patron de duplicacion de capacidad para asegurar el crecimiento automatico de los arreglos en tiempo de ejecucion.
* Manejo Seguro de Cadenas: Empleo estricto de arreglos de caracteres (`char`) y funciones de la libreria `<cstring>` para garantizar la portabilidad y el rendimiento.
* Control de Inventario: Asociacion de repuestos a ordenes de servicio con descuento automatico de stock.
* Facturacion y Cierre: Modulo de calculo automatizado que integra el costo de la mano de obra con los repuestos utilizados para emitir la factura final de reparacion.
* Blindaje de Entrada: Validacion de flujos con `std::cin` para prevenir bucles infinitos y errores por desbordamiento de buffer.

## Estructura del Proyecto
El codigo fuente se encuentra modularizado de la siguiente manera:
* `estructura.h`: Definicion de las estructuras de datos (`Taller`, `Vehiculo`, `Mecanico`, `Repuesto`, `DetalleOrden`, `OrdenServicio` y `SistemaTaller`).
* `logica.h`: Prototipos de las funciones operativas del sistema.
* `logica.cpp`: Implementacion de la logica de negocio, gestion de memoria dinamica y operaciones CRUD.
* `main.cpp`: Punto de entrada de la aplicacion que coordina el menu interactivo y el flujo del usuario.

## Requisitos de Compilacion
Para compilar y ejecutar el sistema de manera local, se requiere un compilador de C++ compatible con el estandar (como GCC/MinGW).

Ejecuta el siguiente comando en la terminal desde la carpeta del proyecto:
```bash
 g++ main.cpp logica.cpp presentacion.cpp -o taller

./taller