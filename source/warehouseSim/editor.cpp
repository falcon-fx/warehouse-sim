#include "editor.h"

#include <QDebug>

Editor::Editor()
{
    setupSizeWindow();
}

void Editor::okButtonClicked()
{
    //_height=_h->text().toInt();
    //_width=_w->text().toInt();
    _size = _s->text().toInt();
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

    //selectedNumber=0;

    editor->setWindowTitle("Editor");
    _mainLayout= new QVBoxLayout(editor);
    _gridLayout = new QGridLayout();
    _bottomLayout = new QVBoxLayout();
    _editButtonsLayout = new QHBoxLayout();
    _infoLayout = new QGridLayout();
    _infoButtonsLayout= new QHBoxLayout();
    _selectArrowLayout = new QHBoxLayout();

    _selectButton = new QPushButton("Select");
    _robotButton= new QPushButton("Robot");
    _podButton= new QPushButton("Pod");
    _targetButton= new QPushButton("Target");
    _dockButton= new QPushButton("Dock");
    _deleteButton= new QPushButton("Delete");
    //_undoButton= new QPushButton("Undo");
    //_redoButton= new QPushButton("Redo");

    _prodNumsLEdit = new QLineEdit();

    _prodNumCBox = new QComboBox();

    _newButton= new QPushButton("New");
    _loadButton= new QPushButton("Load");
    _saveButton= new QPushButton("Save");
    _applyButton= new QPushButton("Apply and close");

    _selectUp= new QPushButton("Move Up");
    _selectUp->setVisible(false);
    _selectDown=new QPushButton("Move Down");
    _selectDown->setVisible(false);
    _selectLeft=new QPushButton("Move Left");
    _selectLeft->setVisible(false);
    _selectRight=new QPushButton("Move Right");
    _selectRight->setVisible(false);

    setupTable();

    _editButtonsLayout->addWidget(_selectButton);
    _editButtonsLayout->addWidget(_robotButton);
    _editButtonsLayout->addWidget(_dockButton);
    _editButtonsLayout->addWidget(_deleteButton);
    //_editButtonsLayout->addWidget(_undoButton);
    //_editButtonsLayout->addWidget(_redoButton);

    _infoLayout->addWidget(_prodNumsLEdit, 0, 0);
    _infoLayout->addWidget(_podButton, 0, 1);
    _infoLayout->addWidget(_prodNumCBox, 0, 2);
    _infoLayout->addWidget(_targetButton, 0, 3);

    _selectArrowLayout->addWidget(_selectUp);
    _selectArrowLayout->addWidget(_selectDown);
    _selectArrowLayout->addWidget(_selectLeft);
    _selectArrowLayout->addWidget(_selectRight);

    _infoButtonsLayout->addWidget(_newButton);
    _infoButtonsLayout->addWidget(_loadButton);
    _infoButtonsLayout->addWidget(_saveButton);
    _infoButtonsLayout->addWidget(_applyButton);

    _bottomLayout->addLayout(_editButtonsLayout);
    _bottomLayout->addLayout(_selectArrowLayout);
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
    //connect(_undoButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
   // connect(_redoButton,SIGNAL(clicked()),this,SLOT(editButtonsClicked()));
    connect(_selectUp,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_selectDown,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_selectRight,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));
    connect(_selectLeft,SIGNAL(clicked()),this,SLOT(selectMoveButtonClicked()));

    connect(_newButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_loadButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_saveButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_applyButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));

    //connect(,SIGNAL(pressed()),this,SLOT(keyPressEvent(QKeyEvent* event)));

    robots.clear();
    pods.clear();
    targets.clear();
    docks.clear();

    prodNums.clear();
    _prodNumCBox->clear();

    editor->show();
}

