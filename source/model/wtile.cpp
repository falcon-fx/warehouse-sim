#include "wtile.h"

/*!
 * \brief WTile::WTile
 * Construct a new tile.
 */
WTile::WTile()
{
    this->dock = false;
    this->reserved = false;
    this->target = -1;
    this->type = EMPTY;
}

/*!
 * \brief WTile::WTile
 * Construct a new tile with the specified field type.
 * \param field_type The field type
 */
WTile::WTile(int field_type) {
    this->dock = false;
    this->reserved = false;
    this->target = -1;
    this->setType(field_type);
}

/*!
 * \brief WTile::setType
 * Set the type of the tile.
 * \param field_type The field type
 */
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

/*!
 * \brief WTile::setTarget
 * Set the target of the tile to the specified
 * product number.
 * \param prodNum The product number
 */
void WTile::setTarget(int prodNum)
{
    this->target = prodNum;
}

/*!
 * \brief WTile::getType
 * Get the type of the tile.
 * \return
 */
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

/*!
 * \brief WTile::clearTarget
 * Remove the target from the tile.
 */
void WTile::clearTarget()
{
    this->target = -1;
}

/*!
 * \brief WTile::isTarget
 * Check if the tile has a target.
 * \return Whether the tile is a target
 */
bool WTile::isTarget()
{
    return this->target != -1;
}

/*!
 * \brief WTile::getTarget
 * Get the target from the tile.
 * \return
 */
int WTile::getTarget()
{
    return this->target;
}

/*!
 * \brief WTile::isDock
 * Check if the tile is a dock.
 * \return Whether the tile is empty
 */
bool WTile::isDock()
{
    return this->dock;
}

/*!
 * \brief WTile::setDock
 * Set whether the tile has a dock.
 * \param dock Is the tile a dock
 */
void WTile::setDock(bool dock)
{
    this->dock = dock;
}

/*!
 * \brief WTile::isEmptyTile
 * Check if the tile is empty.
 * \return Whether the tile is empty
 */
bool WTile::isEmptyTile() {
    return this->isEmpty;
}

/*!
 * \brief WTile::setEmpty
 * Set whether the tile is empty.
 * \param empty Is the tile empty
 */
void WTile::setEmpty(bool empty) {
    this->isEmpty = empty;
}

/*!
 * \brief WTile::setReserved
 * Set whether the tile is reserved.
 * \param reserved Is the tile reserved
 */
void WTile::setReserved(bool reserved)
{
    this->reserved = reserved;
}

/*!
 * \brief WTile::isReserved
 * Get the reserve status of the tile.
 * \return Whether the tile is reserved
 */
bool WTile::isReserved()
{
    return this->reserved;
}
