#ifndef EDITOR_H
#define EDITOR_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QObject>
#include <QVector>
#include <QPoint>
#include <QSet>
#include <QPair>
#include <QComboBox>
#include <QKeyEvent>
#include <QString>


class Editor : public QObject
{
    Q_OBJECT
public:
    Editor();
    int getSize() { return  this->_size; }
    QVector<QPoint> getRobots() { return this->robots; }
    QVector<QPair<QPoint, QSet<int>>> getPods() { return this->pods; }
    QVector<QPair<QPoint, int>> getTargets() { return this->targets; }
    QVector<QPoint> getDocks() { return this->docks; }
private:
     QWidget *editor;
     QWidget *sizeWindow;

     //sizeWindow
     QPushButton* okButton;
     QPushButton* closeButton;
     QLineEdit* _s;

     //layouts:
     QVBoxLayout* _mainLayout;
     QGridLayout* _gridLayout;
     QVBoxLayout* _bottomLayout;
     QHBoxLayout* _editButtonsLayout;
     QGridLayout* _infoLayout;
     QHBoxLayout* _infoButtonsLayout;
     QHBoxLayout* buttonContainer;
     QHBoxLayout* _selectArrowLayout;


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
     QPushButton* _selectUp;
     QPushButton* _selectDown;
     QPushButton* _selectLeft;
     QPushButton* _selectRight;

     //inputs:
     QLineEdit* _prodNumsLEdit;
     QComboBox* _prodNumCBox;

     QPushButton* _newButton;
     QPushButton* _loadButton;
     QPushButton* _saveButton;
     QPushButton* _applyButton;

     QPushButton* selectedButton;
     bool firstClicked;

     int _size;
     int status;//1-select 2-robot 3-pod 4-target 5-dock 6-delete
     bool isSelected;
     //int selectedNumber;

     QVector<QPoint> place;
     QVector<QPoint> selectedGridButtons;
     QVector<QString> selectedProds;


     QVector<QPoint> robots;
     QVector<QPair<QPoint, QSet<int>>> pods;
     QVector<QPair<QPoint, int>> targets;
     QVector<QPoint> docks;

     QSet<int> prodNums;

     void setupEditor();
     void setupSizeWindow();
     void setupTable();

protected:
     //void keyPressEvent(QKeyEvent* event);

private slots:
     void okButtonClicked();
     void closeButtonClicked();

     void editButtonsClicked();
     void controlButtonsClicked();

     void gridButtonClicked();

     void selectMoveButtonClicked();


signals:
     void applyAndClose();
};

#endif // EDITOR_H
