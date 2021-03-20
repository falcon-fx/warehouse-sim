#include "mainwindow.h"

#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    _height=5;
    _width=15;

    window= new QWidget;
    window->setWindowTitle("Waresim");
    window->setMinimumSize(500,500);
    _mainLayout = new QHBoxLayout(window);
    _sideLayout = new QVBoxLayout();
    _titleLayout = new QVBoxLayout();
    _infoLayout = new QVBoxLayout();
    _buttonLayout = new QGridLayout();
    test = new QVBoxLayout();
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
    QLabel *asd = new QLabel("Waresim");
    asd->setStyleSheet("QLabel { color : gray; }");
    asd->setFont(f);
    _titleLayout->setAlignment(Qt::AlignTop);
    _titleLayout->addWidget(asd);
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

    _sideLayout->addLayout(_titleLayout);
    _sideLayout->addLayout(_infoLayout);
    _sideLayout->addLayout(_buttonLayout);
    _mainLayout->addLayout(_sideLayout);
    drawTable();

    test->addLayout(_gridLayout);
    _mainLayout->addLayout(test);
}

void MainWindow::drawTable()
{
    //TODO: gombok torlese, ha uj szimulaciot toltunk be

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
}


