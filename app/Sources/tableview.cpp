#include "Headers/tableview.h"
#include <QtMath>
#include <QDebug>

TableView::TableView(QWidget* parent): QWidget(parent)
{    
    _redColor.setRgb(180,0,0);
    _blueColor.setRgb(0,0,180);
    _noneColor.setRgb(200,200,200);

    this->installEventFilter(this);
}

QColor TableView::getColorForPlayer(Player p)
{
    switch(p)
    {
        case RedPlayer: return _redColor;
        case BluePlayer: return _blueColor;
        default: return _noneColor;
    }
}


void TableView:: setViewData(ViewData* viewData)
{
   _viewData = viewData;
    int tableSize = _viewData->tableSize;
    int tileSize = _viewData->tileSize;
   setMinimumSize(tableSize*tileSize+30,tableSize*tileSize+30);
   setBaseSize(tableSize*tileSize,tableSize*tileSize);
}


void TableView::paintEvent(QPaintEvent*)
{
    int tableSize = _viewData->tableSize;
    int tileSize = _viewData->tileSize;

    setMinimumSize(tableSize*tileSize,tableSize*tileSize);
    setBaseSize(tableSize*tileSize,tableSize*tileSize);

    QPainter painter(this);

    QPen tilePen;
    tilePen.setWidth(1);
    tilePen.setColor(QColor(30,40,40));

    painter.setPen(tilePen);

    QColor bgColor;
    bgColor.setRgb(255,255,255);

    QColor activeColor;
    activeColor.setRgb(50,60,85,100);


    painter.fillRect(0,0,tableSize*tileSize+20,tableSize*tileSize+20,bgColor);

    for(int i=0; i<tableSize; i++)
    {
        for(int j=0; j< tableSize; j++)
        {
            Player value = _viewData->GetOwner(i,j);
            QColor fillColor = getColorForPlayer(value);
            fillColor.setAlpha(128);

            painter.fillRect(j*tileSize+10, i*tileSize+10, tileSize,tileSize, fillColor);

            value = _viewData->GetLine(i,j,Up);
            painter.fillRect(j*tileSize -1+10,i* tileSize -1+10,tileSize+2, 2,getColorForPlayer(value));

            value = _viewData->GetLine(i,j,Down);
            painter.fillRect(j*tileSize -1+10, (i+1)* tileSize -1+10, tileSize + 2, 2,getColorForPlayer(value));

            value = _viewData->GetLine(i,j,Left);
            painter.fillRect(j*tileSize -1+10,i* tileSize -1+10, 2, tileSize + 2,getColorForPlayer(value));

            value = _viewData->GetLine(i,j,Right);
            painter.fillRect((j+1)*tileSize -1+10,i* tileSize -1+10, 2, tileSize + 2,getColorForPlayer(value));
        }
    }

    Position highlightPos = _currentHighlight.first;
    if(
            highlightPos.X >=0 &&
            highlightPos.Y >= 0 &&
            highlightPos.X < tableSize &&
            highlightPos.Y < tableSize)
    {
        qDebug()<<"Drawing into"<<highlightPos.X<<","<<highlightPos.Y<<_currentHighlight.second<<"\n";
        int x = highlightPos.X * tileSize -2;
        int y = highlightPos.Y * tileSize -2;
        int w,h;
        switch(_currentHighlight.second)
        {
            case Up: w = tileSize+4; h = 4; break;
            case Down: y+=tileSize; w = tileSize+4; h = 4; break;
            case Left: w = 4; h = tileSize+4; break;
            case Right: x+=tileSize; w = 4; h = tileSize+4; break;
        }


        QColor highlightColor = getColorForPlayer(_viewData->GetGameStats().CurrentPlayer);
        highlightColor.setAlpha(255);
        painter.fillRect(x+10,y+10,w,h,highlightColor);
    }

    for(int i=0; i< tableSize+1; i++)
    {
        for(int j=0; j< tableSize+1; j++)
        {
            painter.drawEllipse(QPoint(i*tileSize+10, j*tileSize+10),4,4);
            auto path = QPainterPath();
            path.addEllipse(QPoint(i*tileSize+10, j*tileSize+10),4,4);
            painter.fillPath(path,QColor("black"));
        }
    }
}

void TableView::mousePressEvent(QMouseEvent* event)
{
    int tileSize = _viewData->tileSize;
    int posX = (event->x()-10) / tileSize;
    int posY = (event->y()-10) / tileSize;

    int y_offset = ((event->y()-10) % _viewData->tileSize) - tileSize/2;
    int x_offset = ((event->x()-10) % _viewData->tileSize) - tileSize/2;

    if(abs(y_offset) > abs(x_offset))
    {
        if(abs(y_offset) > tileSize/6.0)
        {
            Direction dir = y_offset > 0 ? Down : Up;
            emit LineClicked(posX,posY,dir);
        }
    }
    else
    {
        if(abs(x_offset) > tileSize/6.0)
        {
            Direction dir = x_offset > 0 ? Right : Left;
            emit LineClicked(posX,posY,dir);
        }
    }
}

void TableView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<event->x()<<", "<<event->y()<<"\n";
    int tileSize = _viewData->tileSize;
    int posX = (event->x()-10) / tileSize;
    int posY = (event->y()-10) / tileSize;

    int y_offset = ((event->y()-10) % _viewData->tileSize) - tileSize/2;
    int x_offset = ((event->x()-10) % _viewData->tileSize) - tileSize/2;

    bool isValid = false;
    Direction dir;

    if(abs(y_offset) > abs(x_offset))
    {
        if(abs(y_offset) > tileSize/6.0)
        {
            dir = y_offset > 0 ? Down : Up;
            isValid = true;
        }
    }
    else
    {
        if(abs(x_offset) > tileSize/6.0)
        {
            dir = x_offset > 0 ? Right : Left;
            isValid = true;
        }
    }

    Position newPos = isValid
            ? Position(posX,posY)
            : Position(-1,-1);

    if(
        _currentHighlight.first.X != posX ||
        _currentHighlight.first.Y != posY ||
        _currentHighlight.second != dir)
    {
        _currentHighlight.first = newPos;
        _currentHighlight.second = dir;
        qDebug("Highlight changed");
        emit HighlightChanged();
    }

}

bool TableView::eventFilter(QObject* object, QEvent* event)
{
    if(event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mev=static_cast<QMouseEvent*>(event);
        mouseMoveEvent(mev);

        return true;
    }
   return false;
}

TableView::~TableView()
{
}
