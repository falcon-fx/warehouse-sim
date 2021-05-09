#include <view/editor.h>

#include <QDebug>

Editor::Editor(Model* model)
{
    setupSizeWindow();
    _model = model;
}

void Editor::okButtonClicked()
{
    if(_s->text().toInt()<3) // palya minimum merete
        _size=3;
    else
        _size = _s->text().toInt();

    _model->setMaxPower(_rPower->text().toInt());
    isSelected=false;

    sizeWindow->close();
    firstClicked=true;
    setupEditor();
}

void Editor::newTableButtonClicked()
{
    _loadTable=false;
    _s->setReadOnly(false);
    _rPower->setReadOnly(false);
    okButton->setDisabled(false);
}

void Editor::loadTableButtonClicked()
{
    _loadTable=true;
    _size = _model->getSize();
    isSelected=false;
    sizeWindow->close();
    firstClicked=true;
    setupEditor();
}

void Editor::closeButtonClicked()
{
    sizeWindow->close();
}

void Editor::setupTable()
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
    if(_size<=0)
    {
        _gridLayout->addWidget(new QLabel("Size cannot be 0 or smaller"));
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
                _gridButtons[i][j]= new QPushButton();
                _gridButtons[i][j]->setFixedSize(QSize(40,40));
                _gridButtons[i][j]->setEnabled(false);
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; border:0.5px solid gray;}");
                _gridLayout->setSpacing(0);
                _gridLayout->addWidget(_gridButtons[i][j], i, j);

                connect(_gridButtons[i][j], SIGNAL(clicked()), this,SLOT(gridButtonClicked()));

            }
        }
    }

}

void Editor::setupEditor()
{
    editor= new QWidget();

    editor->setWindowTitle("Editor");
    _mainLayout= new QVBoxLayout(editor);
    _gridLayout = new QGridLayout();
    _bottomLayout = new QVBoxLayout();
    _editButtonsLayout = new QHBoxLayout();
    _infoLayout = new QGridLayout();
    _infoButtonsLayout= new QHBoxLayout();
    _selectArrowLayout = new QHBoxLayout();
    _selectNewProdLayout = new QHBoxLayout();

    _selectButton = new QPushButton("Select");
    _robotButton= new QPushButton("Robot");
    _podButton= new QPushButton("Pod");
    _targetButton= new QPushButton("Target");
    _dockButton= new QPushButton("Dock");
    _deleteButton= new QPushButton("Delete");

    _prodNumsLEdit = new QLineEdit();

    _prodNumCBox = new QComboBox();

    _newButton= new QPushButton("New");
    _applyButton= new QPushButton("Apply and close");

    _selectUp= new QPushButton("Move Up");
    _selectUp->setVisible(false);
    _selectDown=new QPushButton("Move Down");
    _selectDown->setVisible(false);
    _selectLeft=new QPushButton("Move Left");
    _selectLeft->setVisible(false);
    _selectRight=new QPushButton("Move Right");
    _selectRight->setVisible(false);
    _changeProdOkButton=new QPushButton("Put new products");
    _changeProdOkButton->setVisible(false);
    _changeProdNumsLEdit = new QLineEdit();
    _changeProdNumsLEdit->setVisible(false);

    setupTable();

    _editButtonsLayout->addWidget(_selectButton);
    _editButtonsLayout->addWidget(_robotButton);
    _editButtonsLayout->addWidget(_dockButton);
    _editButtonsLayout->addWidget(_deleteButton);

    _infoLayout->addWidget(_prodNumsLEdit, 0, 0);
    _infoLayout->addWidget(_podButton, 0, 1);
    _infoLayout->addWidget(_prodNumCBox, 0, 2);
    _infoLayout->addWidget(_targetButton, 0, 3);

    _selectArrowLayout->addWidget(_selectUp);
    _selectArrowLayout->addWidget(_selectDown);
    _selectArrowLayout->addWidget(_selectLeft);
    _selectArrowLayout->addWidget(_selectRight);

    _selectNewProdLayout->addWidget(_changeProdNumsLEdit);
     _selectNewProdLayout->addWidget(_changeProdOkButton);

    _infoButtonsLayout->addWidget(_newButton);
    _infoButtonsLayout->addWidget(_applyButton);

    _bottomLayout->addLayout(_editButtonsLayout);
    _bottomLayout->addLayout(_selectArrowLayout);
    _bottomLayout->addLayout(_selectNewProdLayout);
    _bottomLayout->addLayout(_infoLayout);
    _bottomLayout->addLayout(_infoButtonsLayout);

    _mainLayout->addLayout(_gridLayout);
    _mainLayout->addLayout(_bottomLayout);

    connect(_selectButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_robotButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_podButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_targetButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_dockButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_deleteButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_selectUp,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_selectDown,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_selectRight,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_selectLeft,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_changeProdOkButton,SIGNAL(clicked()),this,SLOT(changeProd()));

    connect(_newButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_applyButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));

    robots.clear();
    pods.clear();
    targets.clear();
    docks.clear();

    prodNums.clear();
    _prodNumCBox->clear();

    if(_loadTable)
        loadExistingTable();

    editor->show();
}

