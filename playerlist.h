#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include "player.h"

void insertarplayer(Player *&head,
                    std::string name,
                    int number,
                    std::string team,
                    std::string position,
                    int gol);

struct PlayerData{
    std::string name;
    std::string team;
    int number;
    std::string position;
    int gol;
};


void insertarplayer(Player *&head,
                    std::string name,
                    int number,
                    std::string team,
                    std::string position,
                    int gol);


int cargarJugadores(Player *head, PlayerData arr[]);


void mergeSort(PlayerData arr[], int l, int r);


void merge(PlayerData arr[], int l, int m, int r);

void mostrarJugador(const PlayerData &p);

int contarJugadoresEquipo(Player *head, string equipo);

void copiarJugadoresEquipo(Player *head, string equipo, PlayerData arr[]);

void mostrarJugadoresEquipo();

int construirEquipo(Player *head, string equipo, PlayerData arr[]);

int busquedaBinariaDorsal(PlayerData arr[], int n, int dorsal);

int busquedaNombre(PlayerData arr[], int n, string nombre);

#endif
