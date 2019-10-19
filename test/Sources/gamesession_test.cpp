#include "Headers/gamesession_test.h"
#include <QtTest>
#include <sstream>

void GameSessionModelTest::testValidSteps()
{
    _model->newGame(5);
    GameStats stat = _model->GetGameStats();
    QVERIFY2(stat.BluePoints == 0 &&
             stat.RedPoints == 0 &&
             stat.StepCount == 0 &&
             stat.CurrentPlayer == RedPlayer
             ,
             "Initial values are invalid");

    QVERIFY(_model->GetLine(1,1,Up) == None);
    _model->MakeMove(1,1,Up);
    QVERIFY(_model->GetLine(1,1,Up) == RedPlayer);
    QVERIFY2(_model->GetLine(0,1,Down) == RedPlayer,
             "Taking a line should take effect on both squares that share the line");
    GameStats stat2 = _model->GetGameStats();
    QVERIFY2(stat2.StepCount == 1, "Turn counter should be increased every step");

    QVERIFY(_model->GetLine(1,1,Down) == None);
    _model->MakeMove(1,1,Down);
    QVERIFY(_model->GetLine(1,1,Down) == BluePlayer);

}

void GameSessionModelTest::testInvalidStep()
{
    _model->newGame(5);

    try{
        _model->MakeMove(-1,-1,Down);
        _model->MakeMove(-1,-1,Left);
        _model->MakeMove(-1,-1,Up);
        _model->MakeMove(-1,-1,Right);
        GameStats stats  = _model->GetGameStats();
        QVERIFY2(stats.BluePoints == 0 && stats.RedPoints == 0, "Out of bounds square should not score points");
        QVERIFY2(stats.StepCount == 0, "Out of bounds steps should not increase turn counter");
    }
    catch(std::exception& e)
    {
        QFAIL("Out of bounds move should not throw exception");
    }

    _model->MakeMove(1,1,Down);
    QVERIFY(_model->GetLine(1,1,Down) == RedPlayer);

    GameStats stats = _model->GetGameStats();
    QVERIFY(stats.CurrentPlayer == BluePlayer);

    _model->MakeMove(1,1,Down);
    QVERIFY(_model->GetLine(1,1,Down) == RedPlayer);

    GameStats stats2 = _model->GetGameStats();
    QVERIFY2(stats2.CurrentPlayer == BluePlayer,"Invalid step should not advance to the next player");
    QVERIFY2(stats2.StepCount == stats.StepCount,"Invalid step should not increase the Turn counter");
}


void GameSessionModelTest::testScoring()
{
    _model->newGame(6);

    _model->MakeMove(1,1,Down);
    _model->MakeMove(1,1,Up);
    _model->MakeMove(1,1,Left);
    _model->MakeMove(1,1,Right);

    GameStats stats = _model->GetGameStats();

    QVERIFY2(_model->GetOwner(1,1) == BluePlayer && stats.BluePoints == 1 && stats.RedPoints == 0,
            "Scoring should increase the player points, and set the square as owned by them");

    QVERIFY2(stats.CurrentPlayer == BluePlayer,"After scoring, the same player should come again");


    _model->MakeMove(3,3,Down);
    _model->MakeMove(3,3,Up);
    _model->MakeMove(3,3,Left);

    _model->MakeMove(4,3,Down);
    _model->MakeMove(4,3,Up);
    _model->MakeMove(4,3,Right);
    _model->MakeMove(4,3,Left);

    GameStats stats2 = _model->GetGameStats();
    std::stringstream ss;
    QVERIFY2(_model->GetOwner(3,3) == BluePlayer && _model->GetOwner(3,4) == BluePlayer && stats2.BluePoints == 3,
             "Double-taking should increase the player's points by 2, and set both squares as owned by them");
}

void GameSessionModelTest::TestWinning()
{
    _model->newGame(2);
    _model->MakeMove(0,0,Up);
    _model->MakeMove(0,0,Down);
    _model->MakeMove(0,0,Left);
    _model->MakeMove(0,0,Right);

    GameStats stats = _model->GetGameStats();
    QVERIFY2(_model->IsGameWon() && stats.BluePoints == 1,
             "Model should know if the game is finished");
}

void GameSessionModelTest::TestLoadGame()
{
    _model->loadGame(0);
    GameStats stats = _model->GetGameStats();
    QVERIFY2(stats.BluePoints == 0 && stats.RedPoints == 0 && stats.StepCount == 0 && stats.CurrentPlayer == RedPlayer,
             "Initial stats for loading a clean game are invalid");
    _model->loadGame(1);
    GameStats stats2 = _model->GetGameStats();
    QVERIFY2(stats2.BluePoints == 1 && stats2.RedPoints == 0 && stats2.StepCount == 4 && stats2.CurrentPlayer == BluePlayer,
             "Initial stats for loading game2 (1 score for Blue) are invalid");

    _model->loadGame(2);
    _model->MakeMove(1,2,Down);
    GameStats stats3 = _model->GetGameStats();
    QVERIFY2(stats3.BluePoints == 1 && stats3.RedPoints == 2 && stats3.StepCount == 10 && stats3.CurrentPlayer == RedPlayer,
             "Initial stats for loading game3 (near double score for blue)  are invalid");

    /*
    bool res = _model->MakeMove(1,2);
    Position playerPosition = _model->GetPlayerPosition();
    QVERIFY(!res && playerPosition.X == 0 && playerPosition.Y == 2);
    */
}

void GameSessionModelTest::TestSaveGame()
{
    _model->newGame(5);
    _model->MakeMove(1,1,Up);
    _model->MakeMove(1,1,Left);
    _model->MakeMove(1,1,Right);
    _model->MakeMove(1,1,Down);

    _model->saveGame();
}


QTEST_APPLESS_MAIN(GameSessionModelTest)