void Editor::setupSizeWindow()
{
    sizeWindow = new QWidget;
    sizeWindow->setWindowTitle("New");

    QGridLayout* sizeLayout = new QGridLayout(sizeWindow);
    _newTableButton = new QPushButton("New simulation");
    _loadTableButton = new QPushButton("Load current simulation");
    _s = new QLineEdit("12");
    _rPower = new QLineEdit("100");
    okButton = new QPushButton("OK");
    closeButton = new QPushButton("Close");

    sizeLayout->addWidget(_loadTableButton,0,1,1,3);
    sizeLayout->addWidget(_newTableButton,1,1,2,3);
    sizeLayout->addWidget(new QLabel("Set size:"),3,1);
    sizeLayout->addWidget(new QLabel("Set robot power:"),4,1);
    sizeLayout->addWidget(_s,3,2);
    sizeLayout->addWidget(_rPower,4,2);
    sizeLayout->addWidget(okButton,5,2);
    sizeLayout->addWidget(closeButton,5,3);

    _s->setReadOnly(true);
    _rPower->setReadOnly(true);
    okButton->setDisabled(true);

    connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));
    connect(_newTableButton,SIGNAL(clicked()),this,SLOT(newTableButtonClicked()));
    connect(_loadTableButton,SIGNAL(clicked()),this,SLOT(loadTableButtonClicked()));

    sizeWindow->show();
}

void Editor::loadExistingTable()
{
    qDebug() << _model->getTargets().count();
    //targetek betoltese
    for(int i = 0; i < _model->getTargets().count();i++)
    {
        QPoint p = QPoint(_model->getTargets()[i].first.x()*40,_model->getTargets()[i].first.y()*40);
        QPair<QPoint, int> target_pair(p, _model->getTargets()[i].second);
        targets.append(target_pair);
        necTargets.insert( _model->getTargets()[i].second);
        _gridButtons[_model->getTargets()[i].first.y()][_model->getTargets()[i].first.x()]->setStyleSheet("QPushButton { background-color:rgb(146, 208, 80);  }");
        _gridButtons[_model->getTargets()[i].first.y()][_model->getTargets()[i].first.x()]->setText(QString::number(_model->getTargets()[i].second));

    }

    //dockok betoltese

    for(int i = 0; i < _model->getDocks().count();i++)
    {
        QPoint p = QPoint(_model->getDocks()[i].x()*40,_model->getDocks()[i].y()*40);
        docks.append(p);
        _gridButtons[_model->getDocks()[i].y()][_model->getDocks()[i].x()]->setStyleSheet("QPushButton { background-color: rgb(91, 155, 213);  }");
    }


    //podok betoltese
    for(int i = 0; i < _model->getPods().count();i++)
    {
        QPoint p = QPoint(_model->getPods()[i]->getPosition().y()*40,_model->getPods()[i]->getPosition().x()*40);

        QPair<QPoint, QSet<int>> pod_pair(p,_model->getPods()[i]->getProducts());
        pods.append(pod_pair);

        _gridButtons[_model->getPods()[i]->getPosition().x()][_model->getPods()[i]->getPosition().y()]->setStyleSheet("QPushButton { background-color:   rgb(230, 230, 230) }");

        for (int j = 0; j <  _model->getPods()[i]->getProducts().count(); j++)
            prodNums = prodNums.unite(_model->getPods()[i]->getProducts());

        QString podText ="P\n";
        foreach (const int &value, _model->getPods()[i]->getProducts())
        {
            podText += QString::number(value) + " ";
        }
        _gridButtons[_model->getPods()[i]->getPosition().x()][_model->getPods()[i]->getPosition().y()]->setText(podText);
    }

    foreach (const int &value, prodNums)
        if (_prodNumCBox->findText(QString::number(value)) == -1)
            _prodNumCBox->insertItem(_prodNumCBox->count(), QString::number(value));
    _prodNumCBox->model()->sort(0, Qt::AscendingOrder);

    //robotok betoltese
    for(int i = 0; i < _model->getRobots().count();i++){
        QPoint p = QPoint(_model->getRobots()[i]->getPosition().y()*40,_model->getRobots()[i]->getPosition().x()*40);
        robots.append(p);
        _gridButtons[_model->getRobots()[i]->getPosition().x()][_model->getRobots()[i]->getPosition().y()]->setStyleSheet("QPushButton { background-color:  rgb(255, 192, 0); }");
    }


}

