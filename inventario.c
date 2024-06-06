#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "conexiones.h"
#define MAX_PRODUCTOS 100
#define MAX_REABASTECIMIENTOS 100

// Funcion para validar si un año es bisiesto
bool esBisiesto(int anio) {
    if ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0) {
        return true;
    }
    return false;
}

// Funcion para validar una fecha
bool validarFecha(int dia, int mes, int anio) {
    if (mes < 1 || mes > 12) {
        return false;
    }
    if (dia < 1 || dia > 31) {
        return false;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        if (dia > 30) {
            return false;
        }
    } else if (mes == 2) {
        if (esBisiesto(anio)) {
            if (dia > 29) {
                return false;
            }
        } else {
            if (dia > 28) {
                return false;
            }
        }
    }
    return true;
}

// Funcion auxiliar para validar el formato de fecha "dd/mm/aaaa"
bool validarFormatoFecha(const char *fecha) {
    int dd, mm, aaaa;
    return sscanf(fecha, "%d/%d/%d", &dd, &mm, &aaaa) == 3;
}

// Funcion para mostrar el nombre del tipo de producto
const char* obtenerNombreTipoProducto(int tipo) {
    switch (tipo) {
        case 0: return "Completo";
        case 1: return "Escritorios";
        case 2: return "Archiveros";
        case 3: return "Sillas";
        case 4: return "Mesas de centro";
        case 5: return "Mesas ejecutivas";
        case 6: return "Sofas";
        case 7: return "Decorativos";
        default: return "Desconocido";
    }
}

// Funcion para validar la entrada S/N
char obtenerOpcionSN() {
    char opcion[10];
    while (1) {
        printf("\nDesea regresar al submenu de reportes? (S/N): ");
        scanf("%s", opcion);
        if (strlen(opcion) == 1 && (opcion[0] == 'S' || opcion[0] == 's' || opcion[0] == 'N' || opcion[0] == 'n')) {
            return opcion[0];
        } else {
            printf("\nOpcion no valida. Por favor, ingrese S o N.\n");
        }
    }
}

// Funcion para generar un nuevo folio
int generarFolio() {
    FILE *archivoReabastecimiento;
    Reabastecimiento reabastecimiento;
    int folio = 1;

    archivoReabastecimiento = fopen("reabastecimiento.txt", "r");
    if (archivoReabastecimiento != NULL) {
        while (fscanf(archivoReabastecimiento, "%d,%d,%[^,],%d", &reabastecimiento.folio, &reabastecimiento.clave_producto, reabastecimiento.fecha, &reabastecimiento.cantidad_reabastecida) == 4) {
            folio = reabastecimiento.folio + 1;
        }
        fclose(archivoReabastecimiento);
    }

    return folio;
}

