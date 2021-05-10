#include "robot.h"

/*!
 * \brief Robot::Robot
 * Create a new robot with the specified ID and
 * maximum power at the x and y coordinates.
 * \param id The ID
 * \param maxP The maximum power
 * \param x The x coordinate
 * \param y The y coordinate
 */
Robot::Robot(int id, int maxP, int x, int y)
{
    this->id = id;
    this->power = maxP;
    this->maxPower = maxP;
    this->usedPower = 0;
    this->position.setX(x);
    this->position.setY(y);
    this->direction = NORTH;
    this->pod = nullptr;
    this->setProdNum(-1);
}

/*!
 * \brief Robot::getID
 * Get the ID of the robot.
 * \return
 */
int Robot::getID()
{
    return this->id;
}

/*!
 * \brief Robot::getPower
 * Get the available power of the robot.
 * \return
 */
int Robot::getPower()
{
    return this->power;
}

/*!
 * \brief Robot::setPower
 * Set the power of the robot to the specified
 * amount.
 * \param p The power amount
 */
void Robot::setPower(int p)
{
    this->power = p;
}

/*!
 * \brief Robot::lowerPower
 * Lower the power of the robot and increase the
 * used power counter.
 */
void Robot::lowerPower()
{
    this->power--;
    this->usedPower++;
}

/*!
 * \brief Robot::getUsedPower
 * Get the power used by the robot.
 * \return
 */
int Robot::getUsedPower()
{
    return this->usedPower;
}

/*!
 * \brief Robot::setUsedPower
 * Set the used power of the robot.
 * \param up The power used
 */
void Robot::setUsedPower(int up)
{
    this->usedPower = up;
}

/*!
 * \brief Robot::getDirection
 * Get the facing direction of the robot.
 * \return
 */
Robot::Direction Robot::getDirection()
{
    return this->direction;
}

/*!
 * \brief Robot::getPosition
 * Get the position of the robot.
 * \return
 */
QPoint Robot::getPosition()
{
    return this->position;
}

/*!
 * \brief Robot::setPosition
 * Set the position of the robot.
 * \param pos The position
 */
void Robot::setPosition(QPoint pos)
{
    this->position.setX(pos.x());
    this->position.setY(pos.y());
}


/*!
 * \brief Robot::move
 * Move the robot.
 */
void Robot::move()
{
    lowerPower();
    switch (direction)
    {
        case NORTH:
            this->position.rx()--;
            break;
        case EAST:
            this->position.ry()++;
            break;
        case SOUTH:
            this->position.rx()++;
            break;
        case WEST:
            this->position.ry()--;
            break;
    }
    if (pod != nullptr)
    {
        pod->setPosition(this->position);
    }
}

/*!
 * \brief Robot::turnLeft
 * Turn the robot left.
 */
void Robot::turnLeft()
{
    lowerPower();
    switch (direction)
    {
        case NORTH:
            direction = WEST;
            break;
        case WEST:
            direction = SOUTH;
            break;
        case SOUTH:
            direction = EAST;
            break;
        case EAST:
            direction = NORTH;
            break;
    }
}

/*!
 * \brief Robot::turnRight
 * Turn the robot right.
 */
void Robot::turnRight()
{
    lowerPower();
    switch (direction)
    {
        case NORTH:
            direction = EAST;
            break;
        case EAST:
            direction = SOUTH;
            break;
        case SOUTH:
            direction = WEST;
            break;
        case WEST:
            direction = NORTH;
            break;
    }
}

/*!
 * \brief Robot::getPod
 * Get the pod of the robot.
 * \return
 */
Pod* Robot::getPod()
{
    return this->pod;
}

/*!
 * \brief Robot::liftPod
 * Lift a pod in the warehouse.
 * \param pod The pod
 */
void Robot::liftPod(Pod* pod)
{
    lowerPower();
    pod->setPickedUp(true);
    this->pod = pod;
}

/*!
 * \brief Robot::dropPod
 * Lift a pod in the warehouse.
 * \param pod The pod
 */
void Robot::dropPod()
{
    lowerPower();
    this->pod->setPickedUp(false);
    this->pod = nullptr;
}

/*!
 * \brief Robot::hasPod
 * Check if the robot has a pod.
 * \return
 */
bool Robot::hasPod()
{
    return this->pod != nullptr;
}
