#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar un producto en el inventario
typedef struct Producto {
    int codigo;
    char nombre[50];
    int cantidad;
    int precio;
    struct Producto *siguiente;
} Producto;

// Estructura para la cola FIFO del inventario
typedef struct {
    Producto *frente;
    Producto *final;
} Inventario;

// Función para inicializar el inventario
void inicializarInventario(Inventario *inv) {
    inv->frente = NULL;
    inv->final = NULL;
}

// Función para registrar un producto
void registrarProducto(Inventario *inv, int codigo, char *nombre, int cantidad, int precio) {
    Producto *nuevo = (Producto *)malloc(sizeof(Producto));
    if (!nuevo) {
        printf("Error de memoria.\n");
        return;
    }
    nuevo->codigo = codigo;
    strcpy(nuevo->nombre, nombre);
    nuevo->cantidad = cantidad;
    nuevo->precio = precio;
    nuevo->siguiente = NULL;
    
    if (inv->final == NULL) {
        inv->frente = nuevo;
        inv->final = nuevo;
    } else {
        inv->final->siguiente = nuevo;
        inv->final = nuevo;
    }
}

// Función para mostrar todos los productos
void mostrarProductos(Inventario *inv) {
    Producto *actual = inv->frente;
    if (!actual) {
        printf("El inventario está vacío.\n");
        return;
    }
    printf("\nInventario:\n");
    while (actual) {
        printf("Código: %d | Nombre: %s | Cantidad: %d | Precio: %d | Costo Total: %d\n",
               actual->codigo, actual->nombre, actual->cantidad, actual->precio, actual->cantidad * actual->precio);
        actual = actual->siguiente;
    }
}

// Función para actualizar la cantidad tras una venta
void venderProducto(Inventario *inv, int codigo, int cantidadVendida) {
    Producto *actual = inv->frente;
    while (actual) {
        if (actual->codigo == codigo) {
            if (actual->cantidad >= cantidadVendida) {
                actual->cantidad -= cantidadVendida;
                printf("Venta registrada correctamente.\n");
            } else {
                printf("No hay suficiente stock para esta venta.\n");
            }
            return;
        }
        actual = actual->siguiente;
    }
    printf("Producto no encontrado.\n");
}

// Función para calcular el costo total del inventario
void calcularCostoTotal(Inventario *inv) {
    Producto *actual = inv->frente;
    int total = 0;
    while (actual) {
        total += actual->cantidad * actual->precio;
        actual = actual->siguiente;
    }
    printf("Costo total del inventario: %d\n", total);
}

// Función para eliminar productos con cantidad cero
void eliminarProducto(Inventario *inv) {
    while (inv->frente && inv->frente->cantidad == 0) {
        Producto *temp = inv->frente;
        inv->frente = inv->frente->siguiente;
        free(temp);
    }
    if (!inv->frente) {
        inv->final = NULL;
    } else {
        Producto *actual = inv->frente;
        while (actual->siguiente) {
            if (actual->siguiente->cantidad == 0) {
                Producto *temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                if (temp == inv->final) {
                    inv->final = actual;
                }
                free(temp);
            } else {
                actual = actual->siguiente;
            }
        }
    }
    printf("Productos con cantidad 0 eliminados.\n");
}

// Función principal para probar las funcionalidades
int main() {
    Inventario miInventario;
    inicializarInventario(&miInventario);
    
    registrarProducto(&miInventario, 1, "Producto A", 10, 50);
    registrarProducto(&miInventario, 2, "Producto B", 5, 30);
    registrarProducto(&miInventario, 3, "Producto C", 8, 20);
    
    mostrarProductos(&miInventario);
    
    venderProducto(&miInventario, 1, 3);
    venderProducto(&miInventario, 2, 5);
    
    mostrarProductos(&miInventario);
    
    calcularCostoTotal(&miInventario);
    
    eliminarProducto(&miInventario);
    
    mostrarProductos(&miInventario);
    
    return 0;
}
