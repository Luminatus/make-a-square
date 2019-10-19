#ifndef TABLEVIEW_H
#define TABLEVIEW_H
#include <QWidget>
#include <QPainter>
#include "Headers/viewdata.h"
#include <QMouseEvent>

class TableView: public QWidget
{
    Q_OBJECT
public:
    TableView(QWidget* parent=0);
    ~TableView();
    void setViewData(ViewData* viewData);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    bool eventFilter(QObject *, QEvent *);
private:
    QColor getColorForPlayer(Player);
    ViewData* _viewData;
    QColor _redColor;
    QColor _blueColor;
    QColor _noneColor;
    QPair<Position,Direction> _currentHighlight;
signals:
    void LineClicked(int,int,Direction);
    void HighlightChanged();
};

#endif // TABLEVIEW_H
