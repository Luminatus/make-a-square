#ifndef GAME_PERSISTENCE_H
#define GAME_PERSISTENCE_H

#include <QObject>
#include "Headers/datastructures.h"
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
    GamePersistence(){}
    virtual ~GamePersistence(){}
    virtual QVector<SaveGameInfo>& getGameList(bool forceReload = false) = 0;
    virtual bool saveGame(SaveGameInfo info) = 0;

signals:
    void SaveFailed();
};

#endif // GAME_PERSISTENCE_H
