#ifndef WTILE_H
#define WTILE_H

#include <QObject>
#include <QPoint>
#include <tuple>

class WTile : public QObject {
    Q_OBJECT
public:
    void setLocation(int x, int y);
    void setLocation(QPoint* p);
    QPoint* getLocation();
    void setID(int i);
    int getID();
    bool isEmptyTile();
    void setEmpty(bool isE);
protected:
    QPoint* location;
    bool isEmpty;
    int id;
};

#endif // WTILE_H