// Funcion para obtener la fecha actual en formato "dd/mm/yyyy"
void obtenerFechaActual(char *fecha) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(fecha, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void reabastecerProducto() {
    system("cls");
    int tipo, clave, cantidad, encontrado = 0, i;
    char opcion, entrada[10];
    FILE *archivoProductos, *archivoReabastecimiento;
    Producto producto;
    Producto productos[MAX_PRODUCTOS];
    int numProductos = 0;

    do {
        system("cls");
        do {
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

        system("cls");

        archivoProductos = fopen("productos.txt", "r");
        if (archivoProductos == NULL) {
            printf("Error al abrir el archivo de productos.\n");
            return;
        }

        printf("\nProductos disponibles:\n");
        printf("Clave   Descripcion             Cantidad Existencia\n");
        printf("---------------------------------------------------\n");

        // Leer todos los productos en memoria
        while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
            productos[numProductos++] = producto;
            if (producto.tipo == tipo && producto.estatus == 1) {
                printf("%-8d%-24s%-9d\n", producto.clave, producto.descripcion, producto.cantidad_existencia);
            }
        }
        fclose(archivoProductos);

        clave = validarEntero("\nIngrese la clave del producto a reabastecer: ");

        for (i = 0; i < numProductos; i++) {
            if (productos[i].clave == clave) {

                encontrado = 1;
                printf("\n-----------------------");
                printf("\nProducto encontrado:\n");
                printf("Clave: %d\n", productos[i].clave);
                printf("Descripcion: %s\n", productos[i].descripcion);
                printf("Cantidad en existencia: %d\n", productos[i].cantidad_existencia);
                printf("Nivel de stock: %d\n", productos[i].nivel_stock);
                printf("Precio: %.2f\n", productos[i].precio);
                printf("Estatus: %d\n", productos[i].estatus);

                do {
                    printf("\nIngrese la cantidad a reabastecer: ");
                    scanf("%s", entrada);

                    if (!validarNumero(entrada)) {
                        printf("\nCantidad no valida. Ingrese solo numeros!\n");
                    }
                } while (!validarNumero(entrada));
                cantidad = atoi(entrada);

                productos[i].cantidad_existencia += cantidad;

                // Guardar el reabastecimiento en el archivo reabastecimiento.txt
                archivoReabastecimiento = fopen("reabastecimiento.txt", "a");
                if (archivoReabastecimiento == NULL) {
                    printf("Error al abrir el archivo de reabastecimiento.\n");
                    return;
                }
                Reabastecimiento nuevoReabastecimiento;
                nuevoReabastecimiento.folio = generarFolio();
                nuevoReabastecimiento.clave_producto = clave;
                obtenerFechaActual(nuevoReabastecimiento.fecha);
                nuevoReabastecimiento.cantidad_reabastecida = cantidad;
                fprintf(archivoReabastecimiento, "%04d,%d,%s,%d\n", nuevoReabastecimiento.folio, nuevoReabastecimiento.clave_producto, nuevoReabastecimiento.fecha, nuevoReabastecimiento.cantidad_reabastecida);
                fclose(archivoReabastecimiento);

                break;
            }
        }

        if (encontrado) {
            archivoProductos = fopen("productos.txt", "w");
            if (archivoProductos == NULL) {
                printf("Error al abrir el archivo de productos.\n");
                return;
            }
            for (i = 0; i < numProductos; i++) {
                fprintf(archivoProductos, "%d,%s,%d,%d,%d,%.2f,%d\n", productos[i].clave, productos[i].descripcion, productos[i].tipo, productos[i].cantidad_existencia, productos[i].nivel_stock, productos[i].precio, productos[i].estatus);
            }
            fclose(archivoProductos);
            printf("\nProducto reabastecido con exito!\n");
        } else {
            printf("\nProducto no encontrado.\n");
            fflush(stdin);
        }

        char opcion = obtenerOpcionSN();
        if (opcion == 'S' || opcion == 's') {
            printf("\nRegresando al submenu de inventario...\n");
            sleep(1);
            system("cls");
            submenuInventario();
        } else {
            printf("\nRegresando al submenu de inventario...\n");
            sleep(1);
            system("cls");
            submenuInventario();
        }
    } while (opcion == 'S' || opcion == 's');
}

void reporteCompleto() {
    system("cls");
    FILE *archivoProductos;
    Producto producto;
    char fecha[11];

    archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    obtenerFechaActual(fecha);

    printf("\t  Comercializadora Fuentes\n");
    printf("\t\tInventario\n\n");
    printf("\nFecha: %s\n", fecha);
    printf("\nProducto                    Clave               Cantidad en Existencia\n");
    printf("----------------------------------------------------------------------\n");

    // Leer y mostrar los productos activos
    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
        if (producto.estatus == 1) {
            printf("%-28s%-20d%-9d\n", producto.descripcion, producto.clave, producto.cantidad_existencia);
        }
    }

    fclose(archivoProductos);

    char opcion = obtenerOpcionSN();
    if (opcion == 'S' || opcion == 's') {
        printf("\nRegresando al submenu de reportes...\n");
        sleep(1);
        system("cls");
        mostrarMenuReportes();
    } else {
        printf("\nRegresando al submenu de inventario...\n");
        sleep(1);
        system("cls");
        submenuInventario();
    }
}

void reportePorTipoProducto() {
    int tipo, i;
    char fecha[11];
    char entrada[10];
    Producto productos[MAX_PRODUCTOS];
    int numProductos = 0;
    FILE *archivoProductos;
    Producto producto;

    system("cls");

    obtenerFechaActual(fecha);

    // Leer los productos del archivo
    archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
        productos[numProductos++] = producto;
    }
    fclose(archivoProductos);

    do {
        printf("\n--- Tipos de Producto ---\n");
        printf("0. Todos los productos\n");
        printf("1. Escritorios\n");
        printf("2. Archiveros\n");
        printf("3. Sillas\n");
        printf("4. Mesas de centro\n");
        printf("5. Mesas ejecutivas\n");
        printf("6. Sofas\n");
        printf("7. Decorativos\n");
        printf("\nSeleccione el tipo de producto (0-7): ");
        scanf("%s", entrada);

        if (!validarNumero(entrada) || (tipo = atoi(entrada)) < 0 || tipo > 7) {
            printf("\n-------------------------------------------------------\n");
            printf("Tipo de producto no valido. Ingrese un numero correcto!\n");
            printf("-------------------------------------------------------\n");
            sleep(2);
            system("cls");
        }
    } while (!validarNumero(entrada) || (tipo = atoi(entrada)) < 0 || tipo > 7);

    system("cls");

    // Mostrar el encabezado del reporte
    printf("\t\t    Comercializadora Fuentes\n");
    printf("\t\t   Inventario por %s:\n", obtenerNombreTipoProducto(tipo));
    printf("\nFecha: %s\n", fecha);

    if (tipo == 0) {
        printf("\nInventario Completo:\n");
    } else {
        printf("\nInventario de %s:\n", obtenerNombreTipoProducto(tipo));
    }
    printf("\nProducto                    Clave               Cantidad en Existencia\n");
    printf("----------------------------------------------------------------------\n");

    // Mostrar los productos segun el tipo seleccionado
    for (i = 0; i < numProductos; i++) {
        if (productos[i].estatus == 1 && (tipo == 0 || productos[i].tipo == tipo)) {
            printf("%-28s%-20d%-9d\n", productos[i].descripcion, productos[i].clave, productos[i].cantidad_existencia);
        }
    }

    char opcion = obtenerOpcionSN();
    if (opcion == 'S' || opcion == 's') {
        printf("\nRegresando al submenu de reportes...\n");
        sleep(1);
        system("cls");
        mostrarMenuReportes();
    } else {
        printf("\nRegresando al submenu de inventario...\n");
        sleep(1);
        system("cls");
        submenuInventario();
    }
}

