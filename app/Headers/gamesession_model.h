#ifndef GAMESESSIONMODEL_H
#define GAMESESSIONMODEL_H
#include <QObject>
#include <QVector>
#include <QTimer>
#include <QLinkedList>
#include "Headers/datastructures.h"
#include "Headers/game_map.h"
#include "Headers/game_persistence.h"


class GameSessionModel : public QObject
{
    Q_OBJECT
public:
    GameSessionModel(GamePersistence* persistence);
    ~GameSessionModel();
    void init();
    bool loadGame(uint);
    bool newGame(uint);
    bool saveGame();
    int GetTableSize();
    bool IsGameWon();
    GameStats GetGameStats();
    Player GetOwner(int,int);
    Player GetLine(int,int, Direction);
    Player GetOwner(Position);
    Player GetLine(Position, Direction);
    bool MakeMove(int x,int y, Direction);
private:
    bool isFilled(int row, int col);
    bool isValidMove(int x,int y, Direction dir);
    void endGame();

    GameMap* _gameMap;
    GamePersistence* _persistence;
    SaveGameInfo _info;

    bool _gameInitialized = false;
    Player _currentPlayer;

signals:
    void GameWon(Player);
    void Draw();
};

#endif // GAMESESSIONMODEL_H
