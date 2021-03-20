#ifndef ROBOT_H
#define ROBOT_H

#include "wtile.h"
#include <QQueue>

class Robot : public WTile
{
    Q_OBJECT
public:
    Robot(int id, int maxP);
    void giveCommand(QQueue<QPoint> path, State s);
private:
    int power;
    int maxPower;
    bool hasPod;
    int podNum;
    QQueue<QPoint> pathToFollow;
    State state;
};

#endif // ROBOT_H
