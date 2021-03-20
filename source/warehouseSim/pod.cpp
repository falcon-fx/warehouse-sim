#include "pod.h"

Pod::Pod(int id, QList<int> prods, int origin_x, int origin_y)
{
    this->setID(id);
    this->products = prods;
    this->pickedUp = false;
    this->originalPos = new QPoint(origin_x, origin_y);
}

bool Pod::isInProducts(int prodNum) {
    for(int i = 0; i < products.size(); i++) {
        if(products[i] == prodNum) {
            return true;
        }
    }
    return false;
}

void Pod::removeProduct(int prodNum) {
    if(this->isInProducts(prodNum)) {
        products.removeOne(prodNum);
    }
}

bool Pod::isPickedUp() {
    return this->pickedUp;
}

void Pod::pickUp() {
    this->pickedUp = true;
}

void Pod::putDown() {
    this->pickedUp = false;
}
