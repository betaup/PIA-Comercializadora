#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "conexiones.h"
#define MAX_PRODUCTOS 100
#define MAX_CLIENTES 100

// Funcion para validar la entrada S/N
char obtenerOtraCompraSN() {
    char opcion[10];
    while (1) {
        printf("\nDesea realizar otra compra? (S/N): ");
        scanf("%s", opcion);
        if (strlen(opcion) == 1 && (opcion[0] == 'S' || opcion[0] == 's' || opcion[0] == 'N' || opcion[0] == 'n')) {
            return opcion[0];
        } else {
            printf("\nOpcion no valida. Por favor, ingrese S o N.\n");
        }
    }
}

int validarCliente(int idCliente) {
    FILE *archivoClientes = fopen("clientes.txt", "r");
    if (archivoClientes == NULL) {
        printf("Error al abrir el archivo de clientes.\n");
        return 0; // Indica que no se pudo validar el cliente
    }

    Cliente cliente;
    int clienteEncontrado = 0;

    while (fscanf(archivoClientes, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d",
                  &cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno,
                  cliente.RFC, cliente.Correo, cliente.Telefono, &cliente.descuento,
                  cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero,
                  cliente.direccion.CP, &cliente.Estatus) == 13) {
        if (cliente.Id == idCliente && cliente.Estatus == 1) {
            clienteActual = cliente; // Guardar la informacion del cliente encontrado
            clienteEncontrado = 1;
            break;
        }
    }

    fclose(archivoClientes);

    return clienteEncontrado;
}

void* procesarArchivo(void* arg) {
    FILE *archivoVentas = fopen("ventas.txt", "r");
    if (archivoVentas == NULL) {
        printf("Error al abrir el archivo de ventas.\n");
        pthread_exit(NULL);
    }

    int* maxFolio = (int*)arg;
    Venta venta;
    while (fscanf(archivoVentas, "%d,%*d,%*d,%*d,%*f,%*s", &venta.folio) == 1) {
        if (venta.folio > *maxFolio) {
            *maxFolio = venta.folio;
        }
    }

    fclose(archivoVentas);
    pthread_exit(NULL);
}

// Funcion para generar el folio de ventas
int generarFolioVentas() {
    int maxFolio = 0;

    pthread_t thread;
    if (pthread_create(&thread, NULL, procesarArchivo, &maxFolio)) {
        printf("Error al crear el hilo.\n");
        return 0;
    }

    if (pthread_join(thread, NULL)) {
        printf("Error al esperar por el hilo.\n");
        return 0;
    }

    // Incrementar el folio para la proxima venta
    return maxFolio + 1;
}

