#include "estructuras.h"
#include "playerlist.h"
#include <cstdlib>   // para rand()
#include <ctime>

void insertarEquipo(Equipo* &arbol, const std::string& nombre) {
    if (arbol == nullptr) {
        arbol = new Equipo();
        arbol->nombre = nombre;
        return;
    }

    if (nombre < arbol->nombre) {
        insertarEquipo(arbol->izquierdo, nombre);
    } else if (nombre > arbol->nombre) {
        insertarEquipo(arbol->derecho, nombre);
    }
}

Equipo* buscarEquipoPorNombre(Equipo* arbol, const std::string& nombre) {
    if (arbol == nullptr) return nullptr;
    if (arbol->nombre == nombre) return arbol;

    if (nombre < arbol->nombre) {
        return buscarEquipoPorNombre(arbol->izquierdo, nombre);
    } else {
        return buscarEquipoPorNombre(arbol->derecho, nombre);
    }
}

int contarEquipos(Equipo* arbol) {
    if (arbol == nullptr) return 0;
    return 1 + contarEquipos(arbol->izquierdo) + contarEquipos(arbol->derecho);
}


void extraerEquipos(Equipo* arbol, Equipo* arreglo[], int& contador) {
    if (arbol == nullptr) return;
    extraerEquipos(arbol->izquierdo, arreglo, contador);
    arreglo[contador] = arbol;
    contador++;
    extraerEquipos(arbol->derecho, arreglo, contador);
}

void merge(Equipo* arreglo[], int izq, int mid, int der) {
    int tamLeft = mid - izq + 1;
    int tamRight = der - mid;

    Equipo** Left = new Equipo*[tamLeft];
    Equipo** Right = new Equipo*[tamRight];

    for (int i = 0; i < tamLeft; i++) Left[i] = arreglo[izq + i];
    for (int j = 0; j < tamRight; j++) Right[j] = arreglo[mid + 1 + j];

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

    while (i < tamLeft) { arreglo[k] = Left[i]; i++; k++; }
    while (j < tamRight) { arreglo[k] = Right[j]; j++; k++; }

    delete[] Left;
    delete[] Right;
}

void mergeSortEquipos(Equipo* arreglo[], int izq, int der) {
    if (izq >= der) return;
    int mid = (izq + der) / 2;
    mergeSortEquipos(arreglo, izq, mid);
    mergeSortEquipos(arreglo, mid + 1, der);
    merge(arreglo, izq, mid, der);
}

int obtenerTablaPosiciones(Equipo* arbol, Equipo* arreglo[]) {
    int contador = 0;
    extraerEquipos(arbol, arreglo, contador);
    if (contador > 1) {
        mergeSortEquipos(arreglo, 0, contador - 1);
    }
    return contador;
}


Jornada calendario[19];
bool calendarioListo = false;

void generarCalendario(Equipo* arbol) {
    int contador = 0;
    asignarIDs(arbol, contador);

    int equipos[20];
    for (int i = 0; i < 20; i++) equipos[i] = i;

    for (int j = 0; j < 19; j++) {
        for (int p = 0; p < 10; p++) {
            int local = equipos[p];
            int visitante = equipos[19 - p];

            calendario[j].partidos[p].id_local = local;
            calendario[j].partidos[p].id_visitante = visitante;
            calendario[j].partidos[p].goles_local = -1;
            calendario[j].partidos[p].goles_visitante = -1;
            calendario[j].partidos[p].jugado = false;
        }

        int ultimo = equipos[19];
        for (int i = 19; i > 1; i--) {
            equipos[i] = equipos[i - 1];
        }
        equipos[1] = ultimo;
    }

    calendarioListo = true;
}

std::string nombreEquipoPorID(Equipo* arbol, int id) {
    Equipo* eq = buscarEquipoPorID(arbol, id);
    if (eq != nullptr) return eq->nombre;
    return "Desconocido";
}

bool calendarioGenerado() {
    return calendarioListo;
}

void asignarIDs(Equipo* arbol, int& contador) {
    if (arbol == nullptr) return;
    asignarIDs(arbol->izquierdo, contador);
    arbol->id = contador++;
    asignarIDs(arbol->derecho, contador);
}

Equipo* buscarEquipoPorID(Equipo* arbol, int id) {
    if (arbol == nullptr) return nullptr;
    if (arbol->id == id) return arbol;

    Equipo* encontrado = buscarEquipoPorID(arbol->izquierdo, id);
    if (encontrado != nullptr) return encontrado;

    return buscarEquipoPorID(arbol->derecho, id);
}


