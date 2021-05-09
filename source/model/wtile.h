#ifndef WTILE_H
#define WTILE_H

#include <QObject>
#include <QPoint>
enum Type {
    EMPTY = 0,
    DOCK = 1,
    POD = 2,
    ROBOT = 3,
    TARGET = 4
};

class WTile : public QObject {
    Q_OBJECT
public:
    WTile();
    WTile(int field_type);
    void setTarget(int prodNum);
    void clearTarget();
    bool isTarget();
    int getTarget();
    bool isDock();
    void setDock(bool dock);

    bool isEmptyTile();
    void setEmpty(bool empty);
    void setType(int field_type);
    void setReserved(bool res);
    bool isReserved() { return this->reserved; }
    QString getType();
protected:
    Type type;
    QPoint* location;
    int target;
    bool dock;
    bool isEmpty;
    bool reserved;
};

#endif // WTILE_H
