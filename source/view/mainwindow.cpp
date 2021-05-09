#include <view/mainwindow.h>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QFont>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _size = 10;
    _steps = 0;
    _model = new Model(_size, 60);
    connect(_model, SIGNAL(onTick()), this, SLOT(onTick()));
    connect(_model, SIGNAL(onLoad()), this, SLOT(onLoad()));
    connect(_model, SIGNAL(onFinished()), this, SLOT(onFinished()));
    // ez csak a minta szerinti állapot
    _model->setSize(12);
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

    window = new QWidget;
    window->setWindowTitle("Waresim");
    window->setMinimumSize(500, 500);
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

MainWindow::~MainWindow() { }

void MainWindow::setupWindow()
{
    QFont f("Arial", 45, QFont::Bold);
    QLabel* title = new QLabel("Waresim");
    title->setStyleSheet("QLabel { color : gray; }");
    title->setFont(f);
    _titleLayout->setAlignment(Qt::AlignTop);
    _titleLayout->addWidget(title);
    infoLabel = new QLabel("Press Start or Open editor \nto start or edit the simulation");
    _titleLayout->addWidget(infoLabel);
    _newOrderButton = new QPushButton("New Order");

    _infoLayout->setAlignment(Qt::AlignLeft);
    //_infoLayout->addWidget(new QLabel("itt lesznek a robot infok"));

    QPushButton *loadButton = new QPushButton("Load");
    QPushButton *saveButton = new QPushButton("Save");
    editorButton = new QPushButton("Open Editor");
    startButton = new QPushButton("Start");
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setMinimum(0);
    speedSlider->setMaximum(5000);
    speedSlider->setTickInterval(100);
    speedSlider->setValue(speedSlider->maximum() - 600);

    _buttonLayout->setAlignment(Qt::AlignBottom);
    //_buttonLayout->addWidget(new QLabel("No data loaded"),0,0);
    _buttonLayout->addWidget(_newOrderButton, 1, 0,1,2);
    _buttonLayout->addWidget(loadButton, 2, 0);
    _buttonLayout->addWidget(saveButton, 2, 1);
    _buttonLayout->addWidget(editorButton, 3, 0, 1, 2);
    _buttonLayout->addWidget(speedSlider, 4, 0, 1, 2);
    _buttonLayout->addWidget(startButton, 5, 0, 3, 2);

    _leftsideLayout->addLayout(_titleLayout);
    _leftsideLayout->addLayout(_infoLayout);
    _leftsideLayout->addLayout(_buttonLayout);
    _mainLayout->addLayout(_leftsideLayout);
    drawTable();

    _rightsideLayout->addLayout(_gridLayout);
    _mainLayout->addLayout(_rightsideLayout);

    connect(editorButton, SIGNAL(clicked()), this, SLOT(editorButtonClicked()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
    connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(speedSliderChanged(int)));
    connect(_newOrderButton, SIGNAL(clicked()), this, SLOT(newOrder()));
}

void MainWindow::drawTable()
{
    _size = _model->getSize();
    if (_gridLayout->rowCount() != 0 || _gridLayout->columnCount() != 0) {
        while (QLayoutItem* item = _gridLayout->takeAt(0)) {
            Q_ASSERT(!item->layout());
            delete item->widget();
            delete item;
        }
    }
    _gridButtons.clear();

    _gridLayout->setAlignment(Qt::AlignCenter);
    if(_size == 0)
    {
        noDataLabel = new QLabel("No data loaded. Please use\n the Load, or Open editor buttons\n to set up the simulation.");
        _gridLayout->addWidget(noDataLabel);
    }
    else
    {
        _gridButtons.clear();
        _gridButtons.resize(_size);
        for (int i = 0; i < _size; i++)
        {
            _gridButtons[i].resize(_size);
            for (int j = 0; j < _size; j++)
            {
                _gridButtons[i][j]= new QPushButton(this);
                _gridButtons[i][j]->setFixedSize(QSize(40, 40));
                _gridButtons[i][j]->setEnabled(false);
                _gridLayout->setSpacing(0);
                _gridLayout->addWidget(_gridButtons[i][j], i, j);

            }
        }
    }
    refreshTable();
}

void MainWindow::refreshTable()
{
    QVector<QVector<WTile*>> warehouse = _model->getWarehouse();
    for(int i = 0; i < _size; i++)
    {
        for(int j = 0; j < _size; j++)
        {
            _gridButtons[i][j]->setStyleSheet("background-color: #fff; border: 1px solid #adadad;");
            _gridButtons[i][j]->setText("");

            if (warehouse[i][j]->isDock())
            {
                _gridButtons[i][j]->setStyleSheet("background-color: #5b9bd5; border: 1px solid #adadad;");
            }
            else if (warehouse[i][j]->isTarget())
            {
                _gridButtons[i][j]->setStyleSheet("background-color: #92d050; color: black; border: 1px solid #adadad;");
                _gridButtons[i][j]->setText(QString::number(warehouse[i][j]->getTarget()));
            }
            if(_model->getPod(i, j) != nullptr)
            {
                _gridButtons[i][j]->setStyleSheet("background-color: #e6e6e6; color: black; border: 1px solid #adadad;");
                QString podText ="P\n";
                foreach (const int &value, _model->getPod(i, j)->getProducts())
                {
                    podText += QString::number(value) + " ";
                }
                _gridButtons[i][j]->setText(podText);
            }
            /*QVector<QVector<QPoint>> reservedPoints = _model->getReservedPoints();
            for (int k = 0; k < reservedPoints.size(); k++)
            {
                for (int l = 0; l < reservedPoints[k].size(); l++ )
                {
                    _gridButtons[reservedPoints[k][l].x()][reservedPoints[k][l].y()]->setText(QString::number(k));
                    _gridButtons[reservedPoints[k][l].x()][reservedPoints[k][l].y()]->setStyleSheet("background-color: black; color: white;");
                }
            }*/
            if(_model->getRobot(i, j) != nullptr)
            {
                switch (_model->getRobot(i, j)->getDirection())
                {
                    case Robot::Direction::NORTH:
                        _gridButtons[i][j]->setStyleSheet("background-color: #ffc000; color: black; border: 1px solid #adadad; border-top: 3px solid #cc7700;");
                        break;
                    case Robot::Direction::SOUTH:
                        _gridButtons[i][j]->setStyleSheet("background-color: #ffc000; color: black; border: 1px solid #adadad; border-bottom: 3px solid #cc7700;");
                        break;
                    case Robot::Direction::WEST:
                        _gridButtons[i][j]->setStyleSheet("background-color: #ffc000; color: black; border: 1px solid #adadad; border-left: 3px solid #cc7700;");
                        break;
                    case Robot::Direction::EAST:
                        _gridButtons[i][j]->setStyleSheet("background-color: #ffc000; color: black; border: 1px solid #adadad; border-right: 3px solid #cc7700;");
                        break;
                }
            }
        }
    }
}

void MainWindow::editorButtonClicked()
{
    timer->stop();
    _editor = new Editor(_model);
    connect(_editor, SIGNAL(applyAndClose()), this, SLOT(editorApplyAndClose()));
}

void MainWindow::loadButtonClicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load State"), "", tr("Waresim State (*.wss)"));
    _model->load(filename);
}