// Funcion para realizar una venta
void realizarVenta() {
    system("cls");
    FILE *archivoProductos, *archivoVentas;
    FILE *archivoFactura;
    Producto productos[100];
    Venta venta;
    char fecha[11];
    char entrada[10];
    int tipo;
    int idCliente;
    int numProductos = 0;
    int i, j;

    // Validar el ID del cliente
    idCliente = validarEntero("\nIngrese el ID del cliente: ");

    // Validar si el cliente existe
    if (!validarCliente(idCliente)) {
        printf("\nCliente no encontrado. Volviendo al submenu de ventas.\n");
        sleep(2);
        system("cls");
        submenuVentas();
        return;
    }

    system("cls");

    // Variables para la venta
    float subtotal = 0;
    float descuento = clienteActual.descuento / 100.0;
    float totalDescuento = 0;
    float total = 0;
    float iva = 0.16;
    int folio = generarFolioVentas();
    float subtotalConDescuento, totalIVA;

    while (1) {
        // Obtener la fecha actual
        obtenerFechaActual(fecha);

        // Mostrar tipos de productos y solicitar la seleccion al usuario
        do {
            printf("\nCliente encontrado.\n");
            printf("Nombre: %s\n", clienteActual.Nombre);
            printf("Apellido Paterno: %s\n", clienteActual.Paterno);
            printf("\n--- Tipos de Producto ---\n");
            printf("1. Escritorios\n");
            printf("2. Archiveros\n");
            printf("3. Sillas\n");
            printf("4. Mesas de centro\n");
            printf("5. Mesas ejecutivas\n");
            printf("6. Sofas\n");
            printf("7. Decorativos\n");
            printf("\nSeleccione el tipo de producto (1-7): ");
            scanf("%s", entrada);

            if (!validarNumero(entrada) || (tipo = atoi(entrada)) < 1 || tipo > 7) {
                printf("\n-------------------------------------------------------\n");
                printf("Tipo de producto no valido. Ingrese un numero correcto!\n");
                printf("-------------------------------------------------------\n");
                sleep(2);
                system("cls");
            }
        } while (!validarNumero(entrada) || (tipo = atoi(entrada)) < 1 || tipo > 7);

        // Abrir el archivo de productos en modo lectura
        archivoProductos = fopen("productos.txt", "r");
        if (archivoProductos == NULL) {
            printf("Error al abrir el archivo de productos.\n");
            return;
        }

        // Mostrar la lista de productos del tipo seleccionado
        system("cls");
        printf("\t\tComercializadora Fuentes\n");
        printf("\t\t Consulta por Producto\n\n");
        printf("\nFecha: %s\n", fecha);
        printf("\n%-8s %-20s %-20s %-10s %s\n", "Clave", "Descripcion", "Cant. Existencia", "Precio", "Estatus");
        printf("---------------------------------------------------------------------\n");

        numProductos = 0;
        while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d", &productos[numProductos].clave, productos[numProductos].descripcion, &productos[numProductos].tipo, &productos[numProductos].cantidad_existencia, &productos[numProductos].nivel_stock, &productos[numProductos].precio, &productos[numProductos].estatus) == 7) {
            if (productos[numProductos].tipo == tipo) {
                mostrarProducto(productos[numProductos]);
            }
            numProductos++;
        }

        fclose(archivoProductos);

        // Solicitar al usuario la clave del producto a comprar
        int claveProducto;
        int cantidad;
        int productoEncontrado = 0;

        claveProducto = validarEntero("\nIngrese la clave del producto a comprar (0 para cancelar): ");

        // Validar si la clave del producto es un numero entero valido
        if (claveProducto == 0) {
            printf("\nProducto cancelado.\n");
            sleep(1);
            system("cls");
            return;
        }

        // Buscar el producto en la lista de productos
        for (i = 0; i < numProductos; i++) {
            if (productos[i].clave == claveProducto && productos[i].estatus == 1) {
                productoEncontrado = 1;
                printf("\nProducto encontrado:\n");
                printf("Descripcion: %s\n", productos[i].descripcion);
                printf("Existencia: %d\n", productos[i].cantidad_existencia);
                printf("Precio: %.2f\n", productos[i].precio);

                do {
                    cantidad = validarEntero("\nIngrese la cantidad a comprar (0 para cancelar): ");

                    if (cantidad == 0) {
                        printf("\nProducto cancelado.\n");
                        sleep(1);
                        system("cls");
                        return;
                    }

                    if (cantidad > productos[i].cantidad_existencia) {
                        printf("Cantidad no disponible. Ingrese una cantidad menor o igual a %d.\n", productos[i].cantidad_existencia);
                    }
                } while (cantidad > productos[i].cantidad_existencia);

                if (cantidad > 0 && cantidad <= productos[i].cantidad_existencia) {
                    productos[i].cantidad_existencia -= cantidad;
                    subtotal += productos[i].precio * cantidad;

                    // Guardar la información de la venta en el archivo de ventas
                    archivoVentas = fopen("ventas.txt", "a");
                    if (archivoVentas == NULL) {
                        printf("Error al abrir el archivo de ventas.\n");
                        return;
                    }

                    fprintf(archivoVentas, "%d,%d,%d,%d,%.2f,%s\n", folio, idCliente, claveProducto, cantidad, productos[i].precio, fecha);
                    fclose(archivoVentas);

                    // Guardar el producto actualizado
                    archivoProductos = fopen("productos.txt", "w");
                    if (archivoProductos == NULL) {
                        printf("Error al abrir el archivo de productos.\n");
                        return;
                    }

                    for (j = 0; j < numProductos; j++) {
                        fprintf(archivoProductos, "%d,%s,%d,%d,%d,%.2f,%d\n", productos[j].clave, productos[j].descripcion, productos[j].tipo, productos[j].cantidad_existencia, productos[j].nivel_stock, productos[j].precio, productos[j].estatus);
                    }

                    fclose(archivoProductos);
                }
                break;
            }
        }

        if (!productoEncontrado) {
            printf("\nProducto no encontrado.\n");
        }

        // Preguntar al usuario si desea realizar otra compra
        char continuar = obtenerOtraCompraSN();
        if (continuar != 'S' && continuar != 's') {
            break;
        }
    }

    // Calcular totales
    totalDescuento = subtotal * descuento;
    subtotalConDescuento = subtotal - totalDescuento;
    totalIVA = subtotalConDescuento * iva;
    total = subtotalConDescuento + totalIVA;

    // Generar el nombre del archivo de factura
    char nombreArchivo[20];
    sprintf(nombreArchivo, "Fact%d.txt", folio);

    // Abrir el archivo de factura en modo escritura
    archivoFactura = fopen(nombreArchivo, "w");
    if (archivoFactura == NULL) {
        printf("Error al crear el archivo de factura.\n");
        return;
    }

    // Escribir los detalles de la factura
    fprintf(archivoFactura, "\t\tComercializadora Fuentes\n");
    fprintf(archivoFactura, "\t\t    Ticket de Venta\n\n");
    fprintf(archivoFactura, "Folio: %d\n", folio);
    fprintf(archivoFactura, "Fecha: %s\n", fecha);
    fprintf(archivoFactura, "Cliente: %s %s\n", clienteActual.Nombre, clienteActual.Paterno);
    fprintf(archivoFactura, "--------------------------------------------------------\n");
    fprintf(archivoFactura, "Clave       \tCantidad\tPrecio\t\tSubtotal\n");
    fprintf(archivoFactura, "--------------------------------------------------------\n");

    // Reabrir el archivo de ventas para leer las ventas del folio actual
    archivoVentas = fopen("ventas.txt", "r");
    if (archivoVentas == NULL) {
        printf("Error al abrir el archivo de ventas.\n");
        return;
    }

    // Escribir los detalles de cada producto vendido en la factura
    while (fscanf(archivoVentas, "%d,%d,%d,%d,%f,%s", &venta.folio, &venta.idCliente, &venta.claveProducto, &venta.cantidadComprada, &venta.precioVenta, venta.fecha) == 6) {
        if (venta.folio == folio) {
            fprintf(archivoFactura, "%d\t\t%d\t\t%.2f\t\t%.2f\n", venta.claveProducto, venta.cantidadComprada, venta.precioVenta, venta.cantidadComprada * venta.precioVenta);
        }
    }

    fclose(archivoVentas);

    // Escribir los totales en la factura
    fprintf(archivoFactura, "--------------------------------------------------------\n");
    fprintf(archivoFactura, "Subtotal:\t\t\t\t\t%.2f\n", subtotal);
    fprintf(archivoFactura, "Descuento:\t\t\t\t\t%.2f\n", totalDescuento);
    fprintf(archivoFactura, "IVA:\t\t\t\t\t\t%.2f\n", totalIVA);
    fprintf(archivoFactura, "Total a pagar:\t\t\t\t\t%.2f\n", total);
    fclose(archivoFactura);

    printf("\nVenta realizada con exito! Factura generada: %s\n", nombreArchivo);

    printf("\nRegresando al submenu de ventas...\n");
    sleep(2);
    system("cls");
    submenuVentas();
}

