#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editor.h"
#include "model.h"

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    QWidget *window;
    QHBoxLayout* _mainLayout;
    QVBoxLayout* _leftsideLayout;
    QVBoxLayout* _titleLayout;
    QVBoxLayout* _infoLayout;
    QGridLayout* _buttonLayout;
    QVBoxLayout *_rightsideLayout;
    QGridLayout* _gridLayout;
    QVector<QVector<QPushButton*>> _gridButtons;
    QHBoxLayout* buttonContainer;
private:
    Model* _model;
    Editor* _editor;
    void setupWindow();
    void drawTable();
    void on_refreshTable();
    int Size;
    int _height;
    int _width;
private slots:
    void editorButtonClicked();
    void startButtonClicked();
    void saveButtonClicked();
    void loadButtonClicked();


};
#endif // MAINWINDOW_H