void MainWindow::saveButtonClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save State"), "", tr("Waresim State (*.wss)"));
    _model->save(filename);
}

void MainWindow::startButtonClicked()
{
    editorButton->setDisabled(true);
    if(timer->isActive()) {
        timer->stop();
        startButton->setText("Start");
    } else {
        timer->start(speedSlider->maximum() - speedSlider->value());
        startButton->setText("Stop");
    }
}

void MainWindow::speedSliderChanged(int value)
{
    if (timer->isActive())
    {
        timer->stop();
        timer->start(speedSlider->maximum() - value);
    }
}

void MainWindow::editorApplyAndClose()
{
    _model->setSize(_editor->getSize());
    _size = _editor->getSize();
    QVector<QPoint> robots = _editor->getRobots();
    for (int i = 0; i < robots.count(); i++)
    {
        _model->createRobot(robots[i].x(), robots[i].y());
    }
    QVector<QPair<QPoint, QSet<int>>> pods = _editor->getPods();
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
    _steps++;
    QString info;
    info += "Tasks done: " + QString::number(_model->getTasksDone()) + "\n";
    for(int i = 0; i < _model->getRobots().size(); i++) {
        Robot* r = _model->getRobots()[i];
        info += "\nRobot " + QString::number(i+1) + "\nPower: " + QString::number((int)((float)r->getPower()/(float)_model->getMaxPower()*100)) +
                "% position: x:" + QString::number(r->getPosition().x()) + ", y:" + QString::number(r->getPosition().y()) +
                "\nProduct number: " + QString::number(r->getProdNum()) + " Has pod: " + (r->hasPod() ? "true" : "false") + "\n";
        if(r->hasPod()) {
            info +=  "Pod's original position: x:" + QString::number(r->getPod()->getOriginalPosition().x()) + ", y:" + QString::number(r->getPod()->getOriginalPosition().y()) + "\n";
        }
    }
    infoLabel->setText(info);
}

