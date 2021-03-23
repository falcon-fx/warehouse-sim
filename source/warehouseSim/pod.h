#ifndef POD_H
#define POD_H

#include <QPoint>
#include <QSet>

class Pod
{
public:
    Pod(QSet<int> prods, int origin_x, int origin_y);
    bool hasProduct(int prodNum);
    void addProduct(int prodNum);
    void removeProduct(int prodNum);
    QSet<int> getProducts();
    QPoint getPosition();
    void setPosition(QPoint pos);
    QPoint getOriginalPosition();
    void setPickedUp(bool picked);
    bool isPickedUp();
    void setReserved(bool reserved);
    bool isReserved();
private:
    QSet<int> products;
    QPoint pos;
    QPoint originalPos;
    bool pickedUp;
    bool reserved;
};

#endif // POD_H
