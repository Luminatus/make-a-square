#include "Headers/game_persistence.h"
#include <QTextStream>
#include <QDir>

void GamePersistence::loadGameList()
{
    gameList.clear();
    QDir directory(SAVEGAME_FOLDER);
    QStringList maps = directory.entryList(QStringList() << "*.sav",QDir::Files);
    foreach(QString filename, maps) {
        loadGameInfoFromFile(SAVEGAME_FOLDER+"/"+filename);
    }
}

void GamePersistence::loadGameInfoFromFile(QString filename)
{
    QFileInfo fileInfo(filename);
    QString name = fileInfo.baseName();

    QFile file(filename);
    if (file.exists())
    {
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream stream(&file);
        QString map_buffer = stream.readAll().trimmed();

        QVector<QString> rows = map_buffer.split('\n').toVector();

        if(!rows.length())
            return;

        int tableSize = rows.first().toInt();

        if(tableSize <= 0)
            return;

        rows.pop_front();

        QLinkedList<LogEntry> log;

        int turnCount = 0;

        foreach(QString row, rows)
        {
            QVector<QString> row_array = row.split(' ').toVector();
            if(row_array.length() != 3)
                return;
            int pos_row = row_array[0].toInt();
            int pos_col = row_array[1].toInt();
            Direction dir = (Direction)row_array[2].toInt();

            switch(dir)
            {
                case Up:
                case Down:
                case Left:
                case Right:
                    break;
                default: return;
            }

            if(pos_row < 0 || pos_row >= tableSize || pos_col < 0 || pos_col >= tableSize)
                return;

            log.append(LogEntry(pos_row,pos_col,dir));
            turnCount++;
        }

        SaveGameInfo info = SaveGameInfo(name,tableSize,turnCount, log);

        gameList.push_back(info);
    }
}

bool GamePersistence::saveGame(SaveGameInfo info)
{
    if(info.Log.isEmpty() || info.TableSize <= 0)
    {
        return false;
    }

    if(info.FileName.isEmpty())
    {
        QDir directory(SAVEGAME_FOLDER);
        QStringList files = directory.entryList(QStringList() << "*.sav",QDir::Files);
        int id = 0;
        foreach(QString filename, files) {

            QRegExp rx("save_(\\d+)\\.sav");

            QList<int> list;
            if(rx.indexIn(filename,0) != -1)
            {
                int val = rx.cap(1).toInt();
                if(val > id)
                    id = val;
            }
        }

        id++;
        info.FileName = QString("save_%1").arg(id);
    }


    try
    {
        QFile file(SAVEGAME_FOLDER+"/"+info.FileName+".sav");
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream stream(&file);
        stream<<info.TableSize<<"\n";

        foreach(LogEntry entry, info.Log)
        {
            QString row = QString("%1 %2 %3\n").arg(QString::number(entry.Row), QString::number(entry.Column), QString::number((int)entry.Dir));
            stream<<row;
        }

        file.close();
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;

}


QVector<SaveGameInfo>& GamePersistence::getGameList(bool forceReload)
{
    if(forceReload || gameList.empty())
        loadGameList();
    return gameList;
}
