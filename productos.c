// producto.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "conexiones.h"

// Funcion para validar la entrada S/N
char obtenerOpcionSNProductos() {
    char opcion[10];
    while (1) {
        printf("\nDesea regresar al submenu de productos? (S/N): ");
        scanf("%s", opcion);
        if (strlen(opcion) == 1 && (opcion[0] == 'S' || opcion[0] == 's' || opcion[0] == 'N' || opcion[0] == 'n')) {
            return opcion[0];
        } else {
            printf("\nOpcion no valida. Por favor, ingrese S o N.\n");
        }
    }
}

// Funcion para validar que la entrada es un numero
bool validarNumero(const char *entrada) {
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] < '0' || entrada[i] > '9') {
            return false;
        }
    }
    return true;
}

// Funcion para validar y obtener un numero entero
int validarEntero(const char *mensaje) {
    char entrada[100];
    int numero;
    while (true) {
        printf("%s", mensaje);
        scanf("%s", entrada);
        if (validarNumero(entrada)) {
            numero = atoi(entrada);
            break;
        } else {
            printf("\nPor favor, ingrese un numero valido.\n");
        }
    }
    return numero;
}

// Funcion para validar y obtener un numero flotante
float validarFlotante(const char *mensaje) {
    char entrada[100];
    float numero;
    while (true) {
        printf("%s", mensaje);
        scanf("%s", entrada);
        if (validarNumero(entrada)) {
            numero = atof(entrada);
            break;
        } else {
            printf("Por favor, ingrese un numero valido.\n");
        }
    }
    return numero;
}

// Funcion para agregar un nuevo producto
void nuevoProducto() {
    system("cls");
    Producto producto;
    FILE *archivo;
    int clave;

    // Validar la clave del producto
    clave = validarEntero("\nIngrese la clave del producto: ");
    producto.clave = clave;
    system("cls");

    // Comprobamos si el producto ya existe
    archivo = fopen("productos.txt", "r");
    if (archivo != NULL) {
        Producto temp;
        while (fscanf(archivo, "%d,%[^,],%d,%d,%d,%f,%d", &temp.clave, temp.descripcion, &temp.tipo, &temp.cantidad_existencia, &temp.nivel_stock, &temp.precio, &temp.estatus) == 7) {
            if (temp.clave == producto.clave) {
                fclose(archivo);
                printf("\nLa clave de producto ya existe.\n");
                sleep(1);
                system("cls");
                return;
            }
        }
        fclose(archivo);
    }

    fflush(stdin);

    printf("\nIngrese la descripcion del producto: ");
    scanf(" %[^\n]s", producto.descripcion);

    printf("\n \t--- Seleccione el tipo de producto ---\n");
    printf("1. Escritorios\n");
    printf("2. Archiveros\n");
    printf("3. Sillas\n");
    printf("4. Mesas de centro\n");
    printf("5. Mesas ejecutivas\n");
    printf("6. Sofas\n");
    printf("7. Decorativos\n");

    producto.tipo = validarEntero("\nIngrese el numero correspondiente al tipo de producto: ");
    while (producto.tipo < 1 || producto.tipo > 7) {
        printf("\n-------------------------------------------------------\n");
        printf("Tipo de producto no valido. Ingrese un numero correcto!\n");
        printf("-------------------------------------------------------\n");
        producto.tipo = validarEntero("\nIngrese el numero correspondiente al tipo de producto: ");
    }

    producto.cantidad_existencia = validarEntero("\nIngrese la cantidad de existencia del producto: ");
    producto.nivel_stock = validarEntero("\nIngrese el nivel de stock del producto [minima]: ");
    producto.precio = validarFlotante("\nIngrese el precio del producto: ");

    producto.estatus = 1; // Por defecto, el producto esta activo

    archivo = fopen("productos.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%d,%d,%d,%.2f,%d\n", producto.clave, producto.descripcion, producto.tipo, producto.cantidad_existencia, producto.nivel_stock, producto.precio, producto.estatus);
    fclose(archivo);

    printf("\nProducto agregado con exito.\n");

    sleep(2);
    system("cls");
}

void editarProducto() {
    system("cls");
    int clave;
    FILE *archivo, *temporal;
    Producto producto;

    // Validar la clave del producto
    clave = validarEntero("\nIngrese la clave del producto a editar: ");
    system("cls");

    // Abrir el archivo en modo lectura
    archivo = fopen("productos.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    // Crear un archivo temporal para escribir los datos modificados
    temporal = fopen("temporal.txt", "w");
    if (temporal == NULL) {
        printf("Error al abrir el archivo temporal.\n");
        fclose(archivo);
        return;
    }

    // Buscar el producto por su clave y escribir los datos en el archivo temporal
    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%d,%d,%d,%f,%d\n", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
        if (producto.clave == clave) {
            encontrado = 1;
            printf("\n--- Producto encontrado ---\n");
            printf("Clave: %d\n", producto.clave);
            printf("Descripcion: %s\n", producto.descripcion);
            printf("Tipo: %d\n", producto.tipo);
            printf("Cantidad en existencia: %d\n", producto.cantidad_existencia);
            printf("Nivel de stock: %d\n", producto.nivel_stock);
            printf("Precio: %.2f\n", producto.precio);
            printf("Estatus: %d\n", producto.estatus);

            int opcion;
            opcion = validarEntero("\nSeleccione el campo a modificar:\n1. Precio\n2. Nombre de producto\nIngrese su opcion: ");

            switch (opcion) {
                case 1:
                    printf("Ingrese el nuevo precio: ");
                    scanf("%f", &producto.precio);
                    break;
                case 2:
                    printf("Ingrese el nuevo nombre del producto: ");
                    scanf(" %[^\n]s", producto.descripcion);
                    break;
                default:
                    printf("Opción no valida.\n");
                    fclose(archivo);
                    fclose(temporal);
                    remove("temporal.txt"); // Eliminar el archivo temporal
                    return;
            }
        }
        fprintf(temporal, "%d,%s,%d,%d,%d,%.2f,%d\n", producto.clave, producto.descripcion, producto.tipo, producto.cantidad_existencia, producto.nivel_stock, producto.precio, producto.estatus);
    }

    fclose(archivo);
    fclose(temporal);

    // Reemplazar el archivo original con el archivo temporal
    remove("productos.txt");
    rename("temporal.txt", "productos.txt");

    if (!encontrado) {
        printf("El producto con la clave %d no fue encontrado.\n", clave);
    } else {
        printf("\nProducto editado correctamente.\n");
    }

    sleep(2);
    system("cls");
}