void Editor::editButtonsClicked()
{
    if(!firstClicked)
    {
        selectedButton->setStyleSheet("background-color: auto;");
    }
    selectedButton = qobject_cast<QPushButton*>(sender());
    selectedButton->setStyleSheet("background-color: #ff8a8a;");
    firstClicked=false;

    if(selectedButton->text() !="Undo" && selectedButton->text() !="Redo")
    {
        for (int i = 0; i < _size; i++)
        {
            for (int j = 0; j < _size; j++)
                _gridButtons[i][j]->setEnabled(true);
        }
    }


    if(selectedButton->text() == "Select")
    {
        status=1;
    }
    else if(selectedButton->text() == "Robot")
    {
        status=2;
    }
    else if(selectedButton->text() == "Pod")
    {
        status=3;
    }
    else if(selectedButton->text() == "Target")
    {
        status=4;
    }
    else if(selectedButton->text() == "Dock")
    {
        status=5;
    }
    else if(selectedButton->text() == "Delete")
    {
        status=6;
    }

    if(status!=1) //selectrol elkattintunk, eltuntetjuk a mozgato gombokat
    {
        isSelected = false;
        _selectUp->setVisible(false);
        _selectDown->setVisible(false);
        _selectLeft->setVisible(false);
        _selectRight->setVisible(false);
        _changeProdOkButton->setVisible(false);
        _changeProdNumsLEdit->setVisible(false);

    }
    else//select opcioban vagyunk, bekapcsoljuk a mozgato gombokat
    {
        isSelected=true;
        _selectUp->setVisible(true);
        _selectDown->setVisible(true);
        _selectLeft->setVisible(true);
        _selectRight->setVisible(true);
        _changeProdOkButton->setVisible(true);
        _changeProdNumsLEdit->setVisible(true);
    }

    selectedGridButtons.clear();//a kivalasztast toroljuk
    selectedProds.clear();

    for (int i = 0; i < _size; i++)//visszaallitjuk a mezoket a selectrol(eltunik a piros keret)
    {
        for (int j = 0; j < _size; j++)
        {
            QColor color = _gridButtons[i][j]->palette().button().color();
            _gridButtons[i][j]->setStyleSheet("border:0.5px solid gray; background-color: " + color.name() +";");
        }
    }
}

void Editor::controlButtonsClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    if(btn->text() == "New")
    {
        editor->close();
        setupSizeWindow();
    }
    else if(btn->text() == "Apply and close")
    {
        if(isTheTableGood())
        {
            for (int i = 0; i < robots.count(); i++)
            {
                robots[i].setX(robots[i].x() / _gridButtons[0][0]->size().width());
                robots[i].setY(robots[i].y() / _gridButtons[0][0]->size().width());
            }
            for (int i = 0; i < pods.count(); i++)
            {
                pods[i].first.setX(pods[i].first.x() / _gridButtons[0][0]->size().width());
                pods[i].first.setY(pods[i].first.y() / _gridButtons[0][0]->size().width());
            }
            for (int i = 0; i < targets.count(); i++)
            {
                targets[i].first.setX(targets[i].first.x() / _gridButtons[0][0]->size().width());
                targets[i].first.setY(targets[i].first.y() / _gridButtons[0][0]->size().width());
            }
            for (int i = 0; i < docks.count(); i++)
            {
                docks[i].setX(docks[i].x() / _gridButtons[0][0]->size().width());
                docks[i].setY(docks[i].y() / _gridButtons[0][0]->size().width());
            }
            emit applyAndClose();
            editor->close();
        }
        else{
            QMessageBox msgBox;
            msgBox.setText("The simulation has to have at least one robot, one dock, one product and one target to every product");
            msgBox.exec();
        }
    }
}

