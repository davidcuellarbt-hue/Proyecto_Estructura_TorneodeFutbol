#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;




struct Equipo {
    char    nombre_equipo[50];
    int     puntos;          
    int     pj;             
    int     ganados;
    int     empatados;
    int     perdidos;
    int     goles_favor;
    int     goles_contra;
    int     diferencia_goles;  
    Equipo* izquierdo;
    Equipo* derecho;
};

//FUNCION INSERTAR EQUIPO
int contarEquipos(Equipo* arbol) {
    if (arbol == NULL) {
        return 0;
    }
    return 1 + contarEquipos(arbol->izquierdo) + contarEquipos(arbol->derecho);
}


void insertarEquipo(Equipo* &arbol, const char* nombre) {

    
    if (arbol == NULL) {
        arbol = (Equipo*)malloc(sizeof(Equipo));
        strcpy(arbol->nombre_equipo, nombre);
        arbol->puntos    = 0; 
		arbol->pj               = 0;
		arbol->ganados          = 0;
		arbol->empatados        = 0;
		arbol->perdidos         = 0;
		arbol->goles_favor      = 0;
		arbol->goles_contra     = 0;
		arbol->diferencia_goles = 0;     
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
    cout << " - " << arbol->nombre_equipo
         << " | Pts: " << arbol->puntos
         << " | PJ: "  << arbol->pj << endl;
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
 
        bool leftPrimero = false;
 
        if (Left[i]->puntos > Right[j]->puntos) {
            leftPrimero = true;
 
        } else if (Left[i]->puntos == Right[j]->puntos) {
            if (Left[i]->diferencia_goles > Right[j]->diferencia_goles) {
                leftPrimero = true;
 
            } else if (Left[i]->diferencia_goles == Right[j]->diferencia_goles) {
                if (Left[i]->goles_favor >= Right[j]->goles_favor) {
                    leftPrimero = true;
                }
            }
        }
 
        if (leftPrimero) {
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
    cout << " Pos  Club                PJ   G   E   P   GF  GC  DG   Pts"          << endl;

    for (int i = 0; i < contador; i++) {
        cout << "  " << i + 1 << ".  ";
 
        cout << arreglo[i]->nombre_equipo;
        int espacios = 20 - strlen(arreglo[i]->nombre_equipo);
        for (int s = 0; s < espacios; s++) cout << " ";
 
        cout << arreglo[i]->pj               << "    "
             << arreglo[i]->ganados          << "   "
             << arreglo[i]->empatados        << "   "
             << arreglo[i]->perdidos         << "   "
             << arreglo[i]->goles_favor      << "   "
             << arreglo[i]->goles_contra     << "   "
             << arreglo[i]->diferencia_goles << "    "
             << arreglo[i]->puntos
             << endl;
    }
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
            	if (contarEquipos(liga) >= 20) {
        		cout << ">> Error: El torneo ya esta lleno. Se alcanzo el limite de 20 equipos para la creacion de jornadas." << endl;
        		break; 
    			}
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
                cin  >> nombre;
 
                Equipo* eq = buscarEquipoPorNombre(liga, nombre);
                if (eq != NULL) {
                    cout << ">> Encontrado: " << eq->nombre_equipo
                         << " | Pts: "        << eq->puntos
                         << " | PJ: "         << eq->pj
                         << " | G: "          << eq->ganados
                         << " | E: "          << eq->empatados
                         << " | P: "          << eq->perdidos
                         << " | GF: "         << eq->goles_favor
                         << " | GC: "         << eq->goles_contra
                         << " | DG: "         << eq->diferencia_goles
                         << endl;
                } else {
                    cout << ">> No se encontro ningun equipo con ese nombre, intenta de nuevo."
                         << endl;
                }
                break;
            }

            case 3:
                cout << "\nEQUIPOS REGISTRADOS" << endl;
                if (liga == NULL) {
                    cout << ">> No hay equipos registrados aun, vuelvelo a intentar." << endl;
                } else {
                    mostrarEquipos(liga);
                }
                break;
            case 4:
                tablaPosPorPuntos(liga);
                break;

            case 0:
                cout << "\nEstas saliendo del programa...." << endl;
                break;

            default:
                cout << "\n[!] Opcion invalida." << endl;
        }

    } while (opcion != 0);

    return 0;
}

