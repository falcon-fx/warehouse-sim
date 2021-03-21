#include "tile.h"

void Tile::setLocation(int x, int y) {
    this->location = new QPoint(x,y);
}

void Tile::setLocation(QPoint* p) {
    int x = p->x();
    int y = p->y();
    this->location = new QPoint(x,y);
}

QPoint* Tile::getLocation() {
    return this->location;
}

void Tile::setID(int i) {
    this->id = i;
}

int Tile::getID() {
    return this->id;
}

bool Tile::isEmptyTile() {
    return this->isEmpty;
}

void Tile::setEmpty(bool isE) {
    this->isEmpty = isE;
}
