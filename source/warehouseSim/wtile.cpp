#include "wtile.h"

WTile::WTile()
{
    this->dock = false;
    this->target = -1;
}

void WTile::setTarget(int prodNum)
{
    this->target = prodNum;
}

void WTile::clearTarget()
{
    this->target = -1;
}

bool WTile::isTarget()
{
    return this->target != -1;
}

int WTile::getTarget()
{
    return this->target;
}

bool WTile::isDock()
{
    return this->dock;
}

void WTile::setDock(bool dock)
{
    this->dock = dock;
}

bool WTile::isEmptyTile() {
    return this->isEmpty;
}

void WTile::setEmpty(bool empty) {
    this->isEmpty = empty;
}
