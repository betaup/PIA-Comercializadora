#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "conexiones.h"

char obtenerOpcionSNClientes() {
    char opcion[10];
    while (1) {
        printf("\nDesea regresar al submenu de clientes? (S/N): ");
        scanf("%s", opcion);
        if (strlen(opcion) == 1 && (opcion[0] == 'S' || opcion[0] == 's' || opcion[0] == 'N' || opcion[0] == 'n')) {
            return opcion[0];
        } else {
            printf("\nOpcion no valida. Por favor, ingrese S o N.\n");
        }
    }
}

bool es_descuento_valido(int descuento) {
    return descuento >= 0 && descuento <= 15;
}

bool es_fecha_valida(struct Cliente cliente) {
    bool es_bisiesto = (cliente.fecha.year % 4 == 0 && cliente.fecha.year % 100 != 0) || (cliente.fecha.year % 400 == 0);
    int dias_por_mes[] = {31, 28 + es_bisiesto, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (cliente.fecha.mes < 1 || cliente.fecha.mes > 12)
        return false;

    if (cliente.fecha.dia < 1 || cliente.fecha.dia > dias_por_mes[cliente.fecha.mes - 1])
        return false;

    return true;
}

void generarRFC(struct Cliente *cliente) {
    const char letras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char numeros[] = "0123456789";
    const int longitud_letras = strlen(letras);
    const int longitud_numeros = strlen(numeros);


    cliente->RFC[0] = toupper(cliente->Paterno[0]);
    cliente->RFC[1] = toupper(cliente->Paterno[1]);

    cliente->RFC[2] = toupper(cliente->Materno[0]);

    cliente->RFC[3] = toupper(cliente->Nombre[0]);

    int yearDigits = cliente->fecha.year % 100;
    cliente->RFC[4] = numeros[yearDigits / 10];
    cliente->RFC[5] = numeros[yearDigits % 10];
    cliente->RFC[6] = numeros[cliente->fecha.mes / 10];
    cliente->RFC[7] = numeros[cliente->fecha.mes % 10];
    cliente->RFC[8] = numeros[cliente->fecha.dia / 10];
    cliente->RFC[9] = numeros[cliente->fecha.dia % 10];


    for (int i = 10; i < 13; ++i) {
        if (rand() % 2 == 0) {
            cliente->RFC[i] = letras[rand() % longitud_letras];
        } else {
            cliente->RFC[i] = numeros[rand() % longitud_numeros];
        }
    }

    cliente->RFC[13] = '\0';
}

void nuevoCliente() {
    system("cls");
    struct Cliente cliente;
    char c;
    FILE *archivo;

    do {
        printf("Ingrese el ID del cliente: ");
        if (scanf("%d", &cliente.Id) != 1 || getchar() != '\n') {
            printf("El ID debe ser un numero entero. Intentalo de nuevo.\n");
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar el bufer de entrada
        } else {
            break;
        }
    } while (true);

    archivo = fopen("clientes.txt", "r");
    if (archivo != NULL) {
        struct Cliente temp;
        while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d", &temp.Id, temp.Nombre, temp.Paterno, temp.Materno, temp.RFC, temp.Correo, temp.Telefono, &temp.descuento, temp.direccion.Calle, temp.direccion.Colonia, temp.direccion.Numero, temp.direccion.CP, &temp.Estatus) == 13) {
            if (temp.Id == cliente.Id) {
                fclose(archivo);
                printf("\nEl cliente ya existe.\n");
                sleep(1);
                system("cls");
                return;
            }
        }
        fclose(archivo);
    }


    fflush(stdin);

     do {
            printf("Ingrese el nombre del cliente: ");
            scanf(" %[^\n]s", cliente.Nombre);

            if (!contieneSoloLetras(cliente.Nombre)) {
            printf("El nombre debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
        }
    } while (!contieneSoloLetras(cliente.Nombre));

    do {
            printf("Ingrese el apellido paterno del cliente: ");
            scanf(" %[^\n]s", cliente.Paterno);

            if (!contieneSoloLetras(cliente.Paterno)) {
            printf("El apellido debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(cliente.Paterno));

    do {
            printf("Ingrese el apellido materno del cliente: ");
            scanf(" %[^\n]s", cliente.Materno);

            if (!contieneSoloLetras(cliente.Materno)) {
            printf("El apellido tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(cliente.Materno));

   do {
        printf("Ingrese la fecha de nacimiento del cliente (DD/MM/YYYY): ");
        if (scanf("%d/%d/%d", &cliente.fecha.dia, &cliente.fecha.mes, &cliente.fecha.year) != 3) {
            printf("Formato de fecha inválido. ");
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar el búfer de entrada
        } else if (!es_fecha_valida(cliente)) {
            printf("Fecha inválida. Inténtelo de nuevo.\n");
        }
    } while (!es_fecha_valida(cliente));


    generarRFC(&cliente);
    printf ("RFC: %s\n", cliente.RFC);

    do {
    printf("Ingrese el correo electronico del cliente: ");
    scanf(" %[^\n]", cliente.Correo);

    if (!validarCorreo(cliente.Correo)) {
        printf("Correo electronico invalido. Inténtalo de nuevo.\n");
    }
    } while (!validarCorreo(cliente.Correo));

    do {
        printf("Ingrese su numero de telefono (10 digitos): ");
        scanf("%s", cliente.Telefono);

        if (!validarTelefono(cliente.Telefono)) {
            printf("El telefono costa de 10 dígitos. Intentalo de nuevo.\n");
        }
    } while (!validarTelefono(cliente.Telefono));

     do {
        printf("Ingrese el descuento del cliente (0-15): ");
        if (scanf("%d", &cliente.descuento) != 1) {
            printf("Entrada inválida. ");
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar el búfer de entrada
        } else if (!es_descuento_valido(cliente.descuento)) {
            printf("Descuento fuera de rango. Inténtelo de nuevo.\n");
        }
    } while (!es_descuento_valido(cliente.descuento));

    do {
            printf("Ingrese la calle de la direccion del cliente: ");
            scanf(" %[^\n]s", cliente.direccion.Calle);

            if (!contieneSoloLetras(cliente.direccion.Calle)) {
            printf("La calle tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(cliente.direccion.Calle));

    do {
            printf("Ingrese la colonia de la direccion del cliente: ");
            scanf(" %[^\n]s", cliente.direccion.Colonia);

            if (!contieneSoloLetras(cliente.direccion.Colonia)) {
            printf("La calle debe tener por lo menos una mayuscula y 3 letras.. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(cliente.direccion.Colonia));

    do{
            printf("Ingrese el numero de la direccion del cliente: ");
            scanf(" %[^\n]s", cliente.direccion.Numero);

            if (!validarNumeroCasa(cliente.direccion.Numero)) {
            printf("Numero invalido.Intentalo de nuevo.\n");
        }
    } while (!validarNumeroCasa(cliente.direccion.Numero));

    do{
           printf("Ingrese el codigo postal de la direccion del cliente: ");
           scanf(" %[^\n]s", cliente.direccion.CP);

            if (!validarCP(cliente.direccion.CP)) {
            printf("CP invalido.Intentalo de nuevo.\n");
        }
    } while (!validarCP(cliente.direccion.CP));

    cliente.Estatus = 1;

    archivo = fopen("clientes.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de usuarios.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s,%d\n", cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno, cliente.RFC, cliente.Correo, cliente.Telefono, cliente.descuento, cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero, cliente.direccion.CP, cliente.Estatus);
    fclose(archivo);

    printf("\nCliente agregado con exito.\n");

    char volverMenu = obtenerOpcionSNClientes("\nDesea regresar al submenu clientes? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu clientes...\n");
        sleep(2);
        system("cls");
        submenuClientes();
    }
    else{
        printf("\nRegresando a Nuevo Cliente...\n");
        sleep(1);
        system("cls");
        nuevoCliente();
    }
}

void eliminarCliente() {
    system("cls");
    int idCliente;
    char confirmacion;
    FILE *archivo, *temporal;
    Cliente cliente;

    idCliente = validarEntero("\nIngrese el ID del cliente a eliminar: ");
    system("cls");

    archivo = fopen("clientes.txt", "r");
    temporal = fopen("temporal.txt", "w");
    if (archivo == NULL || temporal == NULL) {
        printf("Error al abrir el archivo de clientes.\n");
        return;
    }

    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d", &cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno, cliente.RFC, cliente.Correo, cliente.Telefono, &cliente.descuento, cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero, cliente.direccion.CP, &cliente.Estatus) == 13) {
        if (cliente.Id == idCliente) {
            encontrado = 1;
            if (cliente.Estatus == 0) {
                printf("\nEl cliente con ID '%d' ya esta eliminado.\n", idCliente);
                fclose(archivo);
                fclose(temporal);
                remove("temporal.txt"); // Eliminar el archivo temporal
                printf("\nVolviendo al menu de clientes...\n");
                sleep(2);
                system("cls");
                return;
            }
            printf("\nCliente encontrado:\n");
            printf("ID: %d\n", cliente.Id);
            printf("Nombre: %s %s %s\n", cliente.Nombre, cliente.Paterno, cliente.Materno);
            printf("RFC: %s\n", cliente.RFC);
            printf("Correo: %s\n", cliente.Correo);
            printf("Telefono: %s\n", cliente.Telefono);

            printf("Esta seguro de que desea eliminar este cliente? (S/N): ");
            scanf(" %c", &confirmacion);
            if (confirmacion == 'S' || confirmacion == 's') {
                cliente.Estatus = 0; // Actualizar el estado del cliente a 0
                fprintf(temporal, "%d,%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s,%d\n", cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno, cliente.RFC, cliente.Correo, cliente.Telefono, cliente.descuento, cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero, cliente.direccion.CP, cliente.Estatus);
                printf("\nCliente eliminado correctamente.\n");
            } else {
                fclose(archivo);
                fclose(temporal);
                remove("temporal.txt"); // Eliminar el archivo temporal
                printf("\nVolviendo al menu de clientes...\n");
                sleep(2);
                system("cls");
                return;
            }
        } else {
            // Si no es el cliente a eliminar, escribir la línea tal como está en el archivo temporal
            fprintf(temporal, "%d,%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s,%d\n", cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno, cliente.RFC, cliente.Correo, cliente.Telefono, cliente.descuento, cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero, cliente.direccion.CP, cliente.Estatus);
        }
    }

    fclose(archivo);
    fclose(temporal);

    remove("clientes.txt"); // Eliminar el archivo original
    rename("temporal.txt", "clientes.txt"); // Renombrar el archivo temporal al original

    if (!encontrado) {
        printf("El cliente con el ID '%d' no fue encontrado.\n", idCliente);
    }

    char volverMenu = obtenerOpcionSNClientes("\nDesea regresar al submenu clientes? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu clientes...\n");
        sleep(2);
        system("cls");
        submenuClientes();
    }
    else{
        printf("\nRegresando a Eliminar Cliente...\n");
        sleep(1);
        system("cls");
        eliminarCliente();
    }
}

void mostrarClientePorID() {
    system("cls");
    int idClienteABuscar;
    char fecha[11];
    obtenerFechaActual(fecha);
    FILE *archivo;
    Cliente cliente;

    idClienteABuscar = validarEntero("\nIngrese el ID del cliente que desea buscar: ");
    system("cls");

    archivo = fopen("clientes.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de clientes.\n");
        return;
    }

    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d", &cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno, cliente.RFC, cliente.Correo, cliente.Telefono, &cliente.descuento, cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero, cliente.direccion.CP, &cliente.Estatus) == 13) {
        if (cliente.Id == idClienteABuscar) {
            encontrado = 1;
            printf("\t\t  Comercializadora Fuentes\n");
            printf("\t\t   Consulta por Clientes\n\n");
            printf("\nFecha: %s\n", fecha);
            printf("---------------------------------------------------------------\n");
            printf("ID: %d\n", cliente.Id);
            printf("Nombre: %s %s %s\n", cliente.Nombre, cliente.Paterno, cliente.Materno);
            printf("RFC: %s\n", cliente.RFC);
            printf("Correo: %s\n", cliente.Correo);
            printf("Telefono: %s\n", cliente.Telefono);
            printf("Direccion: %s, #%s, %s, CP: %s\n", cliente.direccion.Calle, cliente.direccion.Numero, cliente.direccion.Colonia, cliente.direccion.CP);
            printf("Descuento: %d%%\n", cliente.descuento);
            printf("Estatus: %s\n", cliente.Estatus == 1 ? "Activo" : "Inactivo");
            printf("---------------------------------------------------------------\n");
            break;
        }
    }

    fclose(archivo);

    if (!encontrado) {
        printf("El cliente con el ID '%d' no fue encontrado.\n", idClienteABuscar);
    }

    char volverMenu = obtenerOpcionSNClientes("\nDesea regresar al submenu clientes? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu clientes...\n");
        sleep(2);
        system("cls");
        submenuClientes();
    }
    else{
        system("cls");
        printf("\nRegresando a Mostrar Cliente Por ID...\n");
        sleep(1);
        mostrarClientePorID();
    }
}

void mostrarClientesPorNombre() {
    system("cls");
    FILE *archivo;
    Cliente cliente;
    char nombreCliente[50], fecha[11];
    printf("Ingrese el nombre del cliente que desea buscar: ");
    scanf("%s", nombreCliente);
    system("cls");
    obtenerFechaActual(fecha);

    archivo = fopen("clientes.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de clientes.\n");
        return;
    }

    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^,],%[^,],%d", &cliente.Id, cliente.Nombre, cliente.Paterno, cliente.Materno, cliente.RFC, cliente.Correo, cliente.Telefono, &cliente.descuento, cliente.direccion.Calle, cliente.direccion.Colonia, cliente.direccion.Numero, cliente.direccion.CP, &cliente.Estatus) == 13) {
        if (strstr(cliente.Nombre, nombreCliente) != NULL || strstr(cliente.Paterno, nombreCliente) != NULL || strstr(cliente.Materno, nombreCliente) != NULL) {
            if (!encontrado) {
                printf("\t\t  Comercializadora Fuentes\n");
                printf("\t\t   Consulta por Clientes\n\n");
                printf("\nFecha: %s\n", fecha);
                printf("---------------------------------------------------------------\n");
                printf("Resultados para '%s':\n\n", nombreCliente);
                encontrado = 1;
            }
            printf("ID: %d\n", cliente.Id);
            printf("Nombre: %s %s %s\n", cliente.Nombre, cliente.Paterno, cliente.Materno);
            printf("RFC: %s\n", cliente.RFC);
            printf("Correo: %s\n", cliente.Correo);
            printf("Telefono: %s\n", cliente.Telefono);
            printf("Direccion: %s, #%s, %s, CP: %s\n", cliente.direccion.Calle, cliente.direccion.Numero, cliente.direccion.Colonia, cliente.direccion.CP);
            printf("Descuento: %d%%\n", cliente.descuento);
            printf("Estatus: %s\n", cliente.Estatus == 1 ? "Activo" : "Inactivo");
            printf("---------------------------------------------------------------\n");

        }
    }

    fclose(archivo);

    if (!encontrado) {
        printf("No se encontraron clientes con el nombre '%s'.\n", nombreCliente);
    }

    char volverMenu = obtenerOpcionSNClientes("\nDesea regresar al submenu clientes? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu clientes...\n");
        sleep(2);
        system("cls");
        submenuClientes();
    }
    else {
        system("cls");
        printf("\nRegresando a la busqueda de clientes por nombre...\n");
        sleep(1);
        mostrarClientesPorNombre();
    }
}

void submenuClientes() {
    int opcion;
    char subopcion[10];

    do {
        system("cls");
        printf("\n--- Submenu de Clientes ---\n\n");
        printf("1. Nuevo Cliente\n");
        printf("2. Eliminar Cliente\n");
        printf("3. Consulta de cliente\n");
        printf("4. Regresar al menu principal\n");

        opcion = validarEntero("\nSeleccione una opcion: ");
        switch (opcion) {
            case 1:
                nuevoCliente();
                break;
            case 2:
                eliminarCliente();
                break;
            case 3:
                system("cls");
                printf("\n --- Consulta de Clientes ---\n");
                printf("\nA. Por Nombre\n");
                printf("B. Por Clave\n");
                printf("C. Regresar al submenu de clientes\n");
                printf("\nSeleccione una opcion: ");
                scanf("%s", subopcion);

                if (strlen(subopcion) == 1 && (subopcion[0] == 'A' || subopcion[0] == 'a' || subopcion[0] == 'B' || subopcion[0] == 'b' || subopcion[0] == 'C' || subopcion[0] == 'c')) {
                    switch (toupper(subopcion[0])) {
                        case 'A':
                            system("cls");
                            mostrarClientesPorNombre();
                            break;
                        case 'B':
                            system("cls");
                            mostrarClientePorID();
                            break;
                        case 'C':
                            printf("\nRegresando al submenu de clientes...\n");
                            sleep(2);
                            system("cls");
                            break;
                        default:
                            printf("\nOpcion no valida. Ingresa una opcion correcta\n");
                            sleep(2);
                            system("cls");
                            break;
                    }
                } else {
                    printf("\nOpcion no valida. Ingrese una opcion correcta\n");
                    sleep(2);
                    system("cls");
                }
                break;
            case 4:
                printf("\nRegresando al menu principal...\n");
                sleep(1);
                system("cls");
                break;
            default:
                printf("\nOpcion no valida. Ingresa una opcion correcta\n");
                sleep(1);
                break;
        }
    } while (opcion != 4);
}
