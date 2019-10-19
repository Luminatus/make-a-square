#include "Headers/mainwindow_view.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

MainWindowView::MainWindowView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    mainWindowModel(new MainWindowModel)
{
    ui->setupUi(this);

    _sizes = { Small, Medium, Large };

    ui->SizePicker->addItem(formatGameSize(Small));
    ui->SizePicker->addItem(formatGameSize(Medium));
    ui->SizePicker->addItem(formatGameSize(Large));

    foreach(SaveGameInfo info,  mainWindowModel->GetMaps())
    {
        ui->SavePicker->addItem(formatGameLabel(info));
    }
    if(!mainWindowModel->GetMaps().length())
    {
        ui->LoadGameButton->setEnabled(false);
    }

    ui->SizePicker->setFocus();

    connect(ui->quitButton,SIGNAL(clicked()),QApplication::instance(),SLOT(quit()));
    connect(ui->NewGameButton,SIGNAL(clicked()),this,SLOT(newGameButtonClicked()));
    connect(ui->LoadGameButton,SIGNAL(clicked()),this,SLOT(loadGameButtonClicked()));
}

void MainWindowView::exitGame()
{
    delete ui;
    delete mainWindowModel;
    QApplication::instance()->quit();
}

void MainWindowView::newGameButtonClicked()
{

    connect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    connect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
    int index = ui->SizePicker->currentIndex();
    ui->NewGameButton->setEnabled(false);
    ui->NewGameButton->setStyleSheet(ui->NewGameButton->styleSheet());
    int size = _sizes[index];
    mainWindowModel->NewGame(size);
}

void MainWindowView::loadGameButtonClicked()
{

    connect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    connect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
    int index = ui->SavePicker->currentIndex();
    ui->LoadGameButton->setEnabled(false);
    ui->LoadGameButton->setStyleSheet(ui->LoadGameButton->styleSheet());
    mainWindowModel->LoadGame(index);
}

void MainWindowView::gameInitFailed()
{
    QMessageBox* message = new QMessageBox;
    message->setText("Init failed!");
    message->show();
    ui->NewGameButton->setEnabled(true);
    ui->NewGameButton->setStyleSheet(ui->NewGameButton->styleSheet());
    ui->LoadGameButton->setEnabled(true);
    ui->LoadGameButton->setStyleSheet(ui->LoadGameButton->styleSheet());
    disconnect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    disconnect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
}

void MainWindowView::initializeGameView(GameSessionModel* gameSession)
{
    disconnect(mainWindowModel,SIGNAL(InitFailed()),this,SLOT(gameInitFailed()));
    disconnect(mainWindowModel,SIGNAL(GameReady(GameSessionModel*)),this,SLOT(initializeGameView(GameSessionModel*)));
    gameSessionView=new GameSessionView(this,gameSession);
    gameSessionView->start();
    connect(gameSessionView,SIGNAL(destroyed()),this,SLOT(returnToMain()));
    close();
}

void MainWindowView::returnToMain()
{
    disconnect(gameSessionView,SIGNAL(destroyed()),this,SLOT(returnToMain()));
    ui->NewGameButton->setEnabled(true);
    ui->NewGameButton->setStyleSheet(ui->NewGameButton->styleSheet());
    ui->LoadGameButton->setEnabled(true);
    ui->LoadGameButton->setStyleSheet(ui->LoadGameButton->styleSheet());
    ui->SizePicker->setCurrentIndex(0);
    ui->SizePicker->setFocus();    
    show();

    ui->SavePicker->clear();
    foreach(SaveGameInfo info, mainWindowModel->GetMaps(true))
    {
        ui->SavePicker->addItem(formatGameLabel(info));
    }
    if(!mainWindowModel->GetMaps().length())
    {
        ui->LoadGameButton->setEnabled(false);
    }
}

QString MainWindowView::formatGameLabel(SaveGameInfo info)
{
    QString name = info.FileName;
    int size =  info.TableSize;
    int turns = info.TurnCount;
    return QString("%1 (%2x%2) - turn %3").arg(name,QString::number(size+1),QString::number(turns));
}

QString MainWindowView::formatGameSize(int n)
{
   return QString("%1x%1").arg(QString::number(n));
}
