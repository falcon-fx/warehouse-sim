#include "robot.h"

Robot::Robot(int id, int maxP) {
    this->setID(id);
    this->maxPower = maxP;
    this->power = this->maxPower;
}

void Robot::giveCommand(QQueue<QPoint> path, State s) {
    this->pathToFollow = path;
    this->state = s;
}
