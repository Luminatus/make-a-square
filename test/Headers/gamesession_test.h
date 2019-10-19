#ifndef GAMESESSION_TEST_H
#define GAMESESSION_TEST_H

#include "Headers/gamesession_model.h"
#include "Headers/datastructures.h"
#include "Headers/game_map.h"
#include "game_persistence_mock.h"

class GameSessionModelTest : public QObject
{
    Q_OBJECT

private:
    GameSessionModel* _model;
    GamePersistence* _persistence;

private slots:
    void initTestCase()
    {
        _persistence = new GamePersistenceMock();
        _model = new GameSessionModel(_persistence);
    }

    void cleanupTestCase()
    {
        delete _model;
        delete _persistence;
    }

    void init()
    {
        _model->newGame(5);
        qDebug("Called before each test");

    }

    void testValidSteps();
    void testInvalidStep();
    void testScoring();
    void TestWinning();

    void TestLoadGame();
    void TestSaveGame();

};

#endif // GAMESESSION_TEST_H
