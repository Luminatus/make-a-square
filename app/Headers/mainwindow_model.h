#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "Headers/gamesession_model.h"
#include "Headers/game_map.h"
#include "Headers/game_persistence.h"

class MainWindowModel : public QObject
{
    Q_OBJECT
public:
    MainWindowModel();
    ~MainWindowModel();
    void NewGame(uint n);
    void LoadGame(uint n);
    QVector<SaveGameInfo> GetMaps(bool = false);
private:
    GameSessionModel* _gameSessionModel;
    GamePersistence* _persistence;

    bool _InitializeGameSession(uint n);
    bool _loadGame(uint n);

signals:
    void InitFailed();
    void GameReady(GameSessionModel*);

};

#endif // MAINWINDOWMODEL_H
