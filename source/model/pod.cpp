#include "pod.h"

Pod::Pod(QSet<int> prods, int origin_x, int origin_y)
{
    this->products = prods;
    this->pos.setX(origin_x);
    this->pos.setY(origin_y);
    this->originalPos.setX(origin_x);
    this->originalPos.setY(origin_y);
    this->pickedUp = false;
    this->reserved = false;
}

bool Pod::hasProduct(int prodNum)
{
    return this->products.contains(prodNum);
}

void Pod::addProduct(int prodNum)
{
    this->products.insert(prodNum);
}

void Pod::removeProduct(int prodNum)
{
    this->products.remove(prodNum);
}

QSet<int> Pod::getProducts()
{
    return this->products;
}

QPoint Pod::getPosition()
{
    return this->pos;
}

void Pod::setPosition(QPoint pos)
{
    this->pos.setX(pos.x());
    this->pos.setY(pos.y());
}

QPoint Pod::getOriginalPosition()
{
    return this->originalPos;
}

void Pod::setPickedUp(bool picked)
{
    this->pickedUp = picked;
}

bool Pod::isPickedUp()
{
    return this->pickedUp;
}

void Pod::setReserved(bool reserved)
{
    this->reserved = reserved;
}

bool Pod::isReserved()
{
    return this->reserved;
}
