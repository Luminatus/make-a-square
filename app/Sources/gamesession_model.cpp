#include "Headers/gamesession_model.h"
#include <QLinkedList>


GameSessionModel::GameSessionModel(GamePersistence* p)
{
    _persistence = p;
}

GameSessionModel::~GameSessionModel(){}

void GameSessionModel::init()
{
    if(_gameInitialized)
    {
        endGame();
    }

    _gameInitialized = true;
}

bool GameSessionModel::newGame(uint size)
{
    if(size < 2)
    {
        return false;
    }

    size--;

    if(_gameInitialized)
    {
        endGame();
    }

    _gameMap = new GameMap(size);

    SaveGameInfo info;

    info.TableSize = size;
    info.TurnCount = 0;

    _info = info;

    _currentPlayer = RedPlayer;

    return true;

}

bool GameSessionModel::loadGame(uint n)
{
    if(_gameInitialized)
    {
        endGame();
    }

    if(n < _persistence->getGameList().length())
    {
        _info = _persistence->getGameList()[n];

        int stepCount = 0;
        Player currentPlayer = RedPlayer;

        _gameMap = new GameMap(_info.TableSize);

        bool isValid = true;

        foreach(LogEntry entry,  _info.Log)
        {
            if(
                _gameMap->isValidIndex(entry.Row,entry.Column) &&
                _gameMap->getLine(entry.Row, entry.Column, entry.Dir) == Player::None
            )
            {
                _gameMap->setLine(entry.Row,entry.Column,entry.Dir,currentPlayer);
                bool isFilled = false;
                if(_gameMap->isFilled(entry.Row,entry.Column))
                {
                    isFilled = true;
                    _gameMap->setOwner(entry.Row,entry.Column,currentPlayer);
                }
                int row2,col2;
                switch(entry.Dir)
                {
                    case Up: row2 = entry.Row-1; col2 = entry.Column; break;
                    case Down: row2 = entry.Row+1; col2 = entry.Column; break;
                    case Left: row2 = entry.Row; col2 = entry.Column-1; break;
                    case Right: row2 = entry.Row; col2 = entry.Column+1; break;
                }
                if(_gameMap->isFilled(row2,col2))
                {
                    isFilled = true;
                    _gameMap->setOwner(row2,col2,currentPlayer);
                }

                if(!isFilled)
                    currentPlayer = currentPlayer == RedPlayer ? BluePlayer : RedPlayer;
                stepCount++;
            }
            else
            {
                isValid = false;
                break;
            }
        }

        if(!isValid)
        {
            delete _gameMap;
            return false;
        }

        _currentPlayer = currentPlayer;
        _info.TurnCount = stepCount;

        return true;
    }
    else
    {
        return false;
    }
}

void GameSessionModel::endGame()
{
    if(_gameMap != NULL)
    {
        delete _gameMap;
        _gameMap = NULL;
    }
}

bool GameSessionModel::saveGame()
{
    return _persistence->saveGame(_info);
}

int GameSessionModel::GetTableSize()
{
    return _gameMap->getSize();
}


bool GameSessionModel::IsGameWon()
{
    bool found = false;
    QVector<Direction> directions = {Up, Down, Left, Right};
    for(int i=0; i<_gameMap->getSize() && !found; i++)
    {
        for(int j=0; j<_gameMap->getSize() && !found; j++)
        {
            for(int d=0; d<directions.length() && !found; d++)
            {
                if(_gameMap->getLine(i,j,directions[d]) == Player::None)
                {
                    found = true;
                }
            }
        }
    }
    return !found;
}

Player GameSessionModel::GetLine(int row, int col, Direction dir)
{
    return _gameMap->getLine(row,col,dir);
}

Player GameSessionModel::GetOwner(int row, int col)
{
    return _gameMap->getOwner(row,col);
}

Player GameSessionModel::GetOwner(Position pos)
{
    return GetOwner(pos.Y,pos.X);
}

Player GameSessionModel::GetLine(Position pos, Direction dir)
{
    return GetLine(pos.Y,pos.X,dir);
}

bool GameSessionModel::isValidMove(int x, int y, Direction dir)
{
     return _gameMap->isValidIndex(y,x) && _gameMap->getLine(y,x, dir) == Player::None;
}

bool GameSessionModel::MakeMove(int x, int y, Direction dir)
{
    if(!IsGameWon() && isValidMove(x,y, dir))
    {
        _gameMap->setLine(y,x,dir,_currentPlayer);

        //check if surrounding squares are filled
        bool filledSquare = false;
        QVector<Position> positions(2);
        positions[0] = Position(x,y);
        switch(dir)
        {
            case Up: positions[1] = Position(x,y-1); break;
            case Down: positions[1] = Position(x,y+1); break;
            case Left: positions[1] = Position(x-1,y); break;
            case Right: positions[1] = Position(x+1,y); break;
        }

        foreach(Position pos, positions)
        {
            if(isFilled(pos.Y, pos.X))
            {
                _gameMap->setOwner(pos.Y,pos.X, _currentPlayer);
                filledSquare = true;
            }
        }

        if(!filledSquare)
            _currentPlayer = _currentPlayer == RedPlayer ? BluePlayer : RedPlayer;

        _info.Log.append(LogEntry(y,x,dir));
        _info.TurnCount++;

        if(IsGameWon())
        {
            GameStats stats = GetGameStats();

            if(stats.BluePoints == stats.RedPoints)
                emit GameWon(Player::None);
            else
            {
                if(stats.BluePoints > stats.RedPoints)
                {
                    emit GameWon(BluePlayer);
                }
                else
                {
                    emit GameWon(RedPlayer);
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool GameSessionModel::isFilled(int row, int col)
{
    bool isValid = true;
    QVector<Direction> directions = {Up, Down, Left, Right};
    for(int i=0; i<directions.length() && isValid; i++)
    {
       if(_gameMap->getLine(row,col,directions[i]) == Player::None)
           isValid = false;
    }

    return isValid;
}

GameStats GameSessionModel::GetGameStats()
{
    int bluePoints = 0, redPoints = 0;

    for(int i=0; i< _gameMap->getSize(); i++)
    {
        for(int j=0; j< _gameMap->getSize(); j++)
        {
            Player owner = _gameMap->getOwner(i,j);
            switch(owner)
            {
                case RedPlayer: redPoints++; break;
                case BluePlayer: bluePoints++; break;
            }
        }
    }

    return GameStats(_info.TurnCount, _currentPlayer, bluePoints, redPoints);
}
