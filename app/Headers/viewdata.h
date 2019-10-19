#ifndef VIEWDATA_H
#define VIEWDATA_H

#include "Headers/datastructures.h"
#include "Headers/gamesession_model.h"

struct ViewData
{
    ViewData(GameSessionModel* model);
    const int tableSize;
    int tileSize;
    int windowWidth;
    int windowHeight;

    GameStats GetGameStats();
    Player GetOwner(int,int);
    Player GetLine(int,int,Direction);
private:
    GameSessionModel* _gameSessionModel;
};

#endif // VIEWDATA_H