void reporteNivelDeStock() {
    system("cls");
    int diferencia, i;
    float porcentajeFaltante;
    FILE *archivoProductos;
    Producto productos[MAX_PRODUCTOS];
    Producto producto;
    int numProductos = 0;

    // Abrir el archivo de productos en modo lectura
    archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    // Leer los productos del archivo
    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
        productos[numProductos++] = producto;
    }

    fclose(archivoProductos);

    // Mostrar el encabezado del reporte
    printf("Reporte de Nivel de Stock\n\n");
    printf("Clave  Producto                     Cant En existencia   Cant de Stock    Dif     Faltante de Stock\n");
    printf("---------------------------------------------------------------------------------------------------\n");

    // Calcular y mostrar los productos con escasez o excedente de stock
    for (int i = 0; i < numProductos; i++) {
        int diferencia = productos[i].cantidad_existencia - productos[i].nivel_stock;
        int porcentajeFaltante = 0;

        // Clasificar si falta o excedente de stock
        char tipoDiferencia;
        if (diferencia < 0) {
            tipoDiferencia = 'F'; // Faltante
            diferencia *= -1; // Convertir la diferencia a positiva
            // Calcular el porcentaje solo si hay diferencia
            if (productos[i].nivel_stock != 0) {
                porcentajeFaltante = (diferencia * 100) / productos[i].nivel_stock;
            }
        } else {
            tipoDiferencia = 'E'; // En orden
        }

        // Mostrar la información del producto
        printf("%-6d %-28s %-20d %-16d %-13d %d%% %c\n", productos[i].clave, productos[i].descripcion, productos[i].cantidad_existencia, productos[i].nivel_stock, diferencia, porcentajeFaltante, tipoDiferencia);
    }

    printf("\nE: En orden\n");
    printf("F: Faltante\n");

    char opcion = obtenerOpcionSN();
    if (opcion == 'S' || opcion == 's') {
        printf("\nRegresando al submenu de reportes...\n");
        sleep(1);
        system("cls");
        mostrarMenuReportes();
    } else {
        printf("\nRegresando al submenu de inventario...\n");
        sleep(1);
        system("cls");
        submenuInventario();
    }
}

