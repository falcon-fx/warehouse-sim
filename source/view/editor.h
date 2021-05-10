#ifndef EDITOR_H
#define EDITOR_H

#include <model/model.h>
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
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

class Editor : public QObject
{
    Q_OBJECT
public:
    Editor(Model* model);
    int getSize() { return  this->size; }
    QVector<QPoint> getRobots() { return this->robots; }
    QVector<QPair<QPoint, QSet<int>>> getPods() { return this->pods; }
    QVector<QPair<QPoint, int>> getTargets() { return this->targets; }
    QVector<QPoint> getDocks() { return this->docks; }
private:
     Model* model;
     QWidget *editor;

     // The "New simulation" window
     QWidget *sizeWindow;
     QPushButton* okButton;
     QPushButton* closeButton;
     QLineEdit* sizeLEdit;
     QLineEdit* maxPowerLEdit;

     // Layouts
     QVBoxLayout* mainLayout;
     QGridLayout* gridLayout;
     QVBoxLayout* bottomLayout;
     QHBoxLayout* editButtonsLayout;
     QGridLayout* infoLayout;
     QHBoxLayout* infoButtonsLayout;
     QHBoxLayout* buttonContainer;
     QHBoxLayout* selectArrowLayout;
     QHBoxLayout* selectNewProdLayout;

     // Buttons
     QVector<QVector<QPushButton*>> gridButtons;
     QPushButton* selectButton;
     QPushButton* robotButton;
     QPushButton* podButton;
     QPushButton* targetButton;
     QPushButton* dockButton;
     QPushButton* deleteButton;
     QPushButton* undoButton;
     QPushButton* redoButton;
     QPushButton* selectUp;
     QPushButton* selectDown;
     QPushButton* selectLeft;
     QPushButton* selectRight;
     QPushButton* changeProdOkButton;
     QPushButton* newTableButton;
     QPushButton* loadTableButton;

     // Inputs
     QLineEdit* prodNumsLEdit;
     QComboBox* prodNumCBox;
     QLineEdit* changeProdNumsLEdit;

     QPushButton* newButton;
     QPushButton* applyButton;

     // A podok mozgatásához
     QPushButton* selectedButton;
     bool firstClicked;
     bool isSelected;
     QVector<QPoint> selectedGridButtons;
     QVector<QString> selectedProds;

     int size;
     int status;//1-select 2-robot 3-pod 4-target 5-dock 6-delete
     bool loadTable;

     // Conditions for making a correct warehouse
     QSet<int> necTargets;
     QSet<int> prodNums;

     QVector<QPoint> robots; // Robots' positions
     QVector<QPair<QPoint, QSet<int>>> pods; // Pods' positions and products
     QVector<QPair<QPoint, int>> targets; // Targets' positions and assigned products
     QVector<QPoint> docks; // Docks' positions

     void setupEditor();
     void setupSizeWindow();
     void setupTable();

     bool IsEmptyTile(QPoint);//az adott mezot ellenorizzuk, hogy van-e rajta valami
     bool isTheTableGood(); // ellenorzi, hogy a tabla, amit a felhasznalo keszitett alkalmas-e a szimulacio futtatasara
     void loadExistingTable();

private slots:
     void okButtonClicked();
     void closeButtonClicked();

     void editButtonsClicked();
     void controlButtonsClicked();

     void gridButtonClicked();

     void selectMoveButtonClicked();

     void changeProd();

     void newTableButtonClicked();
     void loadTableButtonClicked();


signals:
     void applyAndClose();
};

#endif // EDITORH
