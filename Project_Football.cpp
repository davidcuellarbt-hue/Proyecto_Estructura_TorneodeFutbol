#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;




struct Equipo {
    char    nombre_equipo[50];
    int     puntos;
    Equipo* izquierdo;
    Equipo* derecho;
};


//FUNCION INSERTAR EQUIPO
void insertarEquipo(Equipo* &arbol, const char* nombre) {

    
    if (arbol == NULL) {
        arbol = (Equipo*)malloc(sizeof(Equipo));
        strcpy(arbol->nombre_equipo, nombre);
        arbol->puntos    = 0;      
        arbol->izquierdo = NULL;
        arbol->derecho   = NULL;
        return;
    }

    
    if (strcmp(nombre, arbol->nombre_equipo) < 0) {
        insertarEquipo(arbol->izquierdo, nombre);  

    
    } else if (strcmp(nombre, arbol->nombre_equipo) > 0) {
        insertarEquipo(arbol->derecho, nombre);    

    
    } else {
        cout << ">> Error: ya existe un equipo llamado " << nombre << endl;
    }
}


//FUNCION BUSCAR EQUIPO POR NOMBRE
Equipo* buscarEquipoPorNombre(Equipo* arbol, const char* nombre) {

    if (arbol == NULL) {
        return NULL;
    }

    if (strcmp(arbol->nombre_equipo, nombre) == 0) {
        return arbol;
    }

    Equipo* encontrado = buscarEquipoPorNombre(arbol->izquierdo, nombre);
    if (encontrado != NULL) {
        return encontrado;
    }

    return buscarEquipoPorNombre(arbol->derecho, nombre);
}


//FUNCION MOSTRAR EQUIPOS


void mostrarEquipos(Equipo* arbol) {

    if (arbol == NULL) {
        return;
    }

    mostrarEquipos(arbol->izquierdo);           
    cout << arbol->nombre_equipo
         << " - Puntos: " << arbol->puntos << endl; 
    mostrarEquipos(arbol->derecho);               
}


//FUNCION TABLAS POR POR PUNTOS (DIVIDA EN TRES FUNCIONES)

// Parte A: extraer equipos del árbol a un arreglo 

void extraerEquipos(Equipo* arbol, Equipo* arreglo[], int &contador) {

    if (arbol == NULL) return;

    extraerEquipos(arbol->izquierdo, arreglo, contador);  
    arreglo[contador] = arbol;                             
    contador++;                                            
    extraerEquipos(arbol->derecho, arreglo, contador);   
}

// Parte B: Merge Sort por puntos 

void merge(Equipo* arreglo[], int izq, int mid, int der) {

    int tamLeft  = mid - izq + 1;
    int tamRight = der - mid;

    
    Equipo* Left[100];
    Equipo* Right[100];

    for (int i = 0; i < tamLeft; i++)
        Left[i] = arreglo[izq + i];
    for (int j = 0; j < tamRight; j++)
        Right[j] = arreglo[mid + 1 + j];

    int i = 0, j = 0, k = izq;
    
    while (i < tamLeft && j < tamRight) {
        if (Left[i]->puntos >= Right[j]->puntos) {
            arreglo[k] = Left[i];
            i++;
        } else {
            arreglo[k] = Right[j];
            j++;
        }
        k++;
    }

    
    while (i < tamLeft)  { arreglo[k] = Left[i];  i++; k++; }
    while (j < tamRight) { arreglo[k] = Right[j]; j++; k++; }
}

void mergeSort(Equipo* arreglo[], int izq, int der) {

    if (izq >= der) return;  

    int mid = (izq + der) / 2;

    mergeSort(arreglo, izq, mid);      
    mergeSort(arreglo, mid + 1, der);  
    merge(arreglo, izq, mid, der);     
}

// Parte C: la función del menú 

void tablaPosPorPuntos(Equipo* arbol) {

    Equipo* arreglo[100];   
    int contador = 0;

    extraerEquipos(arbol, arreglo, contador);

    mergeSort(arreglo, 0, contador - 1);

    cout << "       TABLA DE POSICIONES              " << endl;

    for (int i = 0; i < contador; i++) {
        cout << i + 1 << ". "
             << arreglo[i]->nombre_equipo
             << " - " << arreglo[i]->puntos << " pts" << endl;
    }
    cout << "========================================" << endl;
}

int main() {

    Equipo* liga = NULL;   

    int opcion;
    do {
        cout << "      LIGAMASTER       " << endl;
        cout << "1. Registrar nuevo equipo"               << endl;
        cout << "2. Buscar equipo por nombre"             << endl;
        cout << "3. Ver todos los equipos (alfabetico)"   << endl;
        cout << "4. Ver tabla de posiciones (por puntos)" << endl;
        cout << "0. Salir"                                << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {

            case 1: {
                char nombre[50];
                cout << "\n--- REGISTRAR EQUIPO ---" << endl;
                cout << "Nombre del equipo: ";
                cin >> nombre;
                insertarEquipo(liga, nombre);
                cout << ">> Equipo registrado correctamente." << endl;
                break;
            }

            case 2: {
                char nombre[50];
                cout << "\n--- BUSCAR EQUIPO ---" << endl;
                cout << "Nombre del equipo: ";
                cin >> nombre;
                Equipo* eq = buscarEquipoPorNombre(liga, nombre);
                if (eq != NULL) {
                    cout << ">> Encontrado: " << eq->nombre_equipo
                         << " - " << eq->puntos << " pts" << endl;
                } else {
                    cout << ">> No existe ese equipo." << endl;
                }
                break;
            }

            case 3:
                cout << "\n--- EQUIPOS REGISTRADOS ---" << endl;
                mostrarEquipos(liga);
                break;

            case 4:
                tablaPosPorPuntos(liga);
                break;

            case 0:
                cout << "\nSaliendo del programa...." << endl;
                break;

            default:
                cout << "\n[!] Opcion invalida." << endl;
        }

    } while (opcion != 0);

    return 0;
}

