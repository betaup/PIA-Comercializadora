#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "conexiones.h"

char obtenerOpcionSNUsuarios() {
    char opcion[10];
    while (1) {
        printf("\nDesea regresar al submenu de usuarios? (S/N): ");
        scanf("%s", opcion);
        if (strlen(opcion) == 1 && (opcion[0] == 'S' || opcion[0] == 's' || opcion[0] == 'N' || opcion[0] == 'n')) {
            return opcion[0];
        } else {
            printf("\nOpcion no valida. Por favor, ingrese S o N.\n");
        }
    }
}

bool es_longitud_valida(const char *cadena, int longitud_minima) {
    return strlen(cadena) >= longitud_minima;
}

int validarCP(const char *cp) {
    int longitud = strlen(cp);

    if (longitud != 5) {
        return 0;
    }

    for (int i = 0; i < longitud; i++) {
        if (!isdigit(cp[i])) {
            return 0;
        }
    }

    return 1;
}

int validarNumeroCasa(const char *numero_casa) {
    int longitud = strlen(numero_casa);

    if (longitud > 8) {
        return 0;
    }
    for (int i = 0; i < longitud; i++) {
        if (!isdigit(numero_casa[i])) {
            return 0;
        }
    }

    return 1;
}

int validarContrasena(const char *contrasena) {
    int longitud = strlen(contrasena);

    if (longitud < 5 || longitud > 20) {
        return 0;
    }

    int tiene_mayuscula = 0;
    int tiene_minuscula = 0;
    int tiene_numero = 0;

    for (int i = 0; i < longitud; i++) {
        if (isupper(contrasena[i])) {
            tiene_mayuscula = 1;
        } else if (islower(contrasena[i])) {
            tiene_minuscula = 1;
        } else if (isdigit(contrasena[i])) {
            tiene_numero = 1;
        }
    }

    if (!tiene_mayuscula || !tiene_minuscula || !tiene_numero) {
        return 0;
    }

    return 1;
}


int contieneSoloLetras(const char *cadena) {
    int letras = 0;
    int mayusculas = 0;

    for (int i = 0; cadena[i] != '\0'; i++) {
        if (isalpha(cadena[i])) {
            letras++;
            if (isupper(cadena[i])) {
                mayusculas++;
            }
        } else if (!isspace(cadena[i])) {
            return 0; // Si el caracter no es una letra ni un espacio, la cadena no contiene solo letras
        }
    }


    if (letras < 3 || mayusculas < 1) {
        return 0;
    }

    return 1;
}

int validarTelefono(const char *telefono) {
    int longitud = strlen(telefono);

    if (longitud != 10)
        return 0;

    for (int i = 0; i < longitud; i++) {
        if (!isdigit(telefono[i]))
            return 0; // No es un digito
    }

    return 1;
}

int validarCorreo(const char *correo) {
    int arroba = 0;
    int punto_despues_arroba = 0;
    int longitud = strlen(correo);

    if (longitud == 0)
        return 0;

    for (int i = 0; i < longitud; i++) {
        if (correo[i] == '@') {
            if (arroba)
                return 0;
            else
                arroba = 1;
        }
        else if (correo[i] == '.') {
            if (arroba)
                punto_despues_arroba = 1;
        }
    }

    return arroba && punto_despues_arroba;
}

