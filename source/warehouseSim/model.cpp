#include "model.h"
#include <QDebug>

//CSAK TEST
Model::Model(int w, int h, int maxP)
{
    this->width = w;
    this->height = h;
    this->maxPower = maxP;
    this->robotCount = 0;
    makeWarehouse();
}

QVector<QVector<WTile*>> Model::getWarehouse()
{
    return this->warehouse;
}

void Model::makeWarehouse()
{
    warehouse.clear();
    for (int i = 0; i < width; i++)
    {
        QVector<WTile*> tmp;
        for (int j = 0; j < height; j++)
        {
            WTile* tile = new WTile();
            tmp.append(tile);
        }
        warehouse.append(tmp);
    }
}

void Model::createRobot(int x, int y)
{
    robots.append(new Robot(robotCount++, this->maxPower, y, x));
    QQueue<Task> task_list;
    tasks.append(task_list);
    warehouse[y][x]->setEmpty(false);
}

void Model::createPod(int x, int y, QSet<int> prods)
{
    pods.append(new Pod(prods, x, y));
}

QList<Robot*> Model::getRobots()
{
    return this->robots;
}

Robot* Model::getRobot(int x, int y)
{
    for (int i = 0; i < robots.count(); i++)
    {
        if (robots[i]->getPosition().x() == x && robots[i]->getPosition().y() == y)
            return robots[i];
    }
    return nullptr;
}

QList<Pod*> Model::getPods()
{
    return this->pods;
}

void Model::createTarget(int x, int y, int prodNum)
{
    if (!this->warehouse[y][x]->isDock())
        this->warehouse[y][x]->setTarget(prodNum);
}

void Model::createDock(int x, int y)
{
    if (!this->warehouse[y][x]->isTarget())
        this->warehouse[y][x]->setDock(true);
}

QQueue<int> Model::getOrders()
{
    return this->orders;
}

void Model::tick()
{
    for (int i = 0; i < robots.count(); i++)
    {
        executeTask(i);
        Robot* r = robots[i];
        if (!orders.empty())
        {
            if (tasks[i].empty())
            {
                QPoint dest = findClosestTarget(r->getPosition(), orders.dequeue());
                if ((dest - r->getPosition()).manhattanLength() + robotCount < r->getPower())
                {
                    // todo: feladatkiosztás
                }
            }
        }
    }
}

void Model::executeTask(int id)
{
    if (!tasks[id].empty())
    {
        Operation op = tasks[id].dequeue().op;
        Robot* r = robots[id];
        switch (op)
        {
            case OP_MOVE:
                r->move();
                break;
            case OP_TURN_LEFT:
                r->turnLeft();
                break;
            case OP_TURN_RIGHT:
                r->turnRight();
                break;
            case OP_LIFT:
                for (int i = 0; i < pods.count(); i++)
                {
                    if (pods[i]->getPosition() == r->getPosition())
                    {
                        r->liftPod(pods[i]);
                        break;
                    }
                }
                break;
            case OP_DROP:
                r->dropPod();
                break;
            case OP_CHARGE_STOP:
                r->setPower(maxPower);
            case OP_CHARGE:
            case OP_WAIT:
                break;
        }
    }
}

QPoint Model::findClosestTarget(QPoint pos, int prodNum)
{
    int distance = -1;
    QPoint closest;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (warehouse[i][j]->isTarget() && warehouse[i][j]->getTarget() == prodNum)
            {
                QPoint p(i, j);
                if (distance == -1 || ((pos - p).manhattanLength() < distance))
                {
                    closest.setX(p.x());
                    closest.setY(p.y());
                    distance = (pos - closest).manhattanLength();
                }
            }
        }
    }
    return closest;
}

bool Model::isPodAvailable(int prodNum)
{
    for (int i = 0; i < pods.size(); i++)
    {
        if (pods[i]->hasProduct(prodNum) && !pods[i]->isPickedUp())
            return true;
    }
    return false;
}

QPoint Model::findClosestPod(QPoint pos, int prodNum)
{
    int distance = -1;
    QPoint closest;
    for (int i = 0; i < pods.size(); i++)
    {
        if (pods[i]->hasProduct(prodNum) && !pods[i]->isPickedUp())
        {
            if (distance == -1 || ((pos - pods[i]->getPosition()).manhattanLength() < distance))
            {
                closest.setX(pods[i]->getPosition().x());
                closest.setY(pods[i]->getPosition().x());
                distance = (pos - pods[i]->getPosition()).manhattanLength();
            }
        }
    }
    return closest;
}
