#include "Headers/viewdata.h"


ViewData::ViewData(GameSessionModel *model) : _gameSessionModel(model), tableSize(model->GetTableSize()){}

Player ViewData::GetOwner(int row, int col)
{
    return _gameSessionModel->GetOwner(row,col);
}

Player ViewData::GetLine(int row, int col, Direction dir)
{
    return _gameSessionModel->GetLine(row,col, dir);
}

GameStats ViewData::GetGameStats()
{
    return _gameSessionModel->GetGameStats();
}

