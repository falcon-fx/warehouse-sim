#include "editor.h"

#include <qDebug>

Editor::Editor()
{
    sizeWindow = new QWidget;

    QGridLayout* sizeLayout = new  QGridLayout(sizeWindow);
    _h = new QLineEdit("10");
    _w = new QLineEdit("10");
    okButton = new QPushButton("OK");
    closeButton = new QPushButton("Close");

    sizeLayout->addWidget(new QLabel("Set size:"),0,1);
    sizeLayout->addWidget(_h,0,2);
    sizeLayout->addWidget(_w,0,3);
    sizeLayout->addWidget(okButton,1,2);
    sizeLayout->addWidget(closeButton,1,3);

    connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));

    sizeWindow->show();

}

void Editor::okButtonClicked()
{
    _height=_h->text().toInt();
    _width=_w->text().toInt();

    sizeWindow->close();
    editor= new QWidget;
    editor->setWindowTitle("Editor");

    setupEditor();
    editor->show();
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
    if(_height<=0 || _width <= 0)
    {
        _gridLayout->addWidget(new QLabel("Size cannot be 0 or smaller"));
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
                _gridButtons[i][j]= new QPushButton();
                _gridButtons[i][j]->setFixedSize(QSize(40,40));
                _gridButtons[i][j]->setEnabled(false);
                _gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; }");
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
    _mainLayout= new QVBoxLayout(editor);
    _gridLayout = new QGridLayout();
    _bottomLayout = new QVBoxLayout();
    _editButtonsLayout = new QHBoxLayout();
    _infoLayout = new QGridLayout();
    _infoButtonsLayout= new QHBoxLayout();

    _selectButton = new QPushButton("Select");
    _robotButton= new QPushButton("Robot");
    _podButton= new QPushButton("Pod");
    _targetButton= new QPushButton("Target");
    _dockButton= new QPushButton("Dock");
    _deleteButton= new QPushButton("Delete");
    //_undoButton= new QPushButton("Undo");
    //_redoButton= new QPushButton("Redo");

    _newButton= new QPushButton("New");
    _loadButton= new QPushButton("Load");
    _saveButton= new QPushButton("Save");
    _applyButton= new QPushButton("Apply and close");

    setupTable();

    _editButtonsLayout->addWidget(_selectButton);
    _editButtonsLayout->addWidget(_robotButton);
    _editButtonsLayout->addWidget(_podButton);
    _editButtonsLayout->addWidget(_targetButton);
    _editButtonsLayout->addWidget(_dockButton);
    _editButtonsLayout->addWidget(_deleteButton);
    //_editButtonsLayout->addWidget(_undoButton);
    //_editButtonsLayout->addWidget(_redoButton);

    //info cuccokat valahogy kitalalni

    _infoButtonsLayout->addWidget(_newButton);
    _infoButtonsLayout->addWidget(_loadButton);
    _infoButtonsLayout->addWidget(_saveButton);
    _infoButtonsLayout->addWidget(_applyButton);

    _bottomLayout->addLayout(_editButtonsLayout);
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

    connect(_newButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_loadButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_saveButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));
    connect(_applyButton,SIGNAL(clicked()),this,SLOT(controlButtonsClicked()));

}

void Editor::editButtonsClicked()
{

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(btn->text() !="Undo" && btn->text() !="Redo")
    {
        for (int i = 0; i < _height; i++)
        {
            for (int j = 0; j < _width; j++)
                _gridButtons[i][j]->setEnabled(true);
        }
    }

    if(btn->text() == "Select")
    {
        status=1;
    }
    else if(btn->text() == "Robot")
    {
        status=2;
    }
    else if(btn->text() == "Pod")
    {
        status=3;
    }
    else if(btn->text() == "Target")
    {
        status=4;
    }
    else if(btn->text() == "Dock")
    {
        status=5;
    }
    else if(btn->text() == "Delete")
    {
        status=6;
    }
    else if(btn->text() == "Undo")
    {

    }
    else if(btn->text() == "Redo")
    {

    }
}

void Editor::controlButtonsClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(btn->text() == "New")
    {
        editor->close();
        sizeWindow = new QWidget;

        QGridLayout* sizeLayout = new  QGridLayout(sizeWindow);
        _h = new QLineEdit("10");
        _w = new QLineEdit("10");
        okButton = new QPushButton("OK");
        closeButton = new QPushButton("Close");

        sizeLayout->addWidget(new QLabel("Set size:"),0,1);
        sizeLayout->addWidget(_h,0,2);
        sizeLayout->addWidget(_w,0,3);
        sizeLayout->addWidget(okButton,1,2);
        sizeLayout->addWidget(closeButton,1,3);

        connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
        connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));

        sizeWindow->show();

    }
    else if(btn->text() == "Load")
    {

    }
    else if(btn->text() == "Save")
    {

    }
    else if(btn->text() == "Apply and close")
    {
        editor->close();
    }
}

 void Editor::gridButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
            _gridButtons[i][j]->setEnabled(false);
    }

    switch(status)
    {
    case 1://select
        //btn->setStyleSheet("QPushButton { background-color: gray; }");
        break;
    case 2://robot
        btn->setStyleSheet("QPushButton { background-color: yellow; }");
        break;
    case 3://pod
        btn->setStyleSheet("QPushButton { background-color: gray; }");
        break;
    case 4://target
        btn->setStyleSheet("QPushButton { background-color: green; }");
        break;
    case 5://dock
        btn->setStyleSheet("QPushButton { background-color: blue; }");
        break;
    case 6://delete
        btn->setStyleSheet("QPushButton { background-color: white; }");
        break;
    }

}
