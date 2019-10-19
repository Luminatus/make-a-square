#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <QVector>
#include <QMap>
#include "Headers/datastructures.h"

struct GameField
{
    Player* UpField;
    Player* DownField;
    Player* LeftField;
    Player* RightField;
    Player Owner = None;
    GameField(){}
    void set(Direction dir, Player p);
    Player get(Direction dir);
};

class GameMap{
    public:
        GameMap(int size);
        bool setLine(int row, int col, Direction dir, Player p);
        bool setOwner(int row, int col, Player p);
        Player getLine(int row, int col, Direction dir);
        Player getOwner(int row, int col);
        bool isValidIndex(int row, int col);
        bool isFilled(int row, int col);

        QString getName();
        int getSize();

    private:
        QVector<QVector<GameField> >* _map;
};

#endif // GAME_MAP_H
