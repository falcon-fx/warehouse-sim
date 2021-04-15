#include "model.h"
#include <QDebug>
#include <QFile>

//CSAK TEST
Model::Model(int s, int maxP)
{
    this->size = s;
    this->maxPower = maxP;
    this->robotCount = 0;
    this->rowNum = {-1,0,0,1};
    this->colNum = {0,-1,1,0};
    makeWarehouse();
}

void Model::setSize(int s)
{
    this->size = s;
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
    for (int i = 0; i < size; i++)
    {
        QVector<WTile*> tmp;
        for (int j = 0; j < size; j++)
        {
            WTile* tile = new WTile(0);
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
    warehouse[y][x]->setType(3);
}

void Model::createPod(int x, int y, QSet<int> prods)
{
    pods.append(new Pod(prods, y, x));
    warehouse[y][x]->setType(2);
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
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
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
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
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
    if (!this->warehouse[y][x]->isDock()) {
        this->warehouse[y][x]->setTarget(prodNum);
        this->warehouse[y][x]->setType(4);
    }
}

void Model::createDock(int x, int y)
{
    if (!this->warehouse[y][x]->isTarget()) {
        this->warehouse[y][x]->setDock(true);
        this->warehouse[y][x]->setType(1);
    }
}

void Model::createOrder(int prodNum) {
    this->orders.append(prodNum);
}

QQueue<int> Model::getOrders()
{
    return this->orders;
}


int Model::getSize()
{
    return this->size;
}

void Model::tick()
{
    /*for (int i = 0; i < robots.count(); i++)
    {
        Robot* r = robots[i];

    }*/
    /*for(int i = 0; i < tasks.size(); i++) {
        QQueue<Task> t = tasks[i];
        qDebug() << "task " << i+1 << ": " << t.size();
        while(!t.isEmpty()) {
            Task task = t.dequeue();
            qDebug() << task.op << " " << task.weight;
        }
    }
    qDebug() << "number of orders: " << orders.size() << " number of tasks: " << tasks.size();
    for(int i = 0; i < robotCount; i++) {
        Robot* r = robots[i];
        if(orders.size() != 0) {
            int order = orders.dequeue();
            QPoint newDest = findClosestPod(r->getPosition(), order);
        }
    }*/
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            qDebug() << warehouse[i][j]->getType();
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
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
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

    file.write(QString::number(size).toUtf8() + "\n");
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
    int s;
    in >> s;
    this->setSize(s);
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

void Model::createPath(QPoint start, QPoint end, int &shortestPath, int &energyNeeded, QQueue<Task> &tasks, Weight weight, Robot* robot) {
    if (warehouse[start.y()][start.x()]->getType() != "empty" || warehouse[start.y()][start.x()]->getType() == "empty") {
            return;
    }

    QVector<QVector<bool>> visited;
    for(int i = 0; i < size; i++) {
        QVector<bool> tmp;
        for(int j = 0; j < size; j++) {
            tmp.push_back(false);
        }
        visited.push_back(tmp);
    }

    visited[start.y()][start.x()] = true;

    QQueue<Node*> q;

    Node* s = new Node(start, 0, nullptr);
    q.enqueue(s);
    while (!q.empty()) {
        Node* curr = q.front();
        QPoint loc = curr->location;

        if (loc.x() == end.x() && loc.y() == end.y()) {
            int robotpath = 0;
            energyNeeded = calculateEnergyNeeded(q.front(), robotpath);
            QVector<QPoint> tmpPath;
            createPathVector(q.front(), tmpPath);
            tasks = generatePathQueue(tmpPath, weight, robot);
            shortestPath = curr->distance;
            return;
        }

        qDebug() << "pointer: " << curr << curr->location.x() << ", y:" << curr->location.y() << ", dist: " << curr->distance << ", parent: " << curr->parent << "\n";

        q.dequeue();

        for (int i = 0; i < 4; i++) {
            int row = loc.x() + rowNum[i];
            int col = loc.y() + colNum[i];

            if ((isValid(row, col) && (weight == WGT_TO_POD && //is valid and going to pod
                (warehouse[col][row]->getType() == "empty" || warehouse[col][row]->getType() == "pod" || warehouse[col][row]->getType() == "target" || warehouse[col][row]->getType() == "dock")) && //can go on empty, pod, target or dock squares
                !visited[col][row]) || //not yet visited
                (isValid(row, col) && ((weight == WGT_POD_TO_TARGET || weight == WGT_POD_TO_ORIGIN) && //is valid and carrying a pod
                (warehouse[col][row]->getType() == "empty" || warehouse[col][row]->getType() == "target" || warehouse[col][row]->getType() == "dock")) && //can go on empty, target or dock squares
                !visited[col][row]) ||
                (isValid(row, col) && (weight == WGT_CHARGE && //is valid and going to charge
                (warehouse[col][row]->getType() == "empty" || warehouse[col][row]->getType() == "pod" || warehouse[col][row]->getType() == "target" || warehouse[col][row]->getType() == "dock")) && //can go on empty, pod, target or dock squares
                !visited[col][row])) {
                visited[col][row] = true;
                Node* Adjcell = new Node({row, col},curr->distance + 1, curr);
                q.enqueue(Adjcell);
            }
        }
    }
}

int Model::calculateEnergyNeeded(Node *n, int &energy) {
    if(n == nullptr) {
        return energy;
    } else {
        if(n->parent->parent == nullptr || n->parent == nullptr) {
            energy++;
            return energy;
        } else if(((n->location.x() != n->parent->location.y() && n->location.x() == n->parent->location.x())
            && (n->parent->location.y() == n->parent->parent->location.y() && n->parent->location.x() != n->parent->parent->location.x()))
            || ((n->location.y() == n->parent->location.y() && n->location.x() != n->parent->location.x())
            && (n->parent->location.y() != n->parent->parent->location.y() && n->parent->location.x() == n->parent->parent->location.x()))) {
            //turn ahead
            energy += 2;
            calculateEnergyNeeded(n->parent, energy);
        } else if((n->parent->location.x() != n->location.x() && n->parent->location.y() == n->location.y())
               || (n->parent->location.y() != n->location.y() && n->parent->location.x() == n->location.x())) {
            //no turn ahead
            energy++;
            calculateEnergyNeeded(n->parent, energy);
        }
    }
}

void Model::createPathVector(Node *n, QVector<QPoint> path) {
    if(n == nullptr) {
        return;
    }
    createPathVector(n->parent, path);
    path.append(n->location);
}

QQueue<Task> Model::generatePathQueue(QVector<QPoint> path, Weight w, Robot *r) {
    //todo: figure out a way to convert the path vector to operations
}

bool Model::isValid(int row, int col) {
    return (row >= 0) && (row < size) &&
           (col >= 0) && (col < size);
}
