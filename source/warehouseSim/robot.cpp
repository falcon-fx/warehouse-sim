#include "robot.h"

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
}

int Robot::getID()
{
    return this->id;
}

int Robot::getPower()
{
    return this->power;
}

void Robot::setPower(int p)
{
    this->power = p;
}

void Robot::lowerPower()
{
    this->power--;
    this->usedPower++;
}

int Robot::getUsedPower()
{
    return this->usedPower;
}

Robot::Direction Robot::getDirection()
{
    return this->direction;
}

QPoint Robot::getPosition()
{
    return this->position;
}

QPoint Robot::getNextPosition()
{
    QPoint next = this->position;
    switch (direction)
    {
        case NORTH:
            next.ry()--;
            break;
        case EAST:
            next.rx()++;
            break;
        case SOUTH:
            next.ry()++;
            break;
        case WEST:
            next.rx()--;
            break;
    }
    return next;
}

void Robot::setPosition(QPoint pos)
{
    this->position.setX(pos.x());
    this->position.setY(pos.y());
}

void Robot::move()
{
    power--;
    switch (direction)
    {
        case NORTH:
            this->position.ry()--;
            break;
        case EAST:
            this->position.rx()++;
            break;
        case SOUTH:
            this->position.ry()++;
            break;
        case WEST:
            this->position.rx()--;
            break;
    }
    if (pod != nullptr)
    {
        pod->setPosition(this->position);
    }
}

void Robot::turnLeft()
{
    power--;
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

void Robot::turnRight()
{
    power--;
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

Pod* Robot::getPod()
{
    return this->pod;
}

void Robot::liftPod(Pod* pod)
{
    power--;
    pod->setPickedUp(true);
    this->pod = pod;
}

void Robot::dropPod()
{
    power--;
    this->pod->setPickedUp(false);
    this->pod = nullptr;
}

bool Robot::hasPod()
{
    return this->pod != nullptr;
}