// Funcion para generar un reporte de ventas del dia
void reporteVentaDelDia() {
    system("cls");
    FILE *archivoVentas;
    Venta venta;
    char fechaActual[11];
    float totalSubtotal = 0, totalDescuento = 0, totalIVA = 0, totalTotal = 0;
    float subtotal, descuento, iva, total;

    // Obtener la fecha actual
    obtenerFechaActual(fechaActual);
    archivoVentas = fopen("ventas.txt", "r");
    if (archivoVentas == NULL) {
        printf("Error al abrir el archivo de ventas.\n");
        return;
    }

    // Imprimir el encabezado del reporte
    printf("\t\t    Comercializadora Fuentes\n");
    printf("\t\tReporte de Ventas al dia %s\n\n", fechaActual);
    printf("%-8s %-20s %-12s %-12s %-12s %-12s\n", "Folio", "Cliente", "Subtotal", "Descuento", "IVA", "Total");
    printf("-----------------------------------------------------------------------------\n");

    // Leer las ventas del archivo y calcular los totales
    while (fscanf(archivoVentas, "%d,%d,%d,%d,%f,%s", &venta.folio, &venta.idCliente, &venta.claveProducto, &venta.cantidadComprada, &venta.precioVenta, venta.fecha) == 6) {
        if (strcmp(venta.fecha, fechaActual) == 0) {
            // Validar si el cliente existe y cargar sus datos
            if (validarCliente(venta.idCliente)) {
                subtotal = venta.cantidadComprada * venta.precioVenta;
                descuento = clienteActual.descuento / 100.0 * subtotal;
                iva = 0.16 * (subtotal - descuento);
                total = subtotal - descuento + iva;

                totalSubtotal += subtotal;
                totalDescuento += descuento;
                totalIVA += iva;
                totalTotal += total;

                printf("%-8d %-20s %-12.2f %-12.2f %-12.2f %-12.2f\n", venta.folio, strcat(strcat(clienteActual.Nombre, " "), clienteActual.Paterno), subtotal, descuento, iva, total);
            }
        }
    }

    // Imprimir los totales del dia
    printf("-----------------------------------------------------------------------------\n");
    printf("%-29s %-12.2f %-12.2f %-12.2f %-12.2f\n", "Totales:", totalSubtotal, totalDescuento, totalIVA, totalTotal);

    fclose(archivoVentas);

    printf("\nPresione cualquier tecla para salir...");
    getchar();
    getchar();
    system("cls");
}


