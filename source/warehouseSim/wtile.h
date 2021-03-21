#ifndef WTILE_H
#define WTILE_H

#include <QObject>
#include <QPoint>

class WTile : public QObject {
    Q_OBJECT
public:
    WTile();
    void setTarget(int prodNum);
    void clearTarget();
    bool isTarget();
    int getTarget();
    bool isDock();
    void setDock(bool dock);

    bool isEmptyTile();
    void setEmpty(bool empty);
protected:
    QPoint* location;
    int target;
    bool dock;
    bool isEmpty;
};

#endif // WTILE_H
