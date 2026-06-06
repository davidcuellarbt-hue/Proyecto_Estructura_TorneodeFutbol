#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include "player.h"
#include "playerlist.h"

struct Equipo {
    std::string nombre;

    int id = -1;

    int puntos = 0;
    int pj = 0;
    int ganados = 0;
    int empatados = 0;
    int perdidos = 0;
    int goles_favor = 0;
    int goles_contra = 0;
    int diferencia_goles = 0;

    Equipo* izquierdo = nullptr;
    Equipo* derecho = nullptr;
};

void insertarEquipo(Equipo* &arbol, const std::string& nombre);

Equipo* buscarEquipoPorNombre(Equipo* arbol, const std::string& nombre);

int contarEquipos(Equipo* arbol);

void asignarIDs(Equipo* arbol, int& contador);
Equipo* buscarEquipoPorID(Equipo* arbol, int id);
void extraerEquipos(Equipo* arbol, Equipo* arreglo[], int& contador);
void mergeSortEquipos(Equipo* arreglo[], int izq, int der);
int obtenerTablaPosiciones(Equipo* arbol, Equipo* arreglo[]);


struct Partido {
    int id_local;
    int id_visitante;
    int goles_local = -1;
    int goles_visitante = -1;
    bool jugado = false;

    int golesLocalAsignados = 0;
    int golesVisitanteAsignados = 0;
    std::string autorGolLocal[20];
    std::string autorGolVisitante[20];
};

struct Jornada {
    Partido partidos[10];
};


extern Jornada calendario[19];
extern bool calendarioListo;


void generarCalendario(Equipo* arbol);
std::string nombreEquipoPorID(Equipo* arbol, int id);
bool calendarioGenerado();

bool registrarResultado(Equipo* arbol, int jornada, int partido, int golesLocal, int golesVisitante);
bool partidoJugado(int jornada, int partido);
void registrarEquiposPrueba(Equipo* &arbol, Player* &head);
bool puedeAsignarGolLocal(int jornada, int partido);
bool puedeAsignarGolVisitante(int jornada, int partido);
int golesPendientesLocal(int jornada, int partido);
int golesPendientesVisitante(int jornada, int partido);

Player* buscarJugador(Player *head, std::string nombre, std::string equipo);
void registrarGolJugador(Player *head, std::string nombre, std::string equipo);

bool asignarGolAJugador(int jornada, int partido, bool esLocal, int numGol,
                        Player *head, std::string nombreJugador, std::string equipo);


struct PartidoEliminatoria {
    int id_local = -1;
    int id_visitante = -1;
    int goles_local = -1;
    int goles_visitante = -1;
    bool jugado = false;
    int ganador_id = -1;
    int perdedor_id = -1;


    int golesLocalAsignados = 0;
    int golesVisitanteAsignados = 0;
    std::string autorGolLocal[20];
    std::string autorGolVisitante[20];
};

struct Eliminatorias {
    PartidoEliminatoria cuartos[4];
    PartidoEliminatoria semis[2];
    PartidoEliminatoria final;
    bool cuartosGenerados = false;
    bool semisGeneradas = false;
    bool finalGenerada = false;
    bool iniciadas = false;
};

extern Eliminatorias eliminatoriasData;


bool generarEliminatorias(Equipo* arbol);


bool registrarResultadoEliminatoria(Equipo* arbol, int fase, int partido, int golesLocal, int golesVisitante);
bool partidoEliminatoriaJugado(int fase, int partido);


int avanzarFaseEliminatoria();


std::string nombreEquipoEliminatoria(Equipo* arbol, int fase, int partido, bool esLocal);


std::string nombreCampeon(Equipo* arbol);
PlayerData obtenerBotaOro(Player *head);

void registrarResultadosPrueba(Equipo* arbol, Player* head);

bool puedeAsignarGolLocalElim(int fase, int partido);
bool puedeAsignarGolVisitanteElim(int fase, int partido);
bool asignarGolAJugadorElim(int fase, int partido, bool esLocal, int numGol,
Player *head, std::string nombreJugador, std::string equipo);


#endif