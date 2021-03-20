#include "wtile.h"

void WTile::setLocation(int x, int y) {
    this->location = new QPoint(x,y);
}

void WTile::setLocation(QPoint* p) {
    int x = p->x();
    int y = p->y();
    this->location = new QPoint(x,y);
}

QPoint* WTile::getLocation() {
    return this->location;
}

void WTile::setID(int i) {
    this->id = i;
}

int WTile::getID() {
    return this->id;
}

bool WTile::isEmptyTile() {
    return this->isEmpty;
}

void WTile::setEmpty(bool isE) {
    this->isEmpty = isE;
}
