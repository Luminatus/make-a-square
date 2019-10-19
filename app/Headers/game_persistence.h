#ifndef GAME_PERSISTENCE_H
#define GAME_PERSISTENCE_H
#include <QObject>
#include "datastructures.h"
#include <QVector>
#include <QLinkedList>

struct LogEntry
{
    int Row;
    int Column;
    Direction Dir;
    LogEntry(int row, int col, Direction dir): Row(row), Column(col), Dir(dir){}
    LogEntry(){}
};

struct SaveGameInfo
{
    QString FileName;
    int TableSize;
    int TurnCount;
    QLinkedList<LogEntry> Log;
    SaveGameInfo(){}
    SaveGameInfo(QString name, int size, int turns, QLinkedList<LogEntry> log) :
        FileName(name),
        TableSize(size),
        TurnCount(turns),
        Log(log){}
};

class GamePersistence : public QObject
{
    Q_OBJECT
public:
    const QString SAVEGAME_FOLDER = "saves";

    GamePersistence(){}
    ~GamePersistence(){}
    QVector<SaveGameInfo>& getGameList(bool forceReload = false);
    bool saveGame(SaveGameInfo info);

private:
    QVector<SaveGameInfo> gameList;

    void loadGameList();
    void loadGameInfoFromFile(QString filename);
};

#endif // GAME_PERSISTENCE_H
