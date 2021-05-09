#include "wtile.h"

WTile::WTile()
{
    this->dock = false;
    this->reserved = false;
    this->target = -1;
    this->type = EMPTY;
}

WTile::WTile(int field_type) {
    this->dock = false;
    this->reserved = false;
    this->target = -1;
    this->setType(field_type);
}

void WTile::setType(int field_type) {
    if(field_type == 0) {
        this->type = EMPTY;
    } else if(field_type == 1) {
        this->type = DOCK;
    } else if(field_type == 2) {
        this->type = POD;
    } else if(field_type == 3) {
        this->type = ROBOT;
    } else if(field_type == 4) {
        this->type = TARGET;
    }
}

void WTile::setTarget(int prodNum)
{
    this->target = prodNum;
}

QString WTile::getType() {
    if(this->type == DOCK) {
        return "dock";
    } else if(this->type == POD) {
        return "pod";
    } else if(this->type == ROBOT) {
        return "robot";
    } else if(this->type == TARGET) {
        return "target";
    }
    return "empty";
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

void WTile::setReserved(bool reserved)
{
    this->reserved = reserved;
}

bool WTile::isReserved()
{
    return this->reserved;
}
