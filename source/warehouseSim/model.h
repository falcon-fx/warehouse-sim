#ifndef MODEL_H
#define MODEL_H

#include <QVector>


class Model
{
public:
    Model();
    void setSize(int h,int w){_height=h; _width=w; makeTable();}
    QVector<QVector<int > > getBoard() {return this->table;}
private:
    QVector<QVector<int>> table;
    int _height;
    int _width;

    void makeTable();
};

#endif // MODEL_H
