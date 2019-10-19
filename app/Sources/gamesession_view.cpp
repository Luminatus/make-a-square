#include "Headers/gamesession_view.h"
#include <QPainter>
#include <QPaintEngine>
#include <QPaintEvent>
#include <QMessageBox>
#include <QTimer>

GameSessionView::GameSessionView(QWidget* mainWindowView, GameSessionModel* gameSessionModel):
    _mainWindowView(mainWindowView),
    _gameSessionModel(gameSessionModel),
    ui(new Ui::GameSessionUI){}

void GameSessionView::start()
{
    _viewData = new ViewData(_gameSessionModel);
    _viewData->tileSize = (576 / _gameSessionModel->GetTableSize());
    if(_viewData->tileSize > 60)
        _viewData->tileSize = 60;

    ui->setupUi(this);
    ui->gameTable->setMinimumSize(_gameSessionModel->GetTableSize()*_viewData->tileSize,_gameSessionModel->GetTableSize()*_viewData->tileSize);
    int minwidth = ui->gameTable->width()+180;
    setMinimumSize(minwidth,ui->gameTable->height()+150);
    setMaximumSize(minwidth,ui->gameTable->height()+150);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    ui->gameTable->setMouseTracking(true);
    init();
    show();
}

GameSessionView::~GameSessionView()
{
    disconnect(ui->closeButton,SIGNAL(clicked()),this,SLOT(backToMainMenu()));
    disconnect(ui->gameTable,SIGNAL(LineClicked(int,int,Direction)),this,SLOT(processMove(int,int,Direction)));
    disconnect(ui->gameTable,SIGNAL(HighlightChanged()),this,SLOT(processHighlight()));
    disconnect(_gameSessionModel,SIGNAL(GameWon(Player)),this,SLOT(gameFinished(Player)));

    delete _viewData;
    delete _gameSessionModel;
    delete ui;
}

void GameSessionView::init()
{
    _gameSessionModel->init();
    GameStats stats = _gameSessionModel->GetGameStats();
    ui->gameTable->setViewData(_viewData);
    ui->stepCounter->display(stats.StepCount);
    ui->bluePoints->display(stats.BluePoints);
    ui->redPoints->display(stats.RedPoints);
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(backToMainMenu()));
    connect(ui->gameTable,SIGNAL(LineClicked(int,int,Direction)),this,SLOT(processMove(int,int,Direction)));
    connect(ui->gameTable,SIGNAL(HighlightChanged()),this,SLOT(processHighlight()));
    connect(_gameSessionModel,SIGNAL(GameWon(Player)),this,SLOT(gameFinished(Player)));
    this->setFocus();
}


void GameSessionView::backToMainMenu()
{
    if(!_gameSessionModel->IsGameWon())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Kilépés","Biztosan ki akarsz lépni?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes)
        {
            return;
        }
        QMessageBox::StandardButton save_reply;
        save_reply = QMessageBox::question(this,"Játék mentése","Szeretnéd elmenteni a jelenlegi játékállást?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (save_reply == QMessageBox::Yes)
        {
            bool success = _gameSessionModel->saveGame();
            if(!success)
            {
                QMessageBox::information(this,"Mentési hiba","Játék mentése sikertelen, a játék nem került mentésre.");
            }
        }
    }
    close();
    delete this;
}

void GameSessionView::updateWindow()
{
    GameStats stat = _gameSessionModel->GetGameStats();
    ui->stepCounter->display(stat.StepCount);
    ui->bluePoints->display(stat.BluePoints);
    ui->redPoints->display(stat.RedPoints);
    update();
}

void GameSessionView::processMove(int posX, int posY, Direction dir)
{
    if(_gameSessionModel->MakeMove(posX, posY, dir))
    {
        updateWindow();
    }
}

void GameSessionView::gameFinished(Player winner)
{
        GameStats gameStats = _gameSessionModel->GetGameStats();
        QMessageBox* message = new QMessageBox();
        bool isWon = false;
        QString winnerName;
        switch(winner)
        {
            case RedPlayer: isWon = true; winnerName = "Vörös"; break;
            case BluePlayer: isWon = true; winnerName = "Kék"; break;
            default: break;
        }

        if(isWon)
        {
            message->setText(QString("%1 Nyert! A játék %2 körön át tartott").arg(winnerName).arg(gameStats.StepCount));
        }
        else
        {
            message->setText("A játék döntetlennel végződött!");
        }        

        message->show();
        connect(message,SIGNAL(finished(int)),SLOT(backToMainMenu()));
}

void GameSessionView::processHighlight()
{
    qDebug("update called");
    update();
}