void nuevoUsuario() {
    system("cls");
    char entrada[10], c;
    struct Usuario usuario;
    FILE *archivo;

    do {
        printf("Ingrese el ID del usuario: ");
        if (scanf("%d", &usuario.Id) != 1 || getchar() != '\n') {
            printf("El ID debe ser un numero entero. Intentalo de nuevo.\n");
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar el bufer de entrada
        } else {
            break; // Salir del bucle si se ingreso un numero valido
        }
    } while (true);

    archivo = fopen("usuarios.txt", "r");
    if (archivo != NULL) {
        struct Usuario temp;
        while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d", &temp.Id, temp.Nombre, temp.Paterno, temp.Materno, temp.Correo, temp.Telefono, temp.Usuario, temp.Contrasena, temp.direccion.Calle, temp.direccion.Colonia, temp.direccion.Numero, temp.direccion.CP, &temp.Estatus, &temp.Tipo) == 14) {
            if (temp.Id == usuario.Id) {
                fclose(archivo);
                printf("\nEl usuario ya existe.\n");
                sleep(1);
                system("cls");
                return;
            }
        }
        fclose(archivo);
    }

    fflush(stdin);

    do {
            printf("Ingrese el nombre del usuario: ");
            scanf(" %[^\n]s", usuario.Nombre);

            if (!contieneSoloLetras(usuario.Nombre)) {
            printf("El nombre debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
        }
    } while (!contieneSoloLetras(usuario.Nombre));


    do {
            printf("Ingrese el apellido paterno del usuario: ");
            scanf(" %[^\n]s", usuario.Paterno);

            if (!contieneSoloLetras(usuario.Paterno)) {
            printf("El apellido debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(usuario.Paterno));


    do {
            printf("Ingrese el apellido materno del usuario: ");
            scanf(" %[^\n]s", usuario.Materno);

            if (!contieneSoloLetras(usuario.Materno)) {
            printf("El nombre debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(usuario.Materno));


   do {
    printf("Ingrese el correo electronico del usuario: ");
    scanf(" %[^\n]", usuario.Correo);

    if (!validarCorreo(usuario.Correo)) {
        printf("Correo electronico invalido. Intentalo de nuevo.\n");
    }
    } while (!validarCorreo(usuario.Correo));


    do {
        printf("Ingrese su numero de telefono (10 digitos): ");
        scanf("%s", usuario.Telefono);

        if (!validarTelefono(usuario.Telefono)) {
            printf("El telefono costa de 10 digitos. Intentalo de nuevo.\n");
        }
    } while (!validarTelefono(usuario.Telefono));

   do {
        printf("Ingrese el nombre de usuario ( 8+ caracteres): ");
        scanf(" %[^\n]s", usuario.Usuario);
        if (!es_longitud_valida(usuario.Usuario, 8)) {
            printf("El nombre de usuario debe tener al menos 8 caracteres. Intentalo de nuevo.\n");
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar el b�fer de entrada
        }
    } while (!es_longitud_valida(usuario.Usuario, 8));

    do{
            printf("Ingrese la contrasena del usuario: ");
            scanf(" %[^\n]s", usuario.Contrasena);

            if (!validarContrasena(usuario.Contrasena)) {
            printf("La contrasena debe incluir al menos una Mayus, Minuscula, numero. Intentalo de nuevo.\n");
        }
    } while (!validarContrasena(usuario.Contrasena));

    do {
            printf("Ingrese la calle de la direccion del usuario: ");
            scanf(" %[^\n]s", usuario.direccion.Calle);

            if (!contieneSoloLetras(usuario.direccion.Calle)) {
            printf("La calle debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(usuario.direccion.Calle));

    do {
            printf("Ingrese la colonia de la direccion del usuario: ");
            scanf(" %[^\n]s", usuario.direccion.Colonia);

            if (!contieneSoloLetras(usuario.direccion.Colonia)) {
            printf("La calle debe tener por lo menos una mayuscula y 3 letras. Intentalo de nuevo\n");
    }
    } while (!contieneSoloLetras(usuario.direccion.Colonia));

    do{
            printf("Ingrese el numero de la direccion del usuario: ");
            scanf(" %[^\n]s", usuario.direccion.Numero);

            if (!validarNumeroCasa(usuario.direccion.Numero)) {
            printf("Numero invalido.Intentalo de nuevo.\n");
        }
    } while (!validarNumeroCasa(usuario.direccion.Numero));

    do{
           printf("Ingrese el codigo postal de la direccion del usuario: ");
           scanf(" %[^\n]s", usuario.direccion.CP);

            if (!validarCP(usuario.direccion.CP)) {
            printf("CP invalido.Intentalo de nuevo.\n");
        }
    } while (!validarCP(usuario.direccion.CP));

    usuario.Estatus = 1; // Por defecto, el usuario esta activo

    do {
        printf("Tipo de usuario (0 para usuario normal, 1 para administrador): ");
        scanf("%d", &usuario.Tipo);

        if (usuario.Tipo != 0 && usuario.Tipo != 1) {
            printf("Tipo de usuario invalido. Por favor ingrese 0 para usuario normal o 1 para administrador.\n");
        }
    } while (usuario.Tipo != 0 && usuario.Tipo != 1);


    archivo = fopen("usuarios.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de usuarios.\n");
        return;
    }

    fprintf(archivo, "%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d\n", usuario.Id, usuario.Nombre, usuario.Paterno, usuario.Materno, usuario.Correo, usuario.Telefono, usuario.Usuario, usuario.Contrasena, usuario.direccion.Calle, usuario.direccion.Colonia, usuario.direccion.Numero, usuario.direccion.CP, usuario.Estatus, usuario.Tipo);
    fclose(archivo);

    printf("\nUsuario agregado con exito.\n");

    char volverMenu = obtenerOpcionSNUsuarios("\nDesea regresar al submenuusuarios? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu usuarios...\n");
        sleep(2);
        system("cls");
        submenuUsuarios();
    }
    else{
        printf("\nRegresando a Nuevo Usuario...\n");
        sleep(1);
        system("cls");
        nuevoUsuario();
    }

    sleep(3);
    system("cls");
}

void eliminarUsuario() {
    system("cls");
    char nombreUsuario[20];
    char confirmacion;
    FILE *archivo;
    struct Usuario usuario;

    printf("Ingrese el nombre de usuario a eliminar: ");
    scanf("%s", nombreUsuario);

    system("cls");

    archivo = fopen("usuarios.txt", "r+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de usuarios.\n");
        return;
    }

    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d", &usuario.Id, usuario.Nombre, usuario.Paterno, usuario.Materno, usuario.Correo, usuario.Telefono, usuario.Usuario, usuario.Contrasena, usuario.direccion.Calle, usuario.direccion.Colonia, usuario.direccion.Numero, usuario.direccion.CP, &usuario.Estatus, &usuario.Tipo) == 14) {
        if (strcmp(usuario.Usuario, nombreUsuario) == 0) {
            encontrado = 1;
            if (usuario.Estatus == 0) {
                printf("El usuario '%s' ya esta eliminado.\n", nombreUsuario);
                fclose(archivo);
                printf("\nVolviendo al submenu de usuarios...\n");
                sleep(2);
                system("cls");
                return;
            }
            printf("\nUsuario encontrado:\n");
            printf("ID: %d\n", usuario.Id);
            printf("Nombre: %s %s %s\n", usuario.Nombre, usuario.Paterno, usuario.Materno);
            printf("Correo: %s\n", usuario.Correo);
            printf("Telefono: %s\n", usuario.Telefono);
            printf("Usuario: %s\n", usuario.Usuario);

            printf("Esta seguro de que desea eliminar este usuario? (S/s): ");
            scanf(" %c", &confirmacion);
            if (confirmacion == 'S' || confirmacion == 's') {
                fseek(archivo, -3, SEEK_CUR); // Mover el puntero tres caracteres hacia atras para sobrescribir el estatus
                fprintf(archivo, "0");
                printf("\nUsuario eliminado correctamente.\n");
            } else {
                fclose(archivo);
                printf("\nOperacion cancelada...\n");
                sleep(2);
                system("cls");
            }
            break;
        }
    }

    fclose(archivo);

    if (!encontrado) {
        printf("El usuario con el nombre '%s' no fue encontrado.\n", nombreUsuario);
    }

    char volverMenu = obtenerOpcionSNUsuarios("\nDesea regresar al submenu usuarios? [S/N]: ");
    if (volverMenu == 'S' || volverMenu == 's') {
        printf("\nVolviendo al submenu usuarios...\n");
        sleep(2);
        system("cls");
        submenuUsuarios();
    }
    else{
        printf("\nRegresando a Eliminar Usuario...\n");
        sleep(1);
        system("cls");
        nuevoUsuario();
    }

    sleep(2);
    system("cls");
}

void reactivarUsuario() {
    system("cls");
    char nombreUsuario[20];
    FILE *archivo;
    struct Usuario usuario;

    printf("Ingrese el nombre de usuario a reactivar: ");
    scanf("%s", nombreUsuario);

    system("cls");

    archivo = fopen("usuarios.txt", "r+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de usuarios.\n");
        return;
    }

    int encontrado = 0;
    while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d", &usuario.Id, usuario.Nombre, usuario.Paterno, usuario.Materno, usuario.Correo, usuario.Telefono, usuario.Usuario, usuario.Contrasena, usuario.direccion.Calle, usuario.direccion.Colonia, usuario.direccion.Numero, usuario.direccion.CP, &usuario.Estatus, &usuario.Tipo) == 14) {
        if (strcmp(usuario.Usuario, nombreUsuario) == 0) {
            encontrado = 1;
            fseek(archivo, -3, SEEK_CUR); // Mover el puntero tres caracteres hacia atras para sobrescribir el estatus
            fprintf(archivo, "1");
            printf("\nUsuario reactivado correctamente.\n");
            break;
        }
    }

    fclose(archivo);

    if (!encontrado) {
        printf("El usuario con el nombre '%s' no fue encontrado.\n", nombreUsuario);
    }

    char opcion = obtenerOpcionSNUsuarios();
    if (opcion == 'S' || opcion == 's') {
        printf("\nRegresando al submenu de usuarios...\n");
        sleep(1);
        system("cls");
        submenuUsuarios();
    } else {
        printf("\nRegresando a Reactivar Usuario...\n");
        sleep(1);
        system("cls");
        reactivarUsuario;
    }
}

void mostrarUsuario(struct Usuario usuario) {
    printf("%-8d %-20s %-20s %-20s %-30s %-20s %-20s %-10s\n", usuario.Id, usuario.Nombre, usuario.Paterno, usuario.Materno, usuario.Correo, usuario.Telefono, usuario.Usuario, usuario.Estatus == 1 ? "Activo" : "Inactivo");
}

void submenuUsuarios() {
    int opcion;
    do {
        system("cls");
        printf("\n--- Submenu de Usuarios ---\n\n");
        printf("1. Nuevo Usuario\n");
        printf("2. Eliminar Usuario\n");
        printf("3. Reactivar Usuario\n");
        printf("4. Regresar al menu principal\n");

        opcion = validarEntero("\nSeleccione una opcion: ");
        switch (opcion) {
            case 1:
                nuevoUsuario();
                break;
            case 2:
                eliminarUsuario();
                break;
            case 3:
                reactivarUsuario();
                break;
            case 4:
                system("cls");
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("\nOpcion no valida. Ingresa una opcion correcta\n");
                sleep(1);
                system("cls");
                break;
        }
    } while (opcion != 4);
}


void *validarCredencialesEnHilo(void *arg) {
    struct Credenciales *credenciales = (struct Credenciales *)arg;

    int *tipoUsuario = (int *)malloc(sizeof(int));
     if (tipoUsuario == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1); // O manejo de error adecuado
    }
    *tipoUsuario = validarCredenciales(credenciales->usuario, credenciales->contrasena);

    // Retornar el tipo de usuario a través de un puntero (int *) para poder pasarlo de regreso al hilo principal
    pthread_exit(tipoUsuario);
}

int validarCredenciales(char *usuario, char *contrasena) {
    if (strcmp(usuario, "admin") == 0 && strcmp(contrasena, "123") == 0) {
        return 1; // Administrador
    } else {
        FILE *archivo = fopen("usuarios.txt", "r+");
        if (archivo == NULL) {
            printf("Error al abrir el archivo.\n");
            exit(1);
        }

        struct Usuario usuarioTemp;
        int usuarioEncontrado = 0;
        int tipoUsuario = -1; // Inicializamos tipoUsuario con un valor por defecto

        while (fscanf(archivo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d",
                &usuarioTemp.Id, usuarioTemp.Nombre, usuarioTemp.Paterno, usuarioTemp.Materno, usuarioTemp.Correo,
                usuarioTemp.Telefono, usuarioTemp.Usuario, usuarioTemp.Contrasena, usuarioTemp.direccion.Calle,
                usuarioTemp.direccion.Colonia, usuarioTemp.direccion.Numero, usuarioTemp.direccion.CP,
                &usuarioTemp.Estatus, &usuarioTemp.Tipo) == 14) {
            if (strcmp(usuarioTemp.Usuario, usuario) == 0) {
                usuarioEncontrado = 1; // Marcamos que se encontró el usuario
                if (usuarioTemp.Estatus == 0) {
                    fclose(archivo);
                    tipoUsuario = -1; // Usuario desactivado
                    break; // Salimos del bucle
                } else if (strcmp(usuarioTemp.Contrasena, contrasena) == 0) {
                    fclose(archivo);
                    tipoUsuario = usuarioTemp.Tipo; // Asignamos el tipo de usuario encontrado
                    break; // Salimos del bucle
                }
            }
        }

        if (usuarioEncontrado && tipoUsuario == -1) {
            // Si se encontró el usuario pero la contraseña es incorrecta, incrementamos los intentos fallidos
            FILE *archivoActualizado = fopen("usuarios.txt", "r+");
            if (archivoActualizado == NULL) {
                printf("Error al abrir el archivo para actualizar.\n");
                exit(1);
            }
            rewind(archivoActualizado);
            while (fscanf(archivoActualizado, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d",
                    &usuarioTemp.Id, usuarioTemp.Nombre, usuarioTemp.Paterno, usuarioTemp.Materno, usuarioTemp.Correo,
                    usuarioTemp.Telefono, usuarioTemp.Usuario, usuarioTemp.Contrasena, usuarioTemp.direccion.Calle,
                    usuarioTemp.direccion.Colonia, usuarioTemp.direccion.Numero, usuarioTemp.direccion.CP,
                    &usuarioTemp.Estatus, &usuarioTemp.Tipo) == 14) {
                if (strcmp(usuarioTemp.Usuario, usuario) == 0) {
                    usuarioTemp.Estatus++; // Incrementamos los intentos fallidos
                    fseek(archivoActualizado, -3, SEEK_CUR); // Retrocedemos al inicio del campo Estatus
                    fprintf(archivoActualizado, "0"); // Actualizamos el archivo
                    break; // Actualizamos el archivo
                }
            }
            fclose(archivoActualizado);
        }

        fclose(archivo);
        return tipoUsuario; // Devolvemos el tipo de usuario encontrado
    }
}

void mostrarMenuAdmin() {
    int opcion;
    do {
        system("cls");
        printf("\nBienvenido Administrador.\n");
        printf("\n1. Clientes\n");
        printf("2. Ventas\n");
        printf("3. Productos\n");
        printf("4. Inventario\n");
        printf("5. Usuarios\n");
        printf("6. Salir\n");

        opcion = validarEntero("\nSeleccione una opcion: ");
        switch (opcion) {
            case 1:
                system("cls");
                submenuClientes();
                break;
            case 2:
                system("cls");
                submenuVentas();
                break;
            case 3:
                system("cls");
                submenuProductos();
                break;
            case 4:
                system("cls");
                submenuInventario();
                break;
            case 5:
                system("cls");
                submenuUsuarios();
                break;
            case 6:
                printf("\nSaliendo del sistema...\n");
                sleep(2);
                system("cls");
                return;
            default:
                printf("\nOpcion no valida. Ingresa una opcion correcta.\n");
                sleep(1);
                system("cls");
                break;
        }
    } while (opcion != 6);
}

void mostrarMenuUsuario() {
    int opcion;
    do {
        system("cls");
        printf("\nBienvenido Usuario.\n");
        printf("\n1. Clientes\n");
        printf("2. Ventas\n");
        printf("3. Salir\n");

        opcion = validarEntero("\nSeleccione una opcion: ");
        switch (opcion) {
            case 1:
                system("cls");
                submenuClientes();
                break;
            case 2:
                system("cls");
                submenuVentas();
                break;
            case 3:
                printf("\nSaliendo del sistema...\n");
                sleep(2);
                system("cls");
                return;
            default:
                printf("\nOpcion no valida. Ingresa una opcion correcta.\n");
                sleep(1);
                system("cls");
                break;
        }
    } while (opcion != 3);
}

void iniciarSesion() {
    char usuario[20];
    char contrasena[20];
    int intentos = 3;

    while (intentos > 0) {
        printf("Usuario: admin Contrasena: 123\n");
        printf("\nInicio de Sesion\n");
        printf("Usuario: ");
        scanf("%s", usuario);
        printf("Contrasena: ");
        scanf("%s", contrasena);

        // Crear un hilo para validar las credenciales
        pthread_t hiloValidacion;
        int *tipoUsuario;

        // Pasar los datos de usuario y contraseña al hilo de validación
        struct Credenciales credenciales;
        strcpy(credenciales.usuario, usuario);
        strcpy(credenciales.contrasena, contrasena);

        // Crear el hilo y esperar a que termine
        pthread_create(&hiloValidacion, NULL, validarCredencialesEnHilo, (void *)&credenciales);
        pthread_join(hiloValidacion, (void **)&tipoUsuario);

        if (*tipoUsuario == 1) {
            mostrarMenuAdmin(); // Si es administrador
            return;
        } else if (*tipoUsuario == 0) {
            mostrarMenuUsuario(); // Si es usuario normal
            return;
        } else {
            printf("\nNombre de usuario o contrasena invalido, intente de nuevo!!!\n");
            intentos--;
            printf("\n-----------------------\n");
            printf("Intentos restantes: %d |\n", intentos);
            printf("-----------------------");
            sleep(2);
            system("cls");
        }
    }

    printf("\nSe agotaron los intentos de inicio de sesion. Comuniquese con el administrador\n");
}
