#include "mainwindow.h"

#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _width=12;
    _height=12;

    _model= new Model(_width, _height, 20);
    // ez csak a minta szerinti állapot
    _model->createDock(0, 1);
    _model->createDock(0, 3);
    _model->createDock(0, 5);
    _model->createDock(0, 7);
    _model->createRobot(11, 8);
    _model->createRobot(11, 9);
    _model->createRobot(11, 10);
    _model->createTarget(2, 11, 1);
    _model->createTarget(4, 11, 2);
    _model->createTarget(6, 11, 3);
    _model->createTarget(8, 11, 4);
    _model->tick();
    //_model->setSize(_height,_width);

    window= new QWidget;
    window->setWindowTitle("Waresim");
    window->setMinimumSize(500,500);
    _mainLayout = new QHBoxLayout(window);
    _leftsideLayout = new QVBoxLayout();
    _titleLayout = new QVBoxLayout();
    _infoLayout = new QVBoxLayout();
    _buttonLayout = new QGridLayout();
    _rightsideLayout = new QVBoxLayout();
    _gridLayout = new QGridLayout();
    buttonContainer = new QHBoxLayout();

    setupWindow();

    window->show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupWindow()
{
    QFont f( "Arial", 45, QFont::Bold);
    QLabel *title = new QLabel("Waresim");
    title->setStyleSheet("QLabel { color : gray; }");
    title->setFont(f);
    _titleLayout->setAlignment(Qt::AlignTop);
    _titleLayout->addWidget(title);
    _titleLayout->addWidget(new QLabel("Press Start or Open editor \nto start or edit the simulation"));

    _infoLayout->setAlignment(Qt::AlignLeft);
    _infoLayout->addWidget(new QLabel("itt lesznek a robot infok"));

    QPushButton *loadButton = new QPushButton("Load");
    QPushButton *saveButton = new QPushButton("Save");
    QPushButton *editorButton = new QPushButton("Open Editor");
    QPushButton *startButton = new QPushButton("Start");

    _buttonLayout->setAlignment(Qt::AlignBottom);
    _buttonLayout->addWidget(new QLabel("No data loaded"),0,0);
    _buttonLayout->addWidget(loadButton,1,0);
    _buttonLayout->addWidget(saveButton,1,1);
    _buttonLayout->addWidget(editorButton,2,0,1,2);
    _buttonLayout->addWidget(startButton,3,0,3,2);

    _leftsideLayout->addLayout(_titleLayout);
    _leftsideLayout->addLayout(_infoLayout);
    _leftsideLayout->addLayout(_buttonLayout);
    _mainLayout->addLayout(_leftsideLayout);
    drawTable();

    _rightsideLayout->addLayout(_gridLayout);
    _mainLayout->addLayout(_rightsideLayout);

     connect(editorButton,SIGNAL(clicked()),this,SLOT(editorButtonClicked()));
     connect(loadButton,SIGNAL(clicked()),this,SLOT(loadButtonClicked()));
     connect(saveButton,SIGNAL(clicked()),this,SLOT(saveButtonClicked()));
     connect(startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
}

void MainWindow::drawTable()
{
    if (_gridLayout->rowCount() != 0 || _gridLayout->columnCount() != 0) {
            while ( QLayoutItem* item = _gridLayout->takeAt( 0 ) )
                {
                    Q_ASSERT( ! item->layout() );
                    delete item->widget();
                    delete item;
                }
     }
    _gridButtons.clear();

    _gridLayout->setAlignment(Qt::AlignCenter);
    if(_height==0 || _width == 0)
    {
        _gridLayout->addWidget(new QLabel("No data loaded. Please use\n the Load,"
                                          " or Open editor buttons\n to set up the simulation."));
    }
    else
    {
        _gridButtons.clear();
        _gridButtons.resize(_height);
        for (int i = 0; i < _height; i++)
        {
            _gridButtons[i].resize(_width);
            for (int j = 0; j < _width; j++)
            {
                _gridButtons[i][j]= new QPushButton(this);
                _gridButtons[i][j]->setFixedSize(QSize(40,40));
                _gridButtons[i][j]->setEnabled(false);
                //_gridButtons[i][j]->setStyleSheet("background-color: white");
                _gridLayout->setSpacing(0);
                _gridLayout->addWidget(_gridButtons[i][j], i, j);

                //_gridButtons[i][j]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

            }
        }
    }
    //ennek nem itt kene lennie, de meg nem talaltam neki jobb helyet
    on_refreshTable();
}

void MainWindow::on_refreshTable()
{
    QVector<QVector<WTile*>> warehouse = _model->getWarehouse();
    for(int i = 0; i < _width; i++)
    {
        for(int j = 0; j < _height; j++)
        {
            if(_model->getRobot(i, j) != nullptr)
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: gray; }"); //polc
            }
            /*if(_model->getBoard()[i][j] == 3)
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: yellow; }"); //robot
            }*/
            if (warehouse[i][j]->isEmptyTile())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
            }
            else if (warehouse[i][j]->isDock())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: blue; }");
            }
            else if (warehouse[i][j]->isTarget())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: green; }");
            }
        }
    }
}

void MainWindow::editorButtonClicked()
{
    _editor = new Editor();

    //itt valamit a modellel

    drawTable();
    on_refreshTable();

}

void MainWindow::loadButtonClicked()
{


}

void MainWindow::saveButtonClicked()
{


}

void MainWindow::startButtonClicked()
{


}