// Funcion para generar un reporte de ventas por cliente
void reporteVentaPorCliente() {
    system("cls");
    int claveCliente, i;
    int numProductos = 0;
    float subtotal = 0, descuento = 0, iva = 0, total = 0;
    float totalDescuento, subtotalConDescuento;
    bool ventasEncontradas = false;
    Venta venta;
    Producto productos[MAX_PRODUCTOS];

    // Validar el ID del cliente
    claveCliente = validarEntero("\nIngrese el ID del cliente: ");

    // Validar si el cliente existe
    if (!validarCliente(claveCliente)) {
        printf("\nCliente no encontrado. Volviendo al submenu de ventas.\n");
        sleep(2);
        system("cls");
        submenuVentas();
        return;
    }

    system("cls");

    // Cargar las ventas del cliente desde el archivo de ventas
    FILE *archivoVentas = fopen("ventas.txt", "r");
    if (archivoVentas == NULL) {
        printf("Error al abrir el archivo de ventas.\n");
        return;
    }

    // Cargar todos los productos desde el archivo de productos
    FILE *archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        fclose(archivoVentas);
        return;
    }

    // Leer los productos del archivo
    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d",
                  &productos[numProductos].clave,
                  productos[numProductos].descripcion,
                  &productos[numProductos].tipo,
                  &productos[numProductos].cantidad_existencia,
                  &productos[numProductos].nivel_stock,
                  &productos[numProductos].precio,
                  &productos[numProductos].estatus) == 7) {
        numProductos++;
    }
    fclose(archivoProductos);

    printf("\n\t\t  Comercializadora Fuentes\n");
    printf("\t\tReporte de Ventas por Cliente\n\n");
    printf("Clave: %d\n", clienteActual.Id);
    printf("Nombre: %s %s\n", clienteActual.Nombre, clienteActual.Paterno);
    printf("Direccion: %s, %s, %s, CP %s\n", clienteActual.direccion.Calle, clienteActual.direccion.Colonia, clienteActual.direccion.Numero, clienteActual.direccion.CP);
    printf("Telefono: %s\n", clienteActual.Telefono);
    printf("Correo Electronico: %s\n\n", clienteActual.Correo);
    printf("%-8s %-14s %-20s %-13s %-12s\n", "Folio", "Fecha", "Descripcion", "Cantidad", "Precio Total");
    printf("-----------------------------------------------------------------------\n");

    while (fscanf(archivoVentas, "%d,%d,%d,%d,%f,%s",
                  &venta.folio,
                  &venta.idCliente,
                  &venta.claveProducto,
                  &venta.cantidadComprada,
                  &venta.precioVenta,
                  venta.fecha) == 6) {
        if (venta.idCliente == claveCliente) {
            ventasEncontradas = true;
            // Buscar la descripcion del producto por su clave
            char descripcionProducto[100] = "Producto no encontrado";
            for (i = 0; i < numProductos; i++) {
                if (productos[i].clave == venta.claveProducto) {
                    strcpy(descripcionProducto, productos[i].descripcion);
                    break;
                }
            }

            printf("%-8d %-14s %-20s %-13d %-12.2f\n", venta.folio, venta.fecha, descripcionProducto, venta.cantidadComprada, venta.cantidadComprada * venta.precioVenta);

            // Actualizar totales
            subtotal += venta.cantidadComprada * venta.precioVenta;
        }
    }

    fclose(archivoVentas);

    if (!ventasEncontradas) {
        printf("No se encontraron ventas para este cliente.\n");
        printf("\nPresione cualquier tecla para salir...");
        getchar();
        getchar();
        system("cls");
        return;
    }

    descuento = clienteActual.descuento / 100.0;
    totalDescuento = subtotal * descuento;
    subtotalConDescuento = subtotal - totalDescuento;
    iva = subtotalConDescuento * 0.16;
    total = subtotalConDescuento + iva;

    // Imprimir totales
    printf("\n\n");
    printf("Subtotal: %.2f\n", subtotal);
    printf("Descuento: %.2f\n", totalDescuento);
    printf("IVA: %.2f\n", iva);
    printf("Total: %.2f\n", total);

    printf("\nPresione cualquier tecla para salir...");
    getchar();
    getchar();
    system("cls");
}