void mostrarProducto(Producto producto) {
    printf("%-8d %-20s %-20d %-10.2f %-8d\n", producto.clave, producto.descripcion, producto.cantidad_existencia, producto.precio, producto.estatus);
}

void consultaPorTipoProducto() {
    system("cls");
    int tipo;
    char fecha[11];
    char entrada[10];
    FILE *archivo;
    Producto producto;

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

    // Abrir el archivo de productos en modo lectura
    archivo = fopen("productos.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    system("cls");
    obtenerFechaActual(fecha);

    printf("\t\t  Comercializadora Fuentes\n");
    printf("\t\t   aConsulta por Producto\n\n");
    printf("\nFecha: %s\n", fecha);
    printf("\n%-8s %-20s %-20s %-10s %s\n", "Clave", "Descripcion", "Cant. Existencia", "Precio", "Estatus");
    printf("---------------------------------------------------------------------\n");

    // Mostrar productos del tipo especificado
    while (fscanf(archivo, "%d,%[^,],%d,%d,%d,%f,%d", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
        if (producto.tipo == tipo) {
            mostrarProducto(producto);
        }
    }

    fclose(archivo);

    char volverMenu = obtenerOpcionSNProductos("\nDesea regresar al submenu productos? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu productos...\n");
        sleep(2);
        system("cls");
        submenuProductos();
    }
    else{
        consultaPorTipoProducto();
    }
}

void consultaPorClave() {
    system("cls");
    int clave;
    char fecha[11];
    FILE *archivo;
    Producto producto;

    clave = validarEntero("\nIngrese la clave del producto: ");

    // Abrir el archivo de productos en modo lectura
    archivo = fopen("productos.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    // Buscar el producto por su clave
    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%d,%d,%d,%f,%d", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidad_existencia, &producto.nivel_stock, &producto.precio, &producto.estatus) == 7) {
        if (producto.clave == clave) {
            encontrado = 1;
            break;
        }
    }

    fclose(archivo);

    system("cls");
    obtenerFechaActual(fecha);

    // Mostrar resultados
    if (encontrado) {
        printf("\t\t  Comercializadora Fuentes\n");
        printf("\t\t   Consulta por Producto\n\n");
        printf("\nFecha: %s\n", fecha);
        printf("\n---------------------------------------------------------------\n");
        printf("Clave: %-40d Estatus: %-8d\n", producto.clave, producto.estatus);
        printf("Descripcion: %-66s\n", producto.descripcion);
        printf("Cantidad en existencia: %-23d Precio: %.2f\n", producto.cantidad_existencia, producto.precio);
        printf("\n---------------------------------------------------------------");
    } else {
        printf("El producto con la clave %d no fue encontrado.\n", clave);
    }

    char volverMenu = obtenerOpcionSNProductos("\nDesea regresar al submenu productos? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu productos...\n");
        sleep(2);
        system("cls");
        submenuProductos();
    }
    else{
        consultaPorClave();
    }
}

void submenuProductos() {
    int opcion;
    char subopcion[10];

    do {
        system("cls");
        // Mostrar submenu de productos
        printf("\n--- Submenu de Productos ---\n\n");
        printf("1. Nuevo Producto\n");
        printf("2. Editar Producto\n");
        printf("3. Consulta productos\n");
        printf("4. Regresar al menu principal\n");

        opcion = validarEntero("\nSeleccione una opcion: ");

        switch (opcion) {
            case 1:
                nuevoProducto();
                break;
            case 2:
                editarProducto();
                break;
            case 3:
                system("cls");
                // Submenú para consulta de productos
                printf("\n --- Consulta de Productos ---\n");
                printf("\nA. Por Tipo de Producto\n");
                printf("B. Por Clave\n");
                printf("\nIngrese su opcion: ");
                scanf("%s", subopcion);

                if (strlen(subopcion) == 1 && (subopcion[0] == 'A' || subopcion[0] == 'a' || subopcion[0] == 'B' || subopcion[0] == 'b')) {
                    switch (subopcion[0]) {
                        case 'A':
                        case 'a':
                            consultaPorTipoProducto();
                            break;
                        case 'B':
                        case 'b':
                            consultaPorClave();
                            break;
                    }
                } else {
                    printf("\nOpcion no valida\n");
                    sleep(2);
                }
                system("cls");
                break;
            case 4:
                printf("\nRegresando al menu principal...\n");
                sleep(2);
                system("cls");
                mostrarMenuAdmin();
                break;
            default:
                printf("\nOpcion no valida. Ingresa una opcion correcta\n");
                sleep(1);
                system("cls");
                break;
        }
    } while (opcion != 4);
}
