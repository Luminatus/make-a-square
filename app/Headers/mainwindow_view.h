#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <ui_mainwindow.h>
#include "Headers/mainwindow_model.h"
#include "Headers/gamesession_view.h"
#include <QString>


class MainWindowView : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindowView(QWidget *parent = 0);

private:
    QVector<GameSize> _sizes;
    Ui::MainWindow* ui;    
    MainWindowModel* mainWindowModel;
    GameSessionView* gameSessionView;
    QString formatGameSize(int n);
    QString formatGameLabel(SaveGameInfo info);


public slots:
    void newGameButtonClicked();
    void loadGameButtonClicked();
    void gameInitFailed();
    void initializeGameView(GameSessionModel* gameSession);
    void returnToMain();
    void exitGame();
};

#endif // MAINWINDOW_H
