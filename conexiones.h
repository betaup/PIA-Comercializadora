#ifndef CONEXIONES_H_INCLUDED
#define CONEXIONES_H_INCLUDED

// Clientes
typedef struct Cliente {
    int Id;
    char Nombre[50];
    char Paterno[50];
    char Materno[50];
    struct {
        int year;
        int mes;
        int dia;
    } fecha;
    char RFC[14];
    char Correo[50];
    char Telefono[13];
    int descuento;
    struct {
        char Calle[50];
        char Colonia[50];
        char Numero[50];
        char CP[6];
    } direccion;
    int Estatus;
} Cliente;
Cliente clienteActual;


//Prototipos para Clientes
void generarRFC(struct Cliente *cliente);
void nuevoCliente ();
void eliminarCliente();
void mostrarClientesPorNombre();
void mostrarClientePorID();
void submenuClientes();


//Inventario
typedef struct {
    int folio;
    int clave_producto;
    int cantidad_reabastecida;
    char fecha[11]; // Suponiendo formato "dd/mm/yyyy"
} Reabastecimiento;

void mostrarSubmenuInventario();
void reabastecerProducto();
void mostrarMenuReportes();
void reporteCompleto();
void reportePorTipoProducto();
void reporteNivelDeStock();
void reporteReabastecimiento();
char obtenerOpcionSN();

//Productos
typedef struct Producto{
    int clave;
    char descripcion[100];
    int tipo;
    int cantidad_existencia;
    int nivel_stock;
    float precio;
    int estatus; // 1 para activo, 0 inactivo
} Producto;

// Prototipos de funciones relacionadas con los productos
void nuevoProducto();
void editarProducto();
void consultaPorTipoProducto();
void consultaPorClave();
void submenuProductos();

//Usuarios
struct Usuario {
    int Id;
    char Nombre[100];
    char Paterno[100];
    char Materno[100];
    char Correo[100];
    char Telefono[100];
    char Usuario[100];
    char Contrasena[100];
    struct {
        char Calle[100];
        char Colonia[100];
        char Numero[100];
        char CP[100];
    } direccion;
    int Estatus;
    int Tipo;
};

struct Credenciales {
    char usuario[20];
    char contrasena[20];
};

int contieneSoloLetras(const char *cadena);
void nuevoUsuario();
void eliminarUsuario();
void reactivarUsuario();
void mostrarUsuario(struct Usuario usuario);
void submenuUsuarios();
void iniciarSesion();
void mostrarMenuAdmin();
void mostrarMenuUsuario();
void *validarCredencialesEnHilo(void *arg);

//Ventas
typedef struct Venta {
    int folio;
    int idCliente;
    int claveProducto;
    int cantidadComprada;
    float precioVenta;
    char fecha[11];
    char descripcion[100];
} Venta;

void submenuVentas();
void realizarVenta();
void reporteVenta();
void reporteVentaDelDia();
void reporteVentaPorCliente();
void regresarMenuPrincipal();


#endif // CONEXIONES_H_INCLUDED
