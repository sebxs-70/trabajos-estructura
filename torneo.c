#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Jugador {
    int id;
    char nombre[30];
    int edad;
    char genero; // 'M' o 'F'
    char deporte[50];
    struct Jugador* siguiente;
} Jugador;

// Cola (FIFO)
Jugador* frente = NULL;
Jugador* final = NULL;

// Pila (LIFO)
Jugador* topeHistorial = NULL;

// Función para contar jugadores en espera y por género
void contarJugadores(int* total, int* hombres, int* mujeres) {
    *total = 0;
    *hombres = 0;
    *mujeres = 0;
    Jugador* actual = frente;
    while (actual) {
        (*total)++;
        if (actual->genero == 'M') (*hombres)++;
        else if (actual->genero == 'F') (*mujeres)++;
        actual = actual->siguiente;
    }
}

// Agregar jugador a la cola con validaciones
void agregarJugador() {
    Jugador* nuevo = (Jugador*)malloc(sizeof(Jugador));
    printf("Ingrese ID: ");
    scanf("%d", &nuevo->id);
    printf("Nombre: ");
    getchar(); // Limpiar buffer
    fgets(nuevo->nombre, 30, stdin);
    nuevo->nombre[strcspn(nuevo->nombre, "\n")] = 0; // Eliminar salto
    printf("Edad: ");
    scanf("%d", &nuevo->edad);
    printf("Genero (M/F): ");
    scanf(" %c", &nuevo->genero);
    printf("Deporte: ");
    getchar();
    fgets(nuevo->deporte, 50, stdin);
    nuevo->deporte[strcspn(nuevo->deporte, "\n")] = 0;
    nuevo->siguiente = NULL;

    // Validación por género
    int total, hombres, mujeres;
    contarJugadores(&total, &hombres, &mujeres);
    if (nuevo->genero == 'M' && hombres - mujeres >= 4) {
        printf("No se puede agregar más hombres hasta que se equilibre la cola.\n");
        free(nuevo);
        return;
    } else if (nuevo->genero == 'F' && mujeres - hombres >= 4) {
        printf("No se puede agregar más mujeres hasta que se equilibre la cola.\n");
        free(nuevo);
        return;
    }

    // Agregar a la cola
    if (final == NULL) {
        frente = final = nuevo;
    } else {
        final->siguiente = nuevo;
        final = nuevo;
    }
    printf("Jugador agregado correctamente.\n");
}

// Mostrar jugadores en espera
void mostrarCola() {
    if (frente == NULL) {
        printf("No hay jugadores en espera.\n");
        return;
    }
    Jugador* actual = frente;
    printf("Jugadores en espera:\n");
    while (actual) {
        printf("ID: %d | Nombre: %s | Edad: %d | Genero: %c | Deporte: %s\n",
               actual->id, actual->nombre, actual->edad, actual->genero, actual->deporte);
        actual = actual->siguiente;
    }
}

// Contar jugadores en espera
void contarEnEspera() {
    int total, hombres, mujeres;
    contarJugadores(&total, &hombres, &mujeres);
    printf("Total en espera: %d | Hombres: %d | Mujeres: %d\n", total, hombres, mujeres);
}

// Permitir participación (sacar de la cola y pasar a la pila)
void permitirParticipacion() {
    if (frente == NULL) {
        printf("No hay jugadores en espera.\n");
        return;
    }

    if (frente->edad < 15) {
        printf("Jugador no cumple con la edad mínima para participar.\n");
        return;
    }

    // Sacar de la cola
    Jugador* participante = frente;
    frente = frente->siguiente;
    if (frente == NULL) final = NULL;

    // Agregar a la pila (historial)
    participante->siguiente = topeHistorial;
    topeHistorial = participante;

    printf("Jugador %s participó en el torneo.\n", participante->nombre);
}

// Mostrar historial (pila)
void mostrarHistorial() {
    if (topeHistorial == NULL) {
        printf("Historial vacío.\n");
        return;
    }
    Jugador* actual = topeHistorial;
    printf("Historial de participación:\n");
    while (actual) {
        printf("ID: %d | Nombre: %s | Edad: %d | Genero: %c | Deporte: %s\n",
               actual->id, actual->nombre, actual->edad, actual->genero, actual->deporte);
        actual = actual->siguiente;
    }
}

// Deshacer última participación (de pila a cola)
void deshacerParticipacion() {
    if (topeHistorial == NULL) {
        printf("No hay participación para deshacer.\n");
        return;
    }

    // Sacar de la pila
    Jugador* jugador = topeHistorial;
    topeHistorial = topeHistorial->siguiente;
    jugador->siguiente = NULL;

    // Agregar al final de la cola
    if (final == NULL) {
        frente = final = jugador;
    } else {
        final->siguiente = jugador;
        final = jugador;
    }

    printf("Última participación deshecha. %s vuelve a la cola.\n", jugador->nombre);
}

int main() {
    int opcion;
    do {
        printf("\n--- Menu Torneo Deportivo ---\n");
        printf("1. Agregar jugador a la cola\n");
        printf("2. Mostrar jugadores en espera\n");
        printf("3. Contar jugadores en espera\n");
        printf("4. Permitir participación del siguiente jugador\n");
        printf("5. Mostrar historial de participación\n");
        printf("6. Deshacer última participación\n");
        printf("7. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: agregarJugador(); break;
            case 2: mostrarCola(); break;
            case 3: contarEnEspera(); break;
            case 4: permitirParticipacion(); break;
            case 5: mostrarHistorial(); break;
            case 6: deshacerParticipacion(); break;
            case 7: printf("Saliendo...\n"); break;
            default: printf("Opción no válida.\n");
        }

    } while (opcion != 7);

    return 0;
}
