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
    connect(_model, SIGNAL(onTick()), this, SLOT(onTick()));
    // ez csak a minta szerinti állapot
    _model->createDock(0, 1);
    _model->createDock(0, 3);
    _model->createDock(0, 5);
    _model->createDock(0, 7);
    _model->createRobot(11, 8);
    _model->createRobot(11, 9);
    _model->createRobot(11, 10);
    _model->createPod(2, 1, {});
    _model->createPod(2, 2, {});
    _model->createPod(2, 3, {1});
    _model->createPod(2, 4, {});
    _model->createPod(2, 5, {2});
    _model->createPod(2, 6, {2});
    _model->createPod(2, 7, {});
    _model->createPod(3, 1, {});
    _model->createPod(3, 2, {1, 3, 4});
    _model->createPod(3, 3, {});
    _model->createPod(3, 4, {1});
    _model->createPod(3, 5, {});
    _model->createPod(3, 6, {});
    _model->createPod(3, 7, {});
    _model->createPod(5, 1, {});
    _model->createPod(5, 2, {2, 4});
    _model->createPod(5, 3, {4});
    _model->createPod(5, 4, {});
    _model->createPod(5, 5, {});
    _model->createPod(5, 6, {});
    _model->createPod(5, 7, {});
    _model->createPod(6, 1, {});
    _model->createPod(6, 2, {});
    _model->createPod(6, 3, {});
    _model->createPod(6, 4, {});
    _model->createPod(6, 5, {});
    _model->createPod(6, 6, {});
    _model->createPod(6, 7, {});
    _model->createPod(8, 1, {1});
    _model->createPod(8, 2, {});
    _model->createPod(8, 3, {});
    _model->createPod(8, 4, {4});
    _model->createPod(8, 5, {4});
    _model->createPod(8, 6, {});
    _model->createPod(8, 7, {});
    _model->createPod(9, 1, {3});
    _model->createPod(9, 2, {3});
    _model->createPod(9, 3, {1});
    _model->createPod(9, 4, {3});
    _model->createPod(9, 5, {});
    _model->createPod(9, 6, {});
    _model->createPod(9, 7, {});
    _model->createTarget(2, 11, 1);
    _model->createTarget(4, 11, 2);
    _model->createTarget(6, 11, 3);
    _model->createTarget(8, 11, 4);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), _model, SLOT(tick()));

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
    //_infoLayout->addWidget(new QLabel("itt lesznek a robot infok"));

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
    refreshTable();
}

void MainWindow::refreshTable()
{
    QVector<QVector<WTile*>> warehouse = _model->getWarehouse();
    for(int i = 0; i < _width; i++)
    {
        for(int j = 0; j < _height; j++)
        {
            _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
            _gridButtons[i][j]->setText("");
            if(_model->getRobot(i, j) != nullptr)
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(255, 192, 0); }");
            }
            if(_model->getPod(i, j) != nullptr)
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(230, 230, 230); color: black; }");
                QString podText ="P\n";
                foreach (const int &value, _model->getPod(i, j)->getProducts())
                {
                    podText += QString::number(value) + " ";
                }
                _gridButtons[i][j]->setText(podText);
            }
            if (warehouse[i][j]->isEmptyTile())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
            }
            else if (warehouse[i][j]->isDock())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(91, 155, 213); }");
            }
            else if (warehouse[i][j]->isTarget())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(146, 208, 80); color: black; }");
                _gridButtons[i][j]->setText(QString::number(warehouse[i][j]->getTarget()));
            }
        }
    }
}

void MainWindow::editorButtonClicked()
{
    timer->stop();
    _editor = new Editor();
    connect(_editor, SIGNAL(applyAndClose()), this, SLOT(editorApplyAndClose()));
}

void MainWindow::loadButtonClicked()
{

}

void MainWindow::saveButtonClicked()
{

}

void MainWindow::startButtonClicked()
{
    //timer->start(1000);
}

void MainWindow::editorApplyAndClose()
{
    _model->setSize(_editor->getWidth(), _editor->getHeight());
    _width = _editor->getWidth();
    _height = _editor->getHeight();
    QVector<QPoint> robots = _editor->getRobots();
    for (int i = 0; i < robots.count(); i++)
    {
        _model->createRobot(robots[i].x(), robots[i].y());
    }
    QVector<QPair<QPoint, QSet<int>>> pods = _editor->getPods();
    qDebug() << pods;
    for (int i = 0; i < pods.count(); i++)
    {
        _model->createPod(pods[i].first.x(), pods[i].first.y(), pods[i].second);
    }
    QVector<QPair<QPoint, int>> targets = _editor->getTargets();
    for (int i = 0; i < targets.count(); i++)
    {
        _model->createTarget(targets[i].first.x(), targets[i].first.y(), targets[i].second);
    }
    QVector<QPoint> docks = _editor->getDocks();
    for (int i = 0; i < docks.count(); i++)
    {
        _model->createDock(docks[i].x(), docks[i].y());
    }
    drawTable();
    refreshTable();
}

void MainWindow::onTick()
{
    refreshTable();
}