void Editor::setupSizeWindow()
{
    sizeWindow = new QWidget;
    sizeWindow->setWindowTitle("New");

    QGridLayout* sizeLayout = new QGridLayout(sizeWindow);
    _s = new QLineEdit("10");
    //_w = new QLineEdit("10");
    okButton = new QPushButton("OK");
    closeButton = new QPushButton("Close");

    sizeLayout->addWidget(new QLabel("Set size:"),0,1);
    sizeLayout->addWidget(_s,0,2);
    //sizeLayout->addWidget(_w,0,3);
    sizeLayout->addWidget(okButton,1,2);
    sizeLayout->addWidget(closeButton,1,3);

    connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));

    sizeWindow->show();
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
    /*else if(selectedButton->text() == "Undo")
    {

    }
    else if(selectedButton->text() == "Redo")
    {

    }*/

    if(status!=1) //selectrol elkattintunk, eltuntetjuk a mozgato gombokat
    {
        isSelected = false;
        _selectUp->setVisible(false);
        _selectDown->setVisible(false);
        _selectLeft->setVisible(false);
        _selectRight->setVisible(false);

    }
    else//select opcioban vagyunk, bekapcsoljuk a mozgato gombokat
    {
        isSelected=true;
        _selectUp->setVisible(true);
        _selectDown->setVisible(true);
        _selectLeft->setVisible(true);
        _selectRight->setVisible(true);
    }

    selectedGridButtons.clear();//a kivalasztast toroljuk

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
    else if(btn->text() == "Load")
    {

    }
    else if(btn->text() == "Save")
    {

    }
    else if(btn->text() == "Apply and close")
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
}

void Editor::gridButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    //btn->setText("");

    QPoint p = btn->pos() - _gridButtons[0][0]->pos();

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
    case 1://select
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
                selectedGridButtons.append(QPoint((btn->pos().x()-11) ,(btn->pos().y()-11)));
                place.append(QPoint(selectedGridButtons.last().x()/40,selectedGridButtons.last().y()/40));
                /*foreach (const int &value, prods)
                {
                    selectedProds.append(prods);
                    podText += QString::number(value) + " ";
                }*/
                selectedProds.append(prods);

            }
        }
        break;
    }
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
    case 6://delete
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

void Editor::selectMoveButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    for(int i=0; i < selectedGridButtons.count();i++)
    {
        //qDebug() << selectedTiles[i].first << " "<< selectedTiles[i].second ;
        if(btn->text() =="Move Up")
        {
            int number=0;
            while(pods[number].first != selectedGridButtons[i] && number < pods.size()-1)
                number++;

            if(selectedGridButtons[i].y()-40>=0)
            {
                    _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setStyleSheet("QPushButton { background-color: white; }"); // valamiert fel vannak cserelodve a koordinatak ahhh
                    selectedGridButtons[i] = QPoint(selectedGridButtons[i].x(),selectedGridButtons[i].y()-40);
                    _gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setStyleSheet("QPushButton { background-color: #e6e6e6; }");


                    QPoint newPlace = QPoint(selectedGridButtons[i].x(),selectedGridButtons[i].y() - 40);
                    qDebug() << newPlace;

                    QPair<QPoint, QSet<int>> pod_pair(newPlace, selectedProds[i]);

                    //btn->setText(podText);
                    foreach (const int &value, prodNums)
                        if (_prodNumCBox->findText(QString::number(value)) == -1)
                            _prodNumCBox->insertItem(_prodNumCBox->count(), QString::number(value));
                    robots.removeOne(newPlace);
                    pods.replace(number,pod_pair);
                    for (int j = 0; j < targets.count(); j++)
                    {
                        if (targets[j].first == newPlace)
                        {
                            targets.remove(j);
                            break;
                        }
                    }
                    docks.removeOne(newPlace);

                }



        }
        else if(btn->text() =="Move Down")
        {
            //qDebug() << "down";
        }
        else if(btn->text() =="Move Left")
        {
            //qDebug() << "left";
        }
        else if(btn->text() =="Move Right")
        {
            //qDebug() << "right";
        }
    }

}
