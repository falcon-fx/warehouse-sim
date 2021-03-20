#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    QVBoxLayout* _sideLayout;
    QVBoxLayout* _titleLayout;
    QVBoxLayout* _infoLayout;
    QGridLayout* _buttonLayout;
    QVBoxLayout *test;
    QGridLayout* _gridLayout;
    QVector<QVector<QPushButton*>> _gridButtons;
    QHBoxLayout* buttonContainer;
private:
    void setupWindow();
    void drawTable();
    int Size;
    int _height;
    int _width;
};
#endif // MAINWINDOW_H