void Editor::gridButtonClicked()
{
    if(!_loadTable && firstClicked)
        _model->setSize(_size);
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    //btn->setText("");

    QPoint p = btn->pos() - _gridButtons[0][0]->pos();

    if(IsEmptyTile(p))
    {
        QSet<int> prods;
        if (!_prodNumsLEdit->text().isEmpty())
        {
            QStringList numbers = _prodNumsLEdit->text().split(",");
            for (int i = 0; i < numbers.count(); i++)
            {
                prods.insert(numbers[i].toInt());
                prodNums.insert(numbers[i].toInt());
            }
        }
        QString podText ="P\n";
        foreach (const int &value, prods)
        {
            podText += QString::number(value) + " ";
        }

        int prodNum = _prodNumCBox->currentText().toInt();
        QPair<QPoint, QSet<int>> pod_pair(p, prods);
        QPair<QPoint, int> target_pair(p, prodNum);

        switch(status)
        {
        case 2://robot
            btn->setStyleSheet("QPushButton { background-color: rgb(255, 192, 0); }");
            robots.append(p);
            for (int i = 0; i < pods.count(); i++)
            {
                if (pods[i].first == p)
                {
                    pods.remove(i);
                    break;
                }
            }
            for (int i = 0; i < targets.count(); i++)
            {
                if (targets[i].first == p)
                {
                    targets.remove(i);
                    break;
                }
            }
            docks.removeOne(p);

            break;
        case 3://pod
            btn->setStyleSheet("QPushButton { background-color: rgb(230, 230, 230); }");
            btn->setText(podText);
            foreach (const int &value, prodNums)
                if (_prodNumCBox->findText(QString::number(value)) == -1)
                    _prodNumCBox->insertItem(_prodNumCBox->count(), QString::number(value));
             _prodNumCBox->model()->sort(0, Qt::AscendingOrder);
            robots.removeOne(p);
            pods.append(pod_pair);
            for (int i = 0; i < targets.count(); i++)
            {
                if (targets[i].first == p)
                {
                    targets.remove(i);
                    break;
                }
            }
            docks.removeOne(p);
            break;
        case 4://target
            if (_prodNumCBox->count() == 0)
                break;
            btn->setStyleSheet("QPushButton { background-color: rgb(146, 208, 80); }");
            btn->setText(QString::number(prodNum));
            necTargets.insert(prodNum);
            robots.removeOne(p);
            for (int i = 0; i < pods.count(); i++)
            {
                if (pods[i].first == p)
                {
                    pods.remove(i);
                    break;
                }
            }
            targets.append(target_pair);
            docks.removeOne(p);
            break;
        case 5://dock
            btn->setStyleSheet("QPushButton { background-color: rgb(91, 155, 213); }");
            robots.removeOne(p);
            for (int i = 0; i < pods.count(); i++)
            {
                if (pods[i].first == p)
                {
                    pods.remove(i);
                    break;
                }
            }
            for (int i = 0; i < targets.count(); i++)
            {
                if (targets[i].first == p)
                {
                    targets.remove(i);
                    break;
                }
            }
            docks.append(p);
            break;
        }
    }
    switch (status) { //select
    case 1:
    {
        QColor color = btn->palette().button().color();
        if(isSelected && color.name() == "#e6e6e6")
        {
            btn->setStyleSheet("border:3px solid red; background-color: " + color.name() +";");

            bool contains=false;
            for(int i = 0; i<selectedGridButtons.count();i++) //ellenorizzuk, hogy nincs e mar a vectorban a kivalasztott mezo
                if(selectedGridButtons[i] == p) contains = true;


            if(!contains)
            {
                if(_size>7)
                    selectedGridButtons.append(QPoint((btn->pos().x()-11) ,(btn->pos().y()-11)));
                else
                    selectedGridButtons.append(QPoint((btn->pos().x()-11 -45 -(7-_size)*20) ,(btn->pos().y()-11)));
                selectedProds.append(btn->text());
            }
        }
        break;
    }
    case 6: //delete
    {
        btn->setStyleSheet("QPushButton { background-color: white; }");
        btn->setText("");
        robots.removeOne(p);
        for (int i = 0; i < pods.count(); i++)
        {
            if (pods[i].first == p)
            {
                pods.remove(i);
                break;
            }
        }
        for (int i = 0; i < targets.count(); i++)
        {
            if (targets[i].first == p)
            {
                targets.remove(i);
                break;
            }
        }
        docks.removeOne(p);
        break;
    }
    }
 }

