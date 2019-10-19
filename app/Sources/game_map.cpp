#include "Headers/game_map.h"

void GameField::set(Direction dir, Player p)
{
    switch(dir)
    {
        case Up:
            if(UpField == NULL) return;
            *(UpField) = p;
        break;
        case Down:
            if(DownField == NULL) return;
            *(DownField) = p;
        break;
        case Right:
            if(RightField == NULL) return;
            *(RightField) = p;
        break;
        case Left:
            if(LeftField == NULL) return;
            *(LeftField) = p;
        break;
    }
}

Player GameField::get(Direction dir)
{
    switch(dir)
    {
        case Up:
            return UpField != NULL ? *(UpField) : None;
        case Down:
            return DownField != NULL ?  *(DownField) : None;
        case Right:
            return RightField != NULL ?  *(RightField) : None;
        case Left:
            return LeftField != NULL ?  *(LeftField) : None;
    }
}

GameMap::GameMap(int size)
{
    _map = new QVector<QVector<GameField>>(size);
    for(int i=0; i< size; i++)
    {
        (*_map)[i].resize(size);
        for(int j=0; j< size; j++)
        {
            Player* val;
            if(i==0)
            {
                val = new Player();
                *val = None;
                (*_map)[i][j].UpField = val;
            }
            else
            {
                val = (*_map)[i-1][j].DownField;
                (*_map)[i][j].UpField = val;
            }

            if(j==0)
            {
                val = new Player();
                *val = None;
                (*_map)[i][j].LeftField = val;
            }
            else
            {
                val = (*_map)[i][j-1].RightField;
                (*_map)[i][j].LeftField = val;
            }

            val = new Player();
            *val = None;
            (*_map)[i][j].DownField = val;

            val = new Player();
            *val = None;
            (*_map)[i][j].RightField = val;
        }
    }
}

bool GameMap::setLine(int row, int col, Direction dir, Player p)
{
    bool isValid = isValidIndex(row,col);
    if(isValid)
    {
        (*_map)[row][col].set(dir,p);
    }

    return isValid;
}

Player GameMap::getOwner(int row, int col)
{
    if(isValidIndex(row,col))
    {
        return (*_map)[row][col].Owner;
    }
    else
    {
        return None;
    }
}

bool GameMap::setOwner(int row, int col, Player p)
{
    if(isValidIndex(row,col))
    {
        (*_map)[row][col].Owner = p;
        return true;
    }

    return false;
}

Player GameMap::getLine(int row, int col, Direction dir)
{
    if(isValidIndex(row,col))
    {
        return (*_map)[row][col].get(dir);
    }
}

bool GameMap::isValidIndex(int row, int col)
{
    return row >= 0 && row < getSize() && col >= 0 && col < getSize();
}

int GameMap::getSize()
{
    return _map->length();
}


bool GameMap::isFilled(int row, int col)
{
    bool isValid = true;
    QVector<Direction> directions = {Up, Down, Left, Right};
    for(int i=0; i<directions.length() && isValid; i++)
    {
       if(getLine(row,col,directions[i]) == Player::None)
           isValid = false;
    }

    return isValid;
}