void reporteReabastecimiento() {
    system("cls");

    int numProductos = 0;
    Producto productos[MAX_PRODUCTOS];

    // Abrir el archivo de productos en modo lectura
    FILE *archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    // Leer los productos del archivo
    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d", &productos[numProductos].clave, productos[numProductos].descripcion, &productos[numProductos].tipo, &productos[numProductos].cantidad_existencia, &productos[numProductos].nivel_stock, &productos[numProductos].precio, &productos[numProductos].estatus) == 7) {
        numProductos++;
    }
    fclose(archivoProductos);

    int numReabastecimientos = 0, i , j;
    FILE *archivoReabastecimientos;
    Reabastecimiento reabastecimiento;
    Reabastecimiento reabastecimientos[MAX_REABASTECIMIENTOS];

    // Abrir el archivo de reabastecimientos en modo lectura
    archivoReabastecimientos = fopen("reabastecimiento.txt", "r");
    if (archivoReabastecimientos == NULL) {
        printf("Error al abrir el archivo de reabastecimientos.\n");
        return;
    }

    // Leer los reabastecimientos del archivo
    while (fscanf(archivoReabastecimientos, "%d,%d,%10[^,],%d", &reabastecimiento.folio, &reabastecimiento.clave_producto, reabastecimiento.fecha, &reabastecimiento.cantidad_reabastecida) == 4) {
        reabastecimientos[numReabastecimientos++] = reabastecimiento;
    }

    fclose(archivoReabastecimientos);

    // Solicitar fechas inicial y final al usuario
    char fechaInicio[11], fechaFin[11];
    int diaInicio, mesInicio, anioInicio, diaFin, mesFin, anioFin;
    do {
        printf("\nIngrese la fecha inicial (dd/mm/aaaa): ");
        scanf("%10s", fechaInicio);
        fflush(stdin);
        sscanf(fechaInicio, "%d/%d/%d", &diaInicio, &mesInicio, &anioInicio);
        if (!validarFecha(diaInicio, mesInicio, anioInicio)) {
            printf("\nFecha invalida. Por favor, ingrese una fecha valida.\n");
        }
    } while (!validarFecha(diaInicio, mesInicio, anioInicio));

    do {
        printf("\nIngrese la fecha final (dd/mm/aaaa): ");
        scanf("%10s", fechaFin);
        fflush(stdin);
        sscanf(fechaFin, "%d/%d/%d", &diaFin, &mesFin, &anioFin);
        if (!validarFecha(diaFin, mesFin, anioFin)) {
            printf("\nFecha invalida. Por favor, ingrese una fecha valida.\n");
        }
    } while (!validarFecha(diaFin, mesFin, anioFin));

    system("cls");

    // Mostrar el encabezado del reporte
    printf("\n\n  \t\t  Comercializadora Fuentes\n");
    printf("\t        Reabastecimiento de productos\n");
    printf("\ndel %s al %s\n\n", fechaInicio, fechaFin);
    printf("Clave   Fecha         No Pedido   Cantidad Pedida   Nivel Stock\n");
    printf("---------------------------------------------------------------\n");

    int encontrado = 0;

    // Filtrar y mostrar los reabastecimientos en el rango de fechas
    for (i = 0; i < numReabastecimientos; i++) {
        if (strcmp(reabastecimientos[i].fecha, fechaInicio) >= 0 && strcmp(reabastecimientos[i].fecha, fechaFin) <= 0) {
            // Actualiza la bandera a true si se encuentra al menos un dato
            encontrado = 1;
            // Buscar el nivel de stock del producto
            int nivel_stock = 0;
            for (j = 0; j < numProductos; j++) {
                if (productos[j].clave == reabastecimientos[i].clave_producto) {
                    nivel_stock = productos[j].nivel_stock;
                    break;
                }
            }
            // Mostrar la información del reabastecimiento
            printf("%-7d %-15s %-14d %-16d %-10d\n", reabastecimientos[i].clave_producto, reabastecimientos[i].fecha, reabastecimientos[i].folio, reabastecimientos[i].cantidad_reabastecida, nivel_stock);
        }
    }

    // Verificar si se encontraron datos
    if (!encontrado) {
        printf("\nNo se encontraron datos en las fechas proporcionadas.\n");
    }

    char opcion = obtenerOpcionSN();
    if (opcion == 'S' || opcion == 's') {
        printf("\nRegresando al submenu de reportes...\n");
        sleep(1);
        system("cls");
        mostrarMenuReportes();
    } else {
        printf("\nRegresando al submenu de inventario...\n");
        sleep(1);
        system("cls");
        submenuInventario();
    }
}

void mostrarMenuReportes() {
    int opcion;
    system("cls");
    do {
        printf("\nMenu de Reportes de Inventario\n");
        printf("\n1. Completo\n");
        printf("2. Por Tipo de Producto\n");
        printf("3. Reporte Nivel de Stock\n");
        printf("4. Reabastecimiento\n");
        printf("5. Regresar al Submenu Inventario\n");
        opcion = validarEntero("\nSeleccione una opcion: ");

        switch(opcion) {
            case 1:
                reporteCompleto();
                break;
            case 2:
                reportePorTipoProducto();
                break;
            case 3:
                reporteNivelDeStock();
                break;
            case 4:
                reporteReabastecimiento();
                break;
            case 5:
                printf("\nRegresando al submenu inventario...\n");
                sleep(2);
                system("cls");
                submenuInventario();
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                break;
        }
    } while(opcion != 5);
}

void submenuInventario() {
    int opcion;
    do {
        system("cls");
        printf("\n--- Submenu de Inventario ---\n\n");
        printf("1. Reabastecer Producto\n");
        printf("2. Reportes Inventario\n");
        printf("3. Regresar al Menu Principal\n");
        opcion = validarEntero("\nSeleccione una opcion: ");

        switch(opcion) {
            case 1:
                reabastecerProducto();
                break;
            case 2:
                mostrarMenuReportes();
                break;
            case 3:
                printf("\nRegresando al menu principal...\n");
                sleep(2);
                system("cls");
                mostrarMenuAdmin();
                break;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
                sleep(1);
                system("cls");
                break;
        }
    } while(opcion != 3);
}