void Editor::selectMoveButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int dirX=0;
    int dirY=0;
    if(btn->text() =="Move Up")//irany beallitasa
    {
        dirX = 0;
        dirY = -40;
    }
    else if(btn->text() =="Move Down")
    {
        dirX = 0;
        dirY = 40;
    }
    else if(btn->text() =="Move Left")
    {
        dirX = -40;
        dirY = 0;
    }
    else if(btn->text() =="Move Right")
    {
        dirX = 40;
        dirY = 0;
    }
    for(int i=0; i < selectedGridButtons.count();i++)
    {
        int number=0;
        while(pods[number].first != selectedGridButtons[i] && number < pods.size()-1) // megkeressuk a podsban hanyadik a kivalasztott elem
            number++;

        if(selectedGridButtons[i].y()+dirY>=0 && selectedGridButtons[i].y()+dirY<_size*40 && selectedGridButtons[i].x()+dirX>=0 && selectedGridButtons[i].x()+dirX<_size*40)
        {
            QPoint newPlace =QPoint(selectedGridButtons[i].x()+dirX,selectedGridButtons[i].y()+dirY);
            if(IsEmptyTile(newPlace))
            {
                _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setStyleSheet("QPushButton { background-color: white; }"); // valamiert fel vannak cserelodve a koordinatak ahhh
                _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setText("");
                selectedGridButtons[i] = QPoint(selectedGridButtons[i].x()+dirX,selectedGridButtons[i].y()+dirY);
                _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setStyleSheet("QPushButton {border:3px solid red; background-color: #e6e6e6; }");
                _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setText(selectedProds[i]);

                QSet<int> prods;
                //pod helyenek frissitese
                QStringList prodnum = selectedProds[i].split(" ");
                prodnum[0].remove(0,2);
                if(prodnum.size()!=1)
                    prodnum.removeLast();

                if(prodnum[0]!="")
                    for (int j = 0; j < prodnum.count(); j++)
                        prods.insert(prodnum[j].toInt());

                QPair<QPoint, QSet<int>> pod_pair(newPlace, prods);
                pods.replace(number,pod_pair);
            }
        }
    }
}

void Editor::changeProd()
{
    for(int i=0; i < selectedGridButtons.count();i++)
    {
        int number=0;
        while(pods[number].first != selectedGridButtons[i] && number < pods.size()-1) // megkeressuk a podsban hanyadik a kivalasztott elem
            number++;

        QPoint newPlace = QPoint(selectedGridButtons[i].x(),selectedGridButtons[i].y());
        QSet<int> prods;
        if (!_changeProdNumsLEdit->text().isEmpty())
        {
            QStringList numbers = _changeProdNumsLEdit->text().split(",");
            for (int i = 0; i < numbers.count(); i++)
            {
                prods.insert(numbers[i].toInt());
                prodNums.insert(numbers[i].toInt());
            }
        }
        QString podText ="P\n";
        foreach (const int &value, prods)
        {
            podText += QString::number(value) + " ";
        }

        selectedProds[i] = podText;
        _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setText(podText);
        QPair<QPoint, QSet<int>> pod_pair(newPlace, prods);
        pods.replace(number,pod_pair);

        foreach (const int &value, prodNums)
            if (_prodNumCBox->findText(QString::number(value)) == -1)
                _prodNumCBox->insertItem(_prodNumCBox->count(), QString::number(value));
         _prodNumCBox->model()->sort(0, Qt::AscendingOrder);
    }

}


bool Editor::IsEmptyTile(QPoint point)
{
    //check robots

    point = QPoint(point.x()/40,point.y()/40);

    for(int i = 0; i < robots.count();i++)
    {
        if(robots[i].x()/40== point.x() && robots[i].y()/40 == point.y())
            return false;
    }
    //check pods
    for(int i= 0; i < pods.count();i++)
    {
        if(pods[i].first.x()/40 == point.x() && pods[i].first.y()/40 == point.y())
            return false;
    }
    //check targets
    for(int i= 0; i < targets.count();i++)
    {
        if(targets[i].first.x()/40 == point.x() && targets[i].first.y()/40 == point.y())
            return false;
    }
    //check docks
    for(int i= 0; i < docks.count();i++)
    {
        if(docks[i].x()/40 == point.x() && docks[i].y()/40 == point.y())
            return false;
    }
    return true;
}

bool Editor::isTheTableGood()
{
    if(robots.count()>=1 && docks.count()>=1 && necTargets == prodNums && prodNums.count() >= 1)
    {
        return true;
    }
    return false;
}






