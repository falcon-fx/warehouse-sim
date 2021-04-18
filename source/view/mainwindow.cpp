#include <view/mainwindow.h>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QFont>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _size = 12;
    _steps = 0;
    _model = new Model(_size, 500);
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
    _model->createOrder(1);
    _model->createOrder(2);
    _model->createOrder(3);
    _model->createOrder(4);
    _model->createOrder(3);
    _model->createOrder(1);
    _model->createOrder(4);
    _model->createOrder(2);
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
    QPushButton *editorButton = new QPushButton("Open Editor");
    startButton = new QPushButton("Start");
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setMinimum(0);
    speedSlider->setMaximum(5000);
    speedSlider->setTickInterval(100);
    speedSlider->setValue(600);

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
            _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
            _gridButtons[i][j]->setText("");

            if (warehouse[i][j]->isDock())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(91, 155, 213); }");
            }
            else if (warehouse[i][j]->isTarget())
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(146, 208, 80); color: black; }");
                _gridButtons[i][j]->setText(QString::number(warehouse[i][j]->getTarget()));
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
            if(_model->getRobot(i, j) != nullptr)
            {
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: rgb(255, 192, 0); }");
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
    if(timer->isActive()) {
        timer->stop();
        startButton->setText("Start");
    } else {
        timer->start(speedSlider->value());
        startButton->setText("Stop");
    }
}

void MainWindow::speedSliderChanged(int value)
{
    if (timer->isActive())
    {
        timer->stop();
        timer->start(value);
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
        res.append(QString::number(i+1) + ". robot: " + QString::number(_energyUsed[i]) + "\n");
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
}

void MainWindow:: newOrder()
{
    timer->stop();
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
    bool isContain = false;
    int counter=0;
    /*while (!isContain /*&& counter < _model-> )
    {
        counter++;
    }*/
    if(true)
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
