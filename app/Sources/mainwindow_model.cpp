#include "Headers/mainwindow_model.h"
#include "Headers/datastructures.h"
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QRegExp>
#include <QDebug>

MainWindowModel::MainWindowModel()
{
    _persistence = new GamePersistence();
}

MainWindowModel::~MainWindowModel()
{
    if(_gameSessionModel != NULL)
    {
        delete _gameSessionModel;
        delete _persistence;
    }
}

void MainWindowModel::NewGame(uint n)
{
    bool ret = _InitializeGameSession(n);

    if(!ret) emit InitFailed();
    else emit GameReady(_gameSessionModel);
}

void MainWindowModel::LoadGame(uint n)
{
    bool ret = _loadGame(n);

    if(!ret) emit InitFailed();
    else emit GameReady(_gameSessionModel);
}


bool MainWindowModel::_loadGame(uint n)
{
    if(n < _persistence->getGameList().length())
    {
        _gameSessionModel = new GameSessionModel(_persistence);
        bool ret = _gameSessionModel->loadGame(n);

        return ret;
    }
    return false;
}



QVector<SaveGameInfo> MainWindowModel::GetMaps(bool forceReload)
{
    return _persistence->getGameList(forceReload);
}

bool MainWindowModel::_InitializeGameSession(uint n)
{
    _gameSessionModel = new GameSessionModel(_persistence);
    bool ret = _gameSessionModel->newGame(n);

    return ret;
}

