#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <view/editor.h>
#include <model/model.h>

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    QWidget *window;
    //layoutok
    QHBoxLayout* _mainLayout;
    QVBoxLayout* _leftsideLayout;
    QVBoxLayout* _titleLayout;
    QVBoxLayout* _infoLayout;
    QGridLayout* _buttonLayout;
    QVBoxLayout *_rightsideLayout;
    QGridLayout* gridLayout;
    QHBoxLayout* buttonContainer;
    QVector<QVector<QPushButton*>> _gridButtons;

    QTimer* timer;
    QLabel* infoLabel;
    QLabel* noDataLabel;
    QPushButton* startButton;
    QPushButton* editorButton;
    QSlider* speedSlider;
    QPushButton* _newOrderButton;
    QWidget *orderWindow;
    QLineEdit* sizeLEdit;
private:
    Model* _model;
    Editor* _editor;
    void setupWindow();
    void drawTable();
    void refreshTable();
    int _size;
    int _steps;
    QList<int> _energyUsed;
    int _allEnergyUsed;

private slots:
    void editorButtonClicked();
    void startButtonClicked();
    void saveButtonClicked();
    void loadButtonClicked();
    void speedSliderChanged(int value);
    void editorApplyAndClose();
    void onTick();
    void onLoad();
    void onFinished();
    void newOrder();
    void closeButtonClicked();
    void confirmButtonClicked();
};

#endif // MAINWINDOW_H