bool partidoJugado(int jornada, int partido) {
    return calendario[jornada].partidos[partido].jugado;
}

bool registrarResultado(Equipo* arbol, int jornada, int partido, int golesLocal, int golesVisitante) {
    if (partidoJugado(jornada, partido)) {
        return false;
    }

    Partido& p = calendario[jornada].partidos[partido];

    // Guardar resultado
    p.goles_local = golesLocal;
    p.goles_visitante = golesVisitante;
    p.jugado = true;

    // Buscar equipos y actualizar estadísticas
    Equipo* local = buscarEquipoPorID(arbol, p.id_local);
    Equipo* visitante = buscarEquipoPorID(arbol, p.id_visitante);

    if (local == nullptr || visitante == nullptr) {
        return false;
    }

    // Actualizar partidos jugados
    local->pj++;
    visitante->pj++;

    // Actualizar goles
    local->goles_favor += golesLocal;
    local->goles_contra += golesVisitante;
    visitante->goles_favor += golesVisitante;
    visitante->goles_contra += golesLocal;


    local->diferencia_goles = local->goles_favor - local->goles_contra;
    visitante->diferencia_goles = visitante->goles_favor - visitante->goles_contra;


    if (golesLocal > golesVisitante) {

        local->ganados++;
        local->puntos += 3;
        visitante->perdidos++;
    } else if (golesLocal < golesVisitante) {

        visitante->ganados++;
        visitante->puntos += 3;
        local->perdidos++;
    } else {

        local->empatados++;
        local->puntos += 1;
        visitante->empatados++;
        visitante->puntos += 1;
    }

    return true;
}

void registrarJugadoresEquipo(Player* &head, const std::string& equipo) {
    struct Plantilla {
        std::string nombre;
        int dorsal;
        std::string posicion;
    };

    Plantilla jugadores[11] = {
        {"Portero",           1,  "POR"},
        {"Defensa Central A", 2,  "DFC"},
        {"Defensa Central B", 3,  "DFC"},
        {"Lateral Derecho",   4,  "LTD"},
        {"Lateral Izquierdo", 5,  "LTI"},
        {"Medio Defensivo",   6,  "MCD"},
        {"Medio Central",     7,  "MC"},
        {"Medio Ofensivo",    8,  "MCO"},
        {"Extremo Derecho",   9,  "ED"},
        {"Extremo Izquierdo", 10, "EI"},
        {"Delantero Centro",  11, "DEL"}
    };

    for (int i = 0; i < 11; i++) {
        insertarplayer(head, jugadores[i].nombre, jugadores[i].dorsal,
                       equipo, jugadores[i].posicion, 0);
    }
}



void registrarEquiposPrueba(Equipo* &arbol, Player* &head) {
    std::string equipos[20] = {
        "America", "Atlas", "Atletico San Luis", "Cruz Azul", "FC Juarez",
        "Guadalajara", "Leon", "Mazatlan", "Monterrey", "Necaxa",
        "Pachuca", "Puebla", "Pumas UNAM", "Queretaro", "Santos Laguna",
        "Tigres UANL", "Tijuana", "Toluca", "UANL", "Veracruz"
    };

    for (int i = 0; i < 20; i++) {
        if (buscarEquipoPorNombre(arbol, equipos[i]) == nullptr) {
            insertarEquipo(arbol, equipos[i]);
        }

        registrarJugadoresEquipo(head, equipos[i]);
    }
}



Player* buscarJugador(Player *head, std::string nombre, std::string equipo) {
    Player *aux = head;
    while (aux != nullptr) {
        if (aux->name == nombre && aux->team == equipo) {
            return aux;
        }
        aux = aux->next;
    }
    return nullptr;
}

void registrarGolJugador(Player *head, std::string nombre, std::string equipo) {
    Player *j = buscarJugador(head, nombre, equipo);
    if (j != nullptr) {
        j->gol++;
    }
}

bool puedeAsignarGolLocal(int jornada, int partido) {
    if (jornada < 0 || jornada >= 19 || partido < 0 || partido >= 10) return false;
    Partido &p = calendario[jornada].partidos[partido];
    return p.jugado && p.golesLocalAsignados < p.goles_local;
}

bool puedeAsignarGolVisitante(int jornada, int partido) {
    if (jornada < 0 || jornada >= 19 || partido < 0 || partido >= 10) return false;
    Partido &p = calendario[jornada].partidos[partido];
    return p.jugado && p.golesVisitanteAsignados < p.goles_visitante;
}

