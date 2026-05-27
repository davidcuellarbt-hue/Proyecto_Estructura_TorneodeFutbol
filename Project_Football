#include <iostream>
#include <cstring> 
#include <cstdlib> 

using namespace std;


// ESTRUCTURAS BASE 

struct Jugador {
    int id_jugador;
    char nombre[50]; 
    int goles_anotados;
    Jugador* siguiente;
};

struct Equipo {
    int id_equipo;
    char nombre_equipo[50]; 
    int puntos;
    Jugador* plantilla; 
    Equipo* izquierdo;
    Equipo* derecho;
};


// FUNCIONES DEL ÁRBOL (EQUIPOS)

void insertarEquipo(Equipo* &arbol, int id, const char* nombre, int puntos) {
    if (arbol == NULL) {
        arbol = (Equipo*)malloc(sizeof(Equipo));
        arbol->id_equipo = id;
        strcpy(arbol->nombre_equipo, nombre);
        arbol->puntos = puntos;
        arbol->plantilla = NULL;
        arbol->izquierdo = NULL;
        arbol->derecho = NULL;
    } else if (puntos < arbol->puntos) {
        insertarEquipo(arbol->izquierdo, id, nombre, puntos);
    } else {
        insertarEquipo(arbol->derecho, id, nombre, puntos);
    }
}

Equipo* buscarEquipoPorID(Equipo* arbol, int id_buscar) {
    if (arbol == NULL) {
        return NULL;
    }
    if (arbol->id_equipo == id_buscar) {
        return arbol; 
    }
    
    Equipo* encontradoIzq = buscarEquipoPorID(arbol->izquierdo, id_buscar);
    if (encontradoIzq != NULL) {
        return encontradoIzq;
    }
    
    return buscarEquipoPorID(arbol->derecho, id_buscar);
}

void mostrarEquipos(Equipo* arbol) {
    if (arbol != NULL) {
        mostrarEquipos(arbol->derecho); // Imprime de mayor a menor puntuación
        cout << "[" << arbol->id_equipo << "] " << arbol->nombre_equipo << " - Puntos: " << arbol->puntos << endl;
        mostrarEquipos(arbol->izquierdo);
    }
}


// FUNCIONES DE LA LISTA Y RECURSIVIDAD

void registrarJugadorEnEquipo(Equipo* raizTorneo, int id_equipo, int id_jugador, const char* nombre, int goles) {
    Equipo* equipoDestino = buscarEquipoPorID(raizTorneo, id_equipo);
    
    if (equipoDestino == NULL) {
        cout << ">> Error: No se encontro el equipo con ID " << id_equipo << ". No se pudo registrar al jugador." << endl;
        return; 
    }

    Jugador* nuevoJugador = (Jugador*)malloc(sizeof(Jugador));
    nuevoJugador->id_jugador = id_jugador;
    strcpy(nuevoJugador->nombre, nombre); 
    nuevoJugador->goles_anotados = goles;
    nuevoJugador->siguiente = NULL;

    if (equipoDestino->plantilla == NULL) {
        equipoDestino->plantilla = nuevoJugador;
    } else {
        Jugador* aux = equipoDestino->plantilla;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevoJugador;
    }
    cout << ">> Exito: Jugador '" << nombre << "' agregado a " << equipoDestino->nombre_equipo << "." << endl;
}

int calcularPoderOfensivo(Jugador* actual) {
    if (actual == NULL) {
        return 0; 
    }
    return actual->goles_anotados + calcularPoderOfensivo(actual->siguiente);
}


// 4. INYECCIÓN AUTOMÁTICA DE DATOS BASE COMO PRUEBA DEL SISTEMA

void inyectarDatosPorDefecto(Equipo* &liga) {
    // Equipos iniciales
    insertarEquipo(liga, 101, "Real_Madrid", 15);
    insertarEquipo(liga, 102, "Barcelona", 12);
    insertarEquipo(liga, 103, "Milan", 18);

    // Jugadores iniciales
    registrarJugadorEnEquipo(liga, 101, 1, "Vinicius_Jr", 8);
    registrarJugadorEnEquipo(liga, 101, 2, "Bellingham", 14);
    registrarJugadorEnEquipo(liga, 102, 3, "Lamine_Yamal", 5);
    registrarJugadorEnEquipo(liga, 102, 4, "Lewandowski", 10);
    registrarJugadorEnEquipo(liga, 103, 5, "Rafael_Leao", 7);
}


// MAIN Y MENÚ INTERACTIVO COMPLETO

int main() {
    Equipo* liga = NULL;

    cout << "=== SISTEMA AUTOMATICO ===" << endl;
    cout << "Cargando datos base en memoria para la demostracion..." << endl;
    inyectarDatosPorDefecto(liga);
    cout << "Datos cargados correctamente.\n" << endl;

    int opcion;
    do {
        cout << "         MENU DE GESTION DE TORNEO       " << endl;
        cout << "1. Registrar un Equipo Nuevo" << endl;
        cout << "2. Registrar un Jugador en un Equipo" << endl;
        cout << "3. Ver Tabla de Posiciones General (Arbol BST)" << endl;
        cout << "4. Calcular Poder Ofensivo de un Equipo (Recursividad)" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int id, pts;
                char nombre[50];
                cout << "\n--- REGISTRAR EQUIPO NUEVO ---" << endl;
                cout << "Ingrese ID numerico del equipo: "; cin >> id;
                cout << "Ingrese Nombre (use guion_bajo para espacios): "; cin >> nombre;
                cout << "Ingrese Puntos actuales en el torneo: "; cin >> pts;
                
                insertarEquipo(liga, id, nombre, pts);
                cout << ">> Exito: Equipo registrado en el sistema." << endl;
                break;
            }

            case 2: {
                int id_eq, id_jug, goles;
                char nombre_jug[50];
                cout << "\n--- REGISTRAR JUGADOR NUEVO ---" << endl;
                cout << "ID del equipo al que pertenece: "; cin >> id_eq;
                cout << "ID numerico del jugador: "; cin >> id_jug;
                cout << "Nombre del jugador (use guion_bajo para espacios): "; cin >> nombre_jug;
                cout << "Goles anotados: "; cin >> goles;
                
                registrarJugadorEnEquipo(liga, id_eq, id_jug, nombre_jug, goles);
                break;
            }

            case 3:
                cout << "\n--- TABLA GENERAL DE POSICIONES (ORDENADA POR PTS) ---" << endl;
                mostrarEquipos(liga);
                break;

            case 4: {
                int id_buscar;
                cout << "\n--- CALCULO DE PODER OFENSIVO RECURSIVO ---" << endl;
                cout << "Ingrese el ID del equipo a analizar: ";
                cin >> id_buscar;
                
                Equipo* eq = buscarEquipoPorID(liga, id_buscar);
                if (eq != NULL) {
                    int poder = calcularPoderOfensivo(eq->plantilla);
                    cout << ">> El equipo " << eq->nombre_equipo << " tiene un poder ofensivo total de: " << poder << " goles." << endl;
                } else {
                    cout << ">> Error: Equipo no encontrado en el sistema." << endl;
                }
                break;
            }

            case 0:
                cout << "\nCerrando el sistema del torneo. Liberando memoria..." << endl;
                break;

            default:
                cout << "\n[!] Opcion invalida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);

    return 0;
}
