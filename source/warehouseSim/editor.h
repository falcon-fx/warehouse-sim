#ifndef EDITOR_H
#define EDITOR_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QObject>

class Editor : public QObject
{
    Q_OBJECT
public:
    Editor();
private:
     QWidget *editor;
     QWidget *sizeWindow;

     //sizeWindow
     QPushButton* okButton;
     QPushButton* closeButton;
     QLineEdit* _height;
     QLineEdit* _width;

     //layouts:
     QHBoxLayout* _mainLayout;
     QGridLayout* _gridLayout;
     QVBoxLayout* _bottomLayout;
     QHBoxLayout* _editButtonsLayout;
     QGridLayout* _infoLayout;
     QHBoxLayout* _infoButtonsLayout;
     QHBoxLayout* buttonContainer;

     //buttons:
     QVector<QVector<QPushButton*>> _gridButtons;
     QPushButton* _selectButton;
     QPushButton* _robotButton;
     QPushButton* _podButton;
     QPushButton* _targetButton;
     QPushButton* _dockButton;
     QPushButton* _deleteButton;
     QPushButton* _undoButton;
     QPushButton* _redoButton;

     QPushButton* _resetButton;
     QPushButton* _loadButton;
     QPushButton* _saveButton;
     QPushButton* _applyButton;


     void setupEditor();

private slots:
     void okButtonClicked();
     void closeButtonClicked();

};

#endif // EDITOR_H