int golesPendientesLocal(int jornada, int partido) {
    if (jornada < 0 || jornada >= 19 || partido < 0 || partido >= 10) return 0;
    Partido &p = calendario[jornada].partidos[partido];
    return p.goles_local - p.golesLocalAsignados;
}

int golesPendientesVisitante(int jornada, int partido) {
    if (jornada < 0 || jornada >= 19 || partido < 0 || partido >= 10) return 0;
    Partido &p = calendario[jornada].partidos[partido];
    return p.goles_visitante - p.golesVisitanteAsignados;
}

bool asignarGolAJugador(int jornada, int partido, bool esLocal, int numGol,
                        Player *head, std::string nombreJugador, std::string equipo) {
    if (jornada < 0 || jornada >= 19 || partido < 0 || partido >= 10) return false;

    Partido &p = calendario[jornada].partidos[partido];
    if (!p.jugado) return false;

    if (esLocal) {
        if (numGol < 0 || numGol >= p.goles_local) return false;
        if (!p.autorGolLocal[numGol].empty()) return false;  // Ya fue asignado

        p.autorGolLocal[numGol] = nombreJugador;
        p.golesLocalAsignados++;
    } else {
        if (numGol < 0 || numGol >= p.goles_visitante) return false;
        if (!p.autorGolVisitante[numGol].empty()) return false;

        p.autorGolVisitante[numGol] = nombreJugador;
        p.golesVisitanteAsignados++;
    }

    registrarGolJugador(head, nombreJugador, equipo);
    return true;
}



Eliminatorias eliminatoriasData;

bool generarEliminatorias(Equipo* arbol) {
    if (eliminatoriasData.iniciadas) return false;
    if (contarEquipos(arbol) < 20) return false;

    Equipo* arreglo[100];
    int n = obtenerTablaPosiciones(arbol, arreglo);
    if (n < 8) return false;

    // Bracket: 1°vs8°, 2°vs7°, 3°vs6°, 4°vs5°
    eliminatoriasData.cuartos[0].id_local = arreglo[0]->id;
    eliminatoriasData.cuartos[0].id_visitante = arreglo[7]->id;
    eliminatoriasData.cuartos[1].id_local = arreglo[1]->id;
    eliminatoriasData.cuartos[1].id_visitante = arreglo[6]->id;
    eliminatoriasData.cuartos[2].id_local = arreglo[2]->id;
    eliminatoriasData.cuartos[2].id_visitante = arreglo[5]->id;
    eliminatoriasData.cuartos[3].id_local = arreglo[3]->id;
    eliminatoriasData.cuartos[3].id_visitante = arreglo[4]->id;

    for (int i = 0; i < 4; i++) {
        eliminatoriasData.cuartos[i].goles_local = -1;
        eliminatoriasData.cuartos[i].goles_visitante = -1;
        eliminatoriasData.cuartos[i].jugado = false;
        eliminatoriasData.cuartos[i].ganador_id = -1;
        eliminatoriasData.cuartos[i].perdedor_id = -1;
        eliminatoriasData.cuartos[i].golesLocalAsignados = 0;
        eliminatoriasData.cuartos[i].golesVisitanteAsignados = 0;
        for (int j = 0; j < 20; j++) {
            eliminatoriasData.cuartos[i].autorGolLocal[j].clear();
            eliminatoriasData.cuartos[i].autorGolVisitante[j].clear();
        }
    }

    eliminatoriasData.cuartosGenerados = true;
    eliminatoriasData.iniciadas = true;
    return true;
}

bool registrarResultadoEliminatoria(Equipo* arbol, int fase, int partido, int golesLocal, int golesVisitante) {
    if (fase < 0 || fase > 2) return false;
    if (partido < 0) return false;

    PartidoEliminatoria* p = nullptr;
    if (fase == 0 && partido < 4) p = &eliminatoriasData.cuartos[partido];
    else if (fase == 1 && partido < 2) p = &eliminatoriasData.semis[partido];
    else if (fase == 2 && partido == 0) p = &eliminatoriasData.final;
    else return false;

    if (p->jugado) return false;
    if (p->id_local == -1 || p->id_visitante == -1) return false;
    if (golesLocal < 0 || golesVisitante < 0) return false;
    if (golesLocal == golesVisitante) return false; // No empates en eliminatoria

    p->goles_local = golesLocal;
    p->goles_visitante = golesVisitante;
    p->jugado = true;

    if (golesLocal > golesVisitante) {
        p->ganador_id = p->id_local;
        p->perdedor_id = p->id_visitante;
    } else {
        p->ganador_id = p->id_visitante;
        p->perdedor_id = p->id_local;
    }

    return true;
}

