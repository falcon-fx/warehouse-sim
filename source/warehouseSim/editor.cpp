#include "editor.h"

#include <qDebug>

Editor::Editor()
{
    sizeWindow = new QWidget;
    QGridLayout* sizeLayout = new  QGridLayout(sizeWindow);
    _height = new QLineEdit();
    _width = new QLineEdit();
    okButton = new QPushButton("OK");
    closeButton = new QPushButton("Close");

    sizeLayout->addWidget(new QLabel("Set size:"),0,1);
    sizeLayout->addWidget(_height,0,2);
    sizeLayout->addWidget(_width,0,3);
    sizeLayout->addWidget(okButton,1,2);
    sizeLayout->addWidget(closeButton,1,3);

    connect(okButton,SIGNAL(clicked()),this,SLOT(okButtonClicked()));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(closeButtonClicked()));

    sizeWindow->show();

}

void Editor::okButtonClicked()
{
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


void Editor::setupEditor()
{
    editor= new QWidget();
    _mainLayout= new QHBoxLayout(editor);
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
    _undoButton= new QPushButton("Undo");
    _redoButton= new QPushButton("Redo");

    _resetButton= new QPushButton("Reset");
    _loadButton= new QPushButton("Load");
    _saveButton= new QPushButton("Save");
    _applyButton= new QPushButton("Apply & close");

    //gridet valahogy

    _editButtonsLayout->addWidget(_selectButton);
    _editButtonsLayout->addWidget(_robotButton);
    _editButtonsLayout->addWidget(_podButton);
    _editButtonsLayout->addWidget(_targetButton);
    _editButtonsLayout->addWidget(_dockButton);
    _editButtonsLayout->addWidget(_deleteButton);
    _editButtonsLayout->addWidget(_undoButton);
    _editButtonsLayout->addWidget(_redoButton);

    //info cuccokat valahogy kitalalni

    _infoButtonsLayout->addWidget(_resetButton);
    _infoButtonsLayout->addWidget(_loadButton);
    _infoButtonsLayout->addWidget(_saveButton);
    _infoButtonsLayout->addWidget(_applyButton);

    _bottomLayout->addLayout(_editButtonsLayout);
    _bottomLayout->addLayout(_infoLayout);
    _bottomLayout->addLayout(_infoButtonsLayout);

    _mainLayout->addLayout(_gridLayout);
    _mainLayout->addLayout(_bottomLayout);
}
