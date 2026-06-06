#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

struct Player{
    string id;
    string name;
    int number;
    string team;
    string position;
    int gol;
    Player *next;
};


#endif