int avanzarFaseEliminatoria() {
    if (!eliminatoriasData.iniciadas) return 0;

    // Cuartos → Semis
    if (!eliminatoriasData.semisGeneradas) {
        bool todos = true;
        for (int i = 0; i < 4; i++) if (!eliminatoriasData.cuartos[i].jugado) { todos = false; break; }
        if (todos) {
            eliminatoriasData.semis[0].id_local = eliminatoriasData.cuartos[0].ganador_id;
            eliminatoriasData.semis[0].id_visitante = eliminatoriasData.cuartos[2].ganador_id;
            eliminatoriasData.semis[1].id_local = eliminatoriasData.cuartos[1].ganador_id;
            eliminatoriasData.semis[1].id_visitante = eliminatoriasData.cuartos[3].ganador_id;

            for (int i = 0; i < 2; i++) {
                eliminatoriasData.semis[i].goles_local = -1;
                eliminatoriasData.semis[i].goles_visitante = -1;
                eliminatoriasData.semis[i].jugado = false;
                eliminatoriasData.semis[i].ganador_id = -1;
                eliminatoriasData.semis[i].perdedor_id = -1;
                eliminatoriasData.semis[i].golesLocalAsignados = 0;
                eliminatoriasData.semis[i].golesVisitanteAsignados = 0;
                for (int j = 0; j < 20; j++) {
                    eliminatoriasData.semis[i].autorGolLocal[j].clear();
                    eliminatoriasData.semis[i].autorGolVisitante[j].clear();
                }
            }
            eliminatoriasData.semisGeneradas = true;
            return 1;
        }
    }

    // Semis → Final
    if (!eliminatoriasData.finalGenerada && eliminatoriasData.semisGeneradas) {
        bool todos = true;
        for (int i = 0; i < 2; i++) if (!eliminatoriasData.semis[i].jugado) { todos = false; break; }
        if (todos) {
            eliminatoriasData.final.id_local = eliminatoriasData.semis[0].ganador_id;
            eliminatoriasData.final.id_visitante = eliminatoriasData.semis[1].ganador_id;

            eliminatoriasData.final.goles_local = -1;
            eliminatoriasData.final.goles_visitante = -1;
            eliminatoriasData.final.jugado = false;
            eliminatoriasData.final.ganador_id = -1;
            eliminatoriasData.final.perdedor_id = -1;
            eliminatoriasData.final.golesLocalAsignados = 0;
            eliminatoriasData.final.golesVisitanteAsignados = 0;
            for (int j = 0; j < 20; j++) {
                eliminatoriasData.final.autorGolLocal[j].clear();
                eliminatoriasData.final.autorGolVisitante[j].clear();
            }
            eliminatoriasData.finalGenerada = true;
            return 2;
        }
    }

    // Final terminada
    if (eliminatoriasData.finalGenerada && eliminatoriasData.final.jugado) {
        return 3;
    }

    return 0;
}

std::string nombreEquipoEliminatoria(Equipo* arbol, int fase, int partido, bool esLocal) {
    int id = -1;
    if (fase == 0 && partido >= 0 && partido < 4) {
        id = esLocal ? eliminatoriasData.cuartos[partido].id_local : eliminatoriasData.cuartos[partido].id_visitante;
    } else if (fase == 1 && partido >= 0 && partido < 2) {
        id = esLocal ? eliminatoriasData.semis[partido].id_local : eliminatoriasData.semis[partido].id_visitante;
    } else if (fase == 2 && partido == 0) {
        id = esLocal ? eliminatoriasData.final.id_local : eliminatoriasData.final.id_visitante;
    }
    return nombreEquipoPorID(arbol, id);
}

std::string nombreCampeon(Equipo* arbol) {
    if (eliminatoriasData.final.jugado && eliminatoriasData.final.ganador_id != -1)
        return nombreEquipoPorID(arbol, eliminatoriasData.final.ganador_id);
    return "";
}

PlayerData obtenerBotaOro(Player *head) {
    PlayerData arr[250];
    int n = cargarJugadores(head, arr);
    if (n > 1) mergeSort(arr, 0, n - 1);
    if (n > 0) return arr[0];
    return PlayerData{"", "", 0, "", 0};
}