//Funcion para mostrar submenu de reportes de ventas
void reporteVenta() {
    system("cls");
    char subopcion[10];
    int salir = 0; // Bandera para controlar la salida del bucle
    do {
        printf("\n--- Reporte de Ventas ---\n");
        printf("\nA. Venta del dia\n");
        printf("B. Por Cliente\n");
        printf("C. Regresar al submenu de ventas\n");
        printf("\nIngrese su opcion: ");
        scanf("%s", subopcion);

        if (strlen(subopcion) == 1 && (subopcion[0] == 'A' || subopcion[0] == 'a' || subopcion[0] == 'B' || subopcion[0] == 'b' || subopcion[0] == 'C' || subopcion[0] == 'c')) {
            switch (toupper(subopcion[0])) {
                case 'A':
                    reporteVentaDelDia();
                    break;
                case 'B':
                    reporteVentaPorCliente();
                    break;
                case 'C':
                    printf("\nRegresando al submenu de ventas...\n");
                    sleep(2);
                    system("cls");
                    salir = 1; // Cambiar la bandera para salir del bucle
                    break;
                default:
                    printf("\nOpcion no valida. Ingresa una opcion correcta\n");
                    break;
            }
        } else {
            printf("\nOpcion no valida. Ingrese una opcion correcta\n");
            sleep(2);
            system("cls");
        }
    } while (!salir);
}

// Funcion para mostrar el menu de ventas
void submenuVentas() {
    system("cls");
    int opcion;
    do {
        printf("\n--- Submenu de Ventas ---\n");
        printf("\n1. Venta\n");
        printf("2. Reporte Venta\n");
        printf("3. Regresar menu principal\n");

        opcion = validarEntero("\nSeleccione una opcion: ");

        switch(opcion) {
            case 1:
                realizarVenta();
                break;
            case 2:
                reporteVenta();
                break;
            case 3:
                printf("\nRegresando al menu principal...\n");
                sleep(2);
                system("cls");
                break;
            default:
                printf("\nOpcion no valida. Ingresa una opcion correcta\n");
                sleep(1);
                system("cls");
                break;
        }
    } while(opcion != 3);
}
