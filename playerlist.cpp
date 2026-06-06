#include "playerlist.h"
#include <iostream>

using namespace std;



void insertarplayer(Player *&head,
                    string name,
                    int number,
                    string team,
                    string position,
                    int gol){

    string teamID = team;

    for(int i = 0; i < (int)teamID.length(); i++){
        if(teamID[i] == ' '){
            teamID[i] = '_';
        }
    }

    Player *nuevoplay = new Player();

    nuevoplay->name = name;
    nuevoplay->number = number;
    nuevoplay->position = position;
    nuevoplay->gol = gol;
    nuevoplay->team = team;
    nuevoplay->id = teamID+to_string(number);

    nuevoplay->next = head;
    head = nuevoplay;
}

int cargarJugadores(Player *head, PlayerData arr[]){

    Player *aux = head;
    int i = 0;

    while(aux != NULL){

        arr[i].name = aux->name;
        arr[i].team = aux->team;
        arr[i].number = aux->number;
        arr[i].position = aux->position;
        arr[i].gol = aux->gol;

        aux = aux->next;
        i++;
    }

    return i;
}


void merge(PlayerData arr[], int l, int m, int r){

    int n1 = m - l + 1;
    int n2 = r - m;

    PlayerData *L = new PlayerData[n1];
    PlayerData *R = new PlayerData[n2];

    for(int i = 0; i < n1; i++) L[i] = arr[l + i];
    for(int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while(i < n1 && j < n2){

        if(L[i].gol >= R[j].gol){
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(PlayerData arr[], int l, int r){

    if(l < r){
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int contarJugadoresEquipo(Player *head, string equipo){

    int count = 0;

    Player *aux = head;

    while(aux != NULL){

        if(aux->team == equipo){
            count++;
        }

        aux = aux->next;
    }

    return count;
}

void copiarJugadoresEquipo(Player *head,
                           string equipo,
                           PlayerData arr[]){

    Player *aux = head;

    int i = 0;

    while(aux != NULL){

        if(aux->team == equipo){

            arr[i].name = aux->name;
            arr[i].team = aux->team;
            arr[i].number = aux->number;
            arr[i].position = aux->position;
            arr[i].gol = aux->gol;

            i++;
        }

        aux = aux->next;
    }
}

int construirEquipo(Player *head, string equipo, PlayerData arr[]){

    Player *aux = head;
    int n = 0;

    while(aux != NULL){

        if(aux->team == equipo){

            arr[n].name = aux->name;
            arr[n].team = aux->team;
            arr[n].number = aux->number;
            arr[n].position = aux->position;
            arr[n].gol = aux->gol;

            n++;
        }

        aux = aux->next;
    }

    return n;
}

int busquedaBinariaDorsal(PlayerData arr[], int n, int dorsal){

    int l = 0;
    int r = n - 1;

    while(l <= r){

        int m = (l + r) / 2;

        if(arr[m].number == dorsal){
            return m;
        }

        if(arr[m].number < dorsal){
            l = m + 1;
        } else {
            r = m - 1;
        }
    }

    return -1;
}

int busquedaNombre(PlayerData arr[], int n, string nombre){

    for(int i = 0; i < n; i++){

        if(arr[i].name == nombre){
            return i;
        }
    }

    return -1;
}