bool partidoEliminatoriaJugado(int fase, int partido) {
    if (fase == 0 && partido >= 0 && partido < 4) return eliminatoriasData.cuartos[partido].jugado;
    if (fase == 1 && partido >= 0 && partido < 2) return eliminatoriasData.semis[partido].jugado;
    if (fase == 2 && partido == 0) return eliminatoriasData.final.jugado;
    return false;
}

bool puedeAsignarGolLocalElim(int fase, int partido) {
    PartidoEliminatoria* p = nullptr;
    if (fase == 0 && partido < 4) p = &eliminatoriasData.cuartos[partido];
    else if (fase == 1 && partido < 2) p = &eliminatoriasData.semis[partido];
    else if (fase == 2 && partido == 0) p = &eliminatoriasData.final;
    if (!p) return false;
    return p->jugado && p->golesLocalAsignados < p->goles_local;
}

bool puedeAsignarGolVisitanteElim(int fase, int partido) {
    PartidoEliminatoria* p = nullptr;
    if (fase == 0 && partido < 4) p = &eliminatoriasData.cuartos[partido];
    else if (fase == 1 && partido < 2) p = &eliminatoriasData.semis[partido];
    else if (fase == 2 && partido == 0) p = &eliminatoriasData.final;
    if (!p) return false;
    return p->jugado && p->golesVisitanteAsignados < p->goles_visitante;
}

bool asignarGolAJugadorElim(int fase, int partido, bool esLocal, int numGol,
                            Player *head, std::string nombreJugador, std::string equipo) {
    PartidoEliminatoria* p = nullptr;
    if (fase == 0 && partido < 4) p = &eliminatoriasData.cuartos[partido];
    else if (fase == 1 && partido < 2) p = &eliminatoriasData.semis[partido];
    else if (fase == 2 && partido == 0) p = &eliminatoriasData.final;
    if (!p || !p->jugado) return false;

    if (esLocal) {
        if (numGol < 0 || numGol >= p->goles_local) return false;
        if (!p->autorGolLocal[numGol].empty()) return false;
        p->autorGolLocal[numGol] = nombreJugador;
        p->golesLocalAsignados++;
    } else {
        if (numGol < 0 || numGol >= p->goles_visitante) return false;
        if (!p->autorGolVisitante[numGol].empty()) return false;
        p->autorGolVisitante[numGol] = nombreJugador;
        p->golesVisitanteAsignados++;
    }

    registrarGolJugador(head, nombreJugador, equipo);
    return true;
}


void registrarResultadosPrueba(Equipo* arbol, Player* head) {
    srand(time(nullptr));

    for (int j = 0; j < 19; j++) {
        for (int p = 0; p < 10; p++) {
            Partido& part = calendario[j].partidos[p];

            if (part.jugado) continue;  // Saltar si ya fue registrado

            // Generar goles aleatorios (0 a 4)
            int golesLocal = rand() % 5;
            int golesVisitante = rand() % 5;

            // Evitar empate en eliminatorias no aplica aquí, en fase regular sí hay empates
            // Registrar resultado (actualiza estadísticas de equipos)
            registrarResultado(arbol, j, p, golesLocal, golesVisitante);

            // Asignar goles a jugadores aleatorios del equipo local
            if (golesLocal > 0 && head != nullptr) {
                // Contar jugadores del equipo local
                std::string equipoLocal = nombreEquipoPorID(arbol, part.id_local);
                int n = contarJugadoresEquipo(head, equipoLocal);
                if (n > 0) {
                    PlayerData *arr = new PlayerData[n];
                    copiarJugadoresEquipo(head, equipoLocal, arr);

                    for (int g = 0; g < golesLocal; g++) {
                        int idx = rand() % n;
                        asignarGolAJugador(j, p, true, g, head, arr[idx].name, equipoLocal);
                    }
                    delete[] arr;
                }
            }

            // Asignar goles a jugadores aleatorios del equipo visitante
            if (golesVisitante > 0 && head != nullptr) {
                std::string equipoVisit = nombreEquipoPorID(arbol, part.id_visitante);
                int n = contarJugadoresEquipo(head, equipoVisit);
                if (n > 0) {
                    PlayerData *arr = new PlayerData[n];
                    copiarJugadoresEquipo(head, equipoVisit, arr);

                    for (int g = 0; g < golesVisitante; g++) {
                        int idx = rand() % n;
                        asignarGolAJugador(j, p, false, g, head, arr[idx].name, equipoVisit);
                    }
                    delete[] arr;
                }
            }
        }
    }
}