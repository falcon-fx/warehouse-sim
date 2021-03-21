#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QPoint>
#include <tuple>

enum State {
    GO_CHARGE,
    GO_POD,
    GO_DELIVER,
    RETURN_POD,
    WAIT
};

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

#endif // TILE_H