void MainWindow::onLoad()
{
    drawTable();
    refreshTable();
}

void MainWindow::onFinished() {
    editorButton->setDisabled(false);
    timer->stop();
    _allEnergyUsed = 0;
    _energyUsed.clear();
    for(auto robot : _model->getRobots()) {
        _allEnergyUsed += robot->getUsedPower();
        _energyUsed.push_back(robot->getUsedPower());
    }
    QMessageBox results;
    QString res = "Number of steps:\n" + QString::number(_steps) + "\nAll energy used:\n" + QString::number(_allEnergyUsed) + "\nEnergy used by individual robots::\n";
    for(int i = 0; i < _energyUsed.size(); i++) {
        res.append("Robot #" + QString::number(i+1) + ": " + QString::number(_energyUsed[i]) + "\n");
    }
    results.setText(res);
    results.setInformativeText("Click Save to save simulation results.");
    results.setStandardButtons(QMessageBox::Save | QMessageBox::Ok);
    int ret = results.exec();

    switch (ret) {
        case QMessageBox::Save: {
            QString filename = QFileDialog::getSaveFileName(this, tr("Simulation Log"), "", tr("Text file (*.txt)"));
            _model->saveResults(filename, _energyUsed, _allEnergyUsed, _steps);
            break;
        }
        case QMessageBox::Ok:
            break;
        default:
            break;
    }
    _steps = 0;
}

void MainWindow:: newOrder()
{
    timer->stop();
    startButton->setText("Start");
    orderWindow = new QWidget;
    orderWindow->setWindowTitle("New Order");

    QGridLayout* sizeLayout = new QGridLayout(orderWindow);
    _s = new QLineEdit();
    QPushButton* confrimButton = new QPushButton("Confirm");
    QPushButton* closeButton = new QPushButton("Close");

    sizeLayout->addWidget(new QLabel("Order number:"),0,1);
    sizeLayout->addWidget(_s,0,2);
    sizeLayout->addWidget(confrimButton,1,2);
    sizeLayout->addWidget(closeButton,1,3);

    connect(confrimButton,SIGNAL(clicked()),this,SLOT(confirmButtonClicked()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));

    orderWindow->show();
}

void MainWindow::closeButtonClicked()
{
    orderWindow->close();
  //  timer->start();
}

void MainWindow::confirmButtonClicked()
{
    int ord =_s->text().toInt();
    if(_model->getProdSet().contains(ord))
        _model->createOrder(ord);
    else
    {
        QMessageBox msgBox;
        msgBox.setText("The order number is incorrect");
        msgBox.exec();
    }
    orderWindow->close();
    //timer->start();
}
