#include "model.h"
#include <QDebug>

//CSAK TEST
Model::Model()
{
}

void Model::makeTable()
{
    //qDebug() << table.size();
    table.clear();
    QVector<int> temp;

    for (int i=0; i<_height; i++)
    {
        for (int j=0; j<_width; j++)
        {
                temp.append(0);
        }
        table.append(temp);
        temp.clear();
        //qDebug() <<i;
    }
    table[_height-1][_width-1] = 2;
    //qDebug() << table.size();
}

