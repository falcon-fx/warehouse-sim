#ifndef ROBOT_H
#define ROBOT_H

#include <model/pod.h>
#include <QPoint>
#include <QQueue>

class Robot
{
public:
    enum Direction
    {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };
    Robot(int id, int maxP, int x, int y);
    int getID();
    int getPower();
    void setPower(int p);
    void lowerPower();
    int getUsedPower();
    void setUsedPower(int up);
    Direction getDirection();
    QPoint getPosition();
    QPoint getNextPosition();
    void setPosition(QPoint pos);
    int getProdNum() { return this->prodNum; }
    void setProdNum(int prod) { this->prodNum = prod; }

    void move();
    void turnLeft();
    void turnRight();

    Pod* getPod();
    void liftPod(Pod* pod);
    void dropPod();
    bool hasPod();
    bool isUnfinished() { return this->unfinished; }
    void setUnfinished(bool uf);
    bool isFinished() { return this->finished; }
    void setFinished(bool f);
    void setTempPodPos(QPoint point) { this->tempPodPos = point; }
    QPoint getTempPodPos() { return this->tempPodPos; }
private:
    int id;
    int power;
    int maxPower;
    int usedPower;
    int prodNum;
    Direction direction;
    QPoint position;
    Pod* pod;
    bool unfinished;
    bool finished;
    QPoint tempPodPos;
};

#endif // ROBOT_H
