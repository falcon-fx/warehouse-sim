#include "model.h"
#include <QDebug>
#include <QFile>

//CSAK TEST
Model::Model(int w, int h, int maxP)
{
    this->width = w;
    this->height = h;
    this->maxPower = maxP;
    this->robotCount = 0;
    makeWarehouse();
}

void Model::setSize(int w, int h)
{
    this->width = w;
    this->height = h;
    makeWarehouse();
}

QVector<QVector<WTile*>> Model::getWarehouse()
{
    return this->warehouse;
}

void Model::makeWarehouse()
{
    robots.clear();
    pods.clear();
    tasks.clear();
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
    //warehouse[y][x]->setEmpty(false);
}

void Model::createPod(int x, int y, QSet<int> prods)
{
    pods.append(new Pod(prods, y, x));
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

Pod* Model::getPod(int x, int y)
{
    for (int i = 0; i < pods.count(); i++)
    {
        if (pods[i]->getPosition().x() == x && pods[i]->getPosition().y() == y)
            return pods[i];
    }
    return nullptr;
}

QList<QPair<QPoint, int>> Model::getTargets()
{
    QList<QPair<QPoint, int>> list;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (warehouse[i][j]->isTarget())
            {
                QPoint point(j, i);
                QPair<QPoint, int> pair(point, warehouse[i][j]->getTarget());
                list.append(pair);
            }
        }
    }
    return list;
}

QList<QPoint> Model::getDocks()
{
    QList<QPoint> list;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (warehouse[i][j]->isDock())
            {
                QPoint point(j, i);
                list.append(point);
            }
        }
    }
    return list;
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

int Model::getWidth()
{
    return this->width;
}

int Model::getHeight()
{
    return this->height;
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
    emit onTick();
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

void Model::save(QString filename)
{
    QFile file(filename);

    if (!file.open(QFile::WriteOnly))
    {
        qDebug() << "Failed to open file" << filename;
        return;
    }

    file.write(QString::number(width).toUtf8() + " " + QString::number(height).toUtf8() + "\n");
    file.write(QString::number(robots.count()).toUtf8() + "\n");
    for (int i = 0; i < robots.count(); i++)
    {
        file.write(QString::number(robots[i]->getPosition().x()).toUtf8() + " ");
        file.write(QString::number(robots[i]->getPosition().y()).toUtf8() + " ");
        file.write(QString::number(robots[i]->getPower()).toUtf8() + " ");
        file.write(QString::number(robots[i]->getUsedPower()).toUtf8() + "\n");
    }
    file.write(QString::number(pods.count()).toUtf8() + "\n");
    for (int i = 0; i < pods.count(); i++)
    {
        file.write(QString::number(pods[i]->getPosition().x()).toUtf8() + " ");
        file.write(QString::number(pods[i]->getPosition().y()).toUtf8() + " ");
        file.write(QString::number(pods[i]->getProducts().count()).toUtf8() + " ");
        foreach (const int &v, pods[i]->getProducts())
        {
            file.write(QString::number(v).toUtf8() + " ");
        }
        file.write("\n");
    }
    QList<QPair<QPoint, int>> targets = getTargets();
    file.write(QString::number(targets.count()).toUtf8() + "\n");
    for (int i = 0; i < targets.count(); i++)
    {
        file.write(QString::number(targets[i].first.x()).toUtf8() + " ");
        file.write(QString::number(targets[i].first.y()).toUtf8() + " ");
        file.write(QString::number(targets[i].second).toUtf8() + "\n");
    }
    QList<QPoint> docks = getDocks();
    file.write(QString::number(docks.count()).toUtf8() + "\n");
    for (int i = 0; i < docks.count(); i++)
    {
        file.write(QString::number(docks[i].x()).toUtf8() + " ");
        file.write(QString::number(docks[i].y()).toUtf8() + "\n");
    }
    file.close();
}

void Model::load(QString filename)
{
    QFile file(filename);

    if (!file.open(QFile::ReadOnly))
    {
        qDebug() << "Failed to open file" << filename;
        return;
    }

    QTextStream in(&file);
    int w, h;
    in >> w >> h;
    this->setSize(w, h);
    int rc;
    in >> rc;
    for (int i = 0; i < rc; i++)
    {
        int x, y, pwr, upwr;
        in >> x >> y >> pwr >> upwr;
        createRobot(x, y);
        QPoint p(x, y);
        robots[i]->setPosition(p);
        robots[i]->setPower(pwr);
        robots[i]->setUsedPower(upwr);
    }
    int pc;
    in >> pc;
    for (int i = 0; i < pc; i++)
    {
        int x, y, prc;
        in >> x >> y >> prc;
        QSet<int> pr;
        for (int j = 0; j < prc; j++)
        {
            int prn;
            in >> prn;
            pr.insert(prn);
        }
        createPod(y, x, pr);
    }
    int tc;
    in >> tc;
    for (int i = 0; i < tc; i++)
    {
        int x, y, tn;
        in >> x >> y >> tn;
        createTarget(x, y, tn);
    }
    int dc;
    in >> dc;
    for (int i = 0; i < dc; i++)
    {
        int x, y;
        in >> x >> y;
        createDock(x, y);
    }
    emit onLoad();
}
