#ifndef GAME_PERSISTENCE_MOCK_H
#define GAME_PERSISTENCE_MOCK_H

#include "game_persistence.h"

class GamePersistenceMock : public GamePersistence
{
public:
    GamePersistenceMock(){
        QVector<SaveGameInfo> list(3);
        QLinkedList<LogEntry> log;
        list[0] = SaveGameInfo("empty_test",5,log.size(),log);
        log<<LogEntry(1,1,Up)<<LogEntry(1,1,Down)<<LogEntry(1,1,Left)<<LogEntry(1,1,Right);
        list[1] = SaveGameInfo("Red 1", 5,log.size(),log);
        log<<LogEntry(2,1,Right)<<LogEntry(2,1,Left)<<LogEntry(3,1,Left)<<LogEntry(3,1,Right)
          <<LogEntry(3,1,Down);
        list[2] = SaveGameInfo("Almost Double", 5, log.size(), log);

        gameList = list;
    }
    ~GamePersistenceMock(){}
    QVector<SaveGameInfo>& getGameList(bool forceReload = false)
    {
        return gameList;
    }

    bool saveGame(SaveGameInfo info)
    {
        qDebug("Game saved");
    }

private:
    QVector<SaveGameInfo> gameList;
};

#endif // GAME_PERSISTENCE_MOCK_H
