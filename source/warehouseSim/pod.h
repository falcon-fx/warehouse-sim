#ifndef POD_H
#define POD_H

#include "wtile.h"
#include <QList>

class Pod : public WTile
{
    Q_OBJECT
public:
    Pod(int id, QList<int> prods, int origin_x, int origin_y);
    bool isInProducts(int prodNum);
    void removeProduct(int prodNum);
    bool isPickedUp();
    void pickUp();
    void putDown();
private:
    QList<int> products;
    bool pickedUp;
    QPoint* originalPos;
};

#endif // POD_H
