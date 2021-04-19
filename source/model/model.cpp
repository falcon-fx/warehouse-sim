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
    tasksDone = 0;
    robotCount = 0;
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
    foreach (const int &p, prods)
        if (!products.contains(p))
            products.append(p);
    warehouse[y][x]->setType(2);
}

void Model::createPod(int x, int y, int ox, int oy, QSet<int> prods)
{
    pods.append(new Pod(prods, y, x, oy, ox));
    foreach (const int &p, prods)
        if (!products.contains(p))
            products.append(p);
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

QVector<int> Model::getProducts()
{
    return this->products;
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
    /*for(int i = 0; i < robotCount; i++) {
        Robot* r = robots[i];
        int energyNeeded = 0;
        int shortestPath = 0;
        if(tasks[i].isEmpty() && !r->hasPod() && !orders.isEmpty() ) {
            int order = orders.dequeue();
            QPoint podDest = findClosestPod(r->getPosition(), order);
            qDebug() << order << "robot:" << i;
            //QPoint dest(11,8);
            QPoint targetDest = findClosestTarget(podDest, order);
            r->setProdNum(order);
            createPath(r->getPosition(), podDest, shortestPath, energyNeeded, tasks[i], WGT_TO_POD, r);
            createPath(podDest, targetDest, shortestPath, energyNeeded, tasks[i], WGT_POD_TO_TARGET, r);
            if(energyNeeded > r->getPower()) {
                tasks[i].clear();
                orders.push_front(order);
                shortestPath = 0;
                energyNeeded = 0;
                createPath(r->getPosition(), findClosestDock(r->getPosition()), shortestPath, energyNeeded, tasks[i], WGT_CHARGE, r);
            }
        } else if(tasks[i].isEmpty() && r->hasPod() && r->getProdNum() != -1) {
            int shortestPath = 0;
            QPoint dest = findClosestTarget(r->getPosition(), r->getProdNum());
            createPath(r->getPosition(), dest, shortestPath, energyNeeded, tasks[i], WGT_POD_TO_TARGET, r);

            if(energyNeeded > r->getPower()) {
                tasks[i].clear();
                dest = findClosestDock(r->getPosition());
                shortestPath = 0;
                energyNeeded = 0;
                createPath(r->getPosition(), dest, shortestPath, energyNeeded, tasks[i], WGT_CHARGE, r);
            }
        } else if(tasks[i].isEmpty() && r->hasPod() && r->getProdNum() == -1) {
            int shortestPath = 0;
            QQueue<Task> tempTasks;
            QPoint dest = r->getPod()->getOriginalPosition();
            createPath(r->getPosition(), dest, shortestPath, energyNeeded, tempTasks, WGT_POD_TO_ORIGIN, r);
            if(energyNeeded > r->getPower()) {
                tempTasks.clear();
                dest = findClosestDock(r->getPosition());
                shortestPath = 0;
                energyNeeded = 0;
                createPath(r->getPosition(), dest, shortestPath, energyNeeded, tempTasks, WGT_CHARGE, r);
            }
        }
        executeTask(i);
    }*/
    for(int i = 0; i < robotCount; i++) {
        qDebug() << "robot #" << i << ", orders size:" << orders.size() << ", tasks[i].size:" << tasks[i].size() << ", has pod:" << robots[i]->hasPod() << ", prodnum:" << robots[i]->getProdNum();
        if(!orders.isEmpty() && tasks[i].isEmpty() && !robots[i]->hasPod() && robots[i]->getProdNum() == -1) {
            int order = orders.dequeue();
            robots[i]->setProdNum(order);
            qDebug() << "robot #" << i << "gets product #" << order;
        }
    }
    for(int i = 0; i < robotCount; i++) {
        Robot* r = robots[i];
        if(r->getProdNum() != -1 && !r->hasPod() && tasks[i].isEmpty()) {
            //qDebug() << "robot #" << i << ", product:" << r->getProdNum() << ", haspod:" << r->hasPod();
            gotoPod(r, i);
        } else if(r->getProdNum() != -1 && r->hasPod() && r->getPod()->getOriginalPosition() == r->getPosition() && tasks[i].isEmpty()) {
            gotoTarget(r, i);
        } else if(r->getProdNum() == -1 && r->hasPod() && r->getPod()->getOriginalPosition() != r->getPosition() && tasks[i].isEmpty()) {
            bringBackPod(r, i);
        } else if(r->getProdNum() != -1 && !r->hasPod() && tasks[i].isEmpty() && r->isUnfinished()) {
            gotoPodUnfinished(r, i);
        } else if(r->getProdNum() != -1 && r->hasPod() && tasks[i].isEmpty() && !r->isFinished()) {
            bringBackPod(r, i);
        }
        executeTask(i);
    }
    emit onTick();
    bool allEmpty = orders.isEmpty();
    for (int i = 0; i < robots.size() && allEmpty; i++) {
        allEmpty = allEmpty && robots[i]->getProdNum() == -1;
    }
    for(int i = 0; i < tasks.size() && allEmpty; i++) {
        allEmpty = allEmpty && tasks[i].isEmpty();
    }
    for(int i = 0; i < pods.size() && allEmpty; i++) {
        allEmpty = allEmpty && pods[i]->getOriginalPosition() == pods[i]->getPosition();
    }
    if(allEmpty) {
        emit onFinished();
    }
}

void Model::gotoPodUnfinished(Robot* robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestPod = robot->getTempPodPos();
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestPod, path, energyNeeded, tasks[robotID], WGT_TO_POD, robot);
    qDebug() << "robot #" << robotID << "position:" << robot->getPosition() << ", closest pod:" << closestPod;
    robot->setUnfinished(false);
    qDebug() << "robot #" << robotID << "goes to pod";
}

void Model::gotoDock(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestDock = findClosestDock(robot->getPosition());
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestDock, path, energyNeeded, tasks[robotID], WGT_CHARGE, robot);
    qDebug() << "robot #" << robotID << "position:" << robot->getPosition() << ", closest dock:" << closestDock;
    if(energyNeeded > robot->getPower()) {
        //cry();
        qDebug() << "robot #" << robotID << "is dead. rip";
    }
    qDebug() << "robot #" << robotID << "goes to dock";
}

void Model::gotoPod(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestPod = findClosestPod(robot->getPosition(), robot->getProdNum());
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestPod, path, energyNeeded, tasks[robotID], WGT_TO_POD, robot);
    qDebug() << "robot #" << robotID << "position:" << robot->getPosition() << ", closest pod:" << closestPod;
    if(energyNeeded > robot->getPower()) {
        orders.push_front(robot->getProdNum());
        robot->setProdNum(-1);
        gotoDock(robot, robotID);
    } else {
        robot->setUnfinished(false);
        robot->setFinished(true);
    }
    qDebug() << "robot #" << robotID << "goes to pod";
}

void Model::gotoTarget(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestTarget = findClosestTarget(robot->getPosition(), robot->getProdNum());
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestTarget, path, energyNeeded, tasks[robotID], WGT_POD_TO_TARGET, robot);
    qDebug() << "robot #" << robotID << "position:" << robot->getPosition() << ", closest target:" << closestTarget;
    if(energyNeeded > robot->getPower()) {
        orders.push_front(robot->getProdNum());
        robot->setProdNum(-1);
        gotoDock(robot, robotID);
    } else {
        robot->setUnfinished(false);
        robot->setFinished(true);
    }
    qDebug() << "robot #" << robotID << "goes to target";
}

void Model::bringBackPod(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint origin = robot->getPod()->getOriginalPosition();
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), origin, path, energyNeeded, tasks[robotID], WGT_POD_TO_ORIGIN, robot);
    qDebug() << "robot #" << robotID << "position:" << robot->getPosition() << ", origin:" << origin;
    if(energyNeeded > robot->getPower()) {
        robot->setUnfinished(true);
        robot->setFinished(false);
        robot->setTempPodPos(robot->getPod()->getPosition());
        robot->dropPod();
        gotoDock(robot, robotID);
    } else {
        robot->setFinished(true);
        tasksDone++;
    }
    qDebug() << "robot #" << robotID << "brings back pod";
}

void Model::executeTask(int id)
{
    if (!tasks[id].empty())
    {
        Operation op = tasks[id].dequeue().op;
        Robot* r = robots[id];
        switch (op)
        {
            case OP_MOVE: {
                int tmpx = r->getPosition().x();
                int tmpy = r->getPosition().y();
                r->move();
                qDebug() << "robot id:" << id << "temp x:" << tmpx << "temp y:" << tmpy << "previous tile type:" << warehouse[tmpx][tmpy]->getType();
                break;
            }
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
                        warehouse[pods[i]->getPosition().x()][pods[i]->getPosition().y()]->setType(0);
                        break;
                    }
                }
                break;
            case OP_DROP:
                warehouse[r->getPod()->getPosition().y()][r->getPod()->getPosition().x()]->setType(2);
                r->dropPod();
                break;
            case OP_DELIVER:
                r->setProdNum(-1);
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
            //qDebug() << "podfind: " << distance << pos.manhattanLength() << pos.x() << pos.y() << "pod: " << pods[i]->getPosition().manhattanLength() << pods[i]->getPosition().x() << pods[i]->getPosition().y();
            if (distance == -1 || ((pos - pods[i]->getPosition()).manhattanLength() < distance))
            {
                closest.setX(pods[i]->getPosition().x());
                closest.setY(pods[i]->getPosition().y());
                distance = (pos - pods[i]->getPosition()).manhattanLength();
            }
        }
    }
    return closest;
}

QPoint Model::findClosestDock(QPoint pos)
{
    int distance = -1;
    QPoint closest;
    for (int i = 0; i < getDocks().size(); i++)
    {
        if (true) //meg  kene nezni hogy van e a dockon robot
        {
            if (distance == -1 || ((pos - getDocks()[i]).manhattanLength() < distance))
            {
                closest.setX(getDocks()[i].y());
                closest.setY(getDocks()[i].x());
                distance = (pos - pods[i]->getPosition()).manhattanLength();
            }
        }
    }
    //qDebug() << closest;
    return closest;

}
void Model::save(QString filename)
{
    /*
        Formátum:
        [pálya mérete]
        [robotok száma]
          [x] [y] [energia] [elhasznált energia]
        [podok száma]
          [x] [y] [eredeti x] [eredeti y] [áruk száma] {áruk}
        [célállomások száma]
          [x] [y] [áru id]
        [töltők száma]
          [x] [y]
        [rendelések száma]
          [id]
    */
    QFile file(filename);

    if (!file.open(QFile::WriteOnly))
    {
        qDebug() << "Failed to open file" << filename;
        return;
    }

    file.write(QString::number(size).toUtf8() + "\n"); // a pálya mérete
    file.write(QString::number(robots.count()).toUtf8() + "\n"); // robotok száma
    for (int i = 0; i < robots.count(); i++)
    {
        file.write(QString::number(robots[i]->getPosition().x()).toUtf8() + " "); //
        file.write(QString::number(robots[i]->getPosition().y()).toUtf8() + " "); // robot pozíciója
        file.write(QString::number(robots[i]->getPower()).toUtf8() + " "); // robot mentéskori energiája
        file.write(QString::number(robots[i]->getUsedPower()).toUtf8() + "\n"); // robot elhasznált energiája
    }
    file.write(QString::number(pods.count()).toUtf8() + "\n"); // podok száma
    for (int i = 0; i < pods.count(); i++)
    {
        file.write(QString::number(pods[i]->getPosition().x()).toUtf8() + " "); //
        file.write(QString::number(pods[i]->getPosition().y()).toUtf8() + " "); // pod pozíciója
        file.write(QString::number(pods[i]->getOriginalPosition().x()).toUtf8() + " "); //
        file.write(QString::number(pods[i]->getOriginalPosition().y()).toUtf8() + " "); // pod eredeti pozíciója
        file.write(QString::number(pods[i]->getProducts().count()).toUtf8() + " "); // pod áruk száma
        foreach (const int &v, pods[i]->getProducts())
        {
            file.write(QString::number(v).toUtf8() + " "); // áruk száma
        }
        file.write("\n");
    }
    QList<QPair<QPoint, int>> targets = getTargets();
    file.write(QString::number(targets.count()).toUtf8() + "\n"); // célállomások száma
    for (int i = 0; i < targets.count(); i++)
    {
        file.write(QString::number(targets[i].first.x()).toUtf8() + " "); //
        file.write(QString::number(targets[i].first.y()).toUtf8() + " "); // célállomás pozíciója
        file.write(QString::number(targets[i].second).toUtf8() + "\n");
    }
    QList<QPoint> docks = getDocks();
    file.write(QString::number(docks.count()).toUtf8() + "\n"); // töltők száma
    for (int i = 0; i < docks.count(); i++)
    {
        file.write(QString::number(docks[i].x()).toUtf8() + " ");  //
        file.write(QString::number(docks[i].y()).toUtf8() + "\n"); // töltő pozíciója
    }
    file.close();
    file.write(QString::number(orders.count()).toUtf8() + "\n"); // rendelések száma
    for (int i = 0; i < orders.count(); i++)
    {
        file.write(QString::number(orders[i]).toUtf8() + "\n"); // áru id
    }
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
    int rc; // robotok száma
    in >> rc;
    for (int i = 0; i < rc; i++)
    {
        int x, y, pwr, upwr; // x, y, energia, elhasznált energia
        in >> x >> y >> pwr >> upwr;
        createRobot(x, y);
        QPoint p(x, y);
        robots[i]->setPosition(p);
        robots[i]->setPower(pwr);
        robots[i]->setUsedPower(upwr);
    }
    int pc; // podok száma
    in >> pc;
    for (int i = 0; i < pc; i++)
    {
        int x, y, ox, oy, prc; // x, y, eredeti x, eredeti y, áruk száma
        in >> x >> y >> ox >> oy >> prc;
        QSet<int> pr;
        for (int j = 0; j < prc; j++)
        {
            int prn; // áru
            in >> prn;
            pr.insert(prn);
        }
        createPod(y, x, oy, ox, pr);
    }
    int tc; // célállomások száma
    in >> tc;
    for (int i = 0; i < tc; i++)
    {
        int x, y, tn; // x, y, áru id
        in >> x >> y >> tn;
        createTarget(x, y, tn);
    }
    int dc; // töltők száma
    in >> dc;
    for (int i = 0; i < dc; i++)
    {
        int x, y; // x, y
        in >> x >> y;
        createDock(x, y);
    }
    int oc; // rendelések száma
    in >> oc;
    for (int i = 0; i < oc; i++)
    {
        int id; // áru
        in >> id;
        orders.append(id);
    }
    emit onLoad();
}

void Model::createPath(QPoint start, QPoint end, int &shortestPath, int &energyNeeded, QQueue<Task> &tasks, Weight weight, Robot* robot) {
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
            //int robotpath = 0;
            QVector<QPoint> tmpPath;
            createPathVector(q.front(), tmpPath);
            for(int i = 0; i < tmpPath.size(); i++) {
                //qDebug() << "x:" << tmpPath[i].x() << "y:" << tmpPath[i].y();
            }
            //qDebug() << "pathfinding: path vector" << tmpPath.size();
            tasks.append(generatePathQueue(tmpPath, weight, robot));
            for(int i = 0; i < tasks.size(); i++) {
                //qDebug() << "path queue:" << tasks[i].op << ", weight:" << tasks[i].weight;
            }
            energyNeeded += tasks.size();
            Task tmp;
            tmp.weight = weight;
            switch(weight) {
                case WGT_POD_TO_TARGET:
                    tmp.op = OP_DELIVER;
                    tasks.append(tmp);
                    break;
                case WGT_POD_TO_ORIGIN:
                    tmp.op = OP_DROP;
                    tasks.append(tmp);
                    energyNeeded++;
                    break;
                case WGT_TO_POD:
                    tmp.op = OP_LIFT;
                    tasks.append(tmp);
                    energyNeeded++;
                    break;
                case WGT_CHARGE:
                    tmp.op = OP_CHARGE;
                    tasks.append(tmp);
                    tasks.append(tmp);
                    tasks.append(tmp);
                    tasks.append(tmp);
                    tmp.op = OP_CHARGE_STOP;
                    tasks.append(tmp);
                    break;
            }

            shortestPath = curr->distance;
            return;
        }

        //qDebug() << "pointer: " << curr << curr->location.x() << ", y:" << curr->location.y() << ", dist: " << curr->distance << ", parent: " << curr->parent << "\n";

        q.dequeue();

        for (int i = 0; i < 4; i++) {
            int row = loc.x() + rowNum[i];
            int col = loc.y() + colNum[i];
            if((isValid(row, col) && (weight == WGT_TO_POD && //is valid and going to pod
                (warehouse[row][col]->getType() == "empty" || warehouse[row][col]->getType() == "pod" || warehouse[row][col]->getType() == "target" || warehouse[row][col]->getType() == "dock")) && //can go on empty, pod, target or dock squares
                !visited[col][row])) {
                //qDebug() << warehouse[row][col]->getType() << row << col << "to pod";
                visited[col][row] = true;
                Node* Adjcell = new Node({row, col},curr->distance + 1, curr);
                q.enqueue(Adjcell);
            } else if((isValid(row, col) && (weight == WGT_POD_TO_TARGET && //is valid and carrying a pod
                (warehouse[row][col]->getType() == "empty" || warehouse[row][col]->getType() == "target" || warehouse[row][col]->getType() == "dock")) && //can go on empty, target or dock squares
                !visited[col][row])) {
                //qDebug() << warehouse[row][col]->getType() << row << col << "has pod";
                visited[col][row] = true;
                Node* Adjcell = new Node({row, col},curr->distance + 1, curr);
                q.enqueue(Adjcell);
            } else if((isValid(row, col) && (weight == WGT_POD_TO_ORIGIN && //is valid and carrying a pod back to origin
                (warehouse[row][col]->getType() == "empty" || warehouse[row][col]->getType() == "target" || warehouse[row][col]->getType() == "dock")) && //can go on empty, target or dock squares
                !visited[col][row])) {
                visited[col][row] = true;
                Node* Adjcell = new Node({row, col},curr->distance + 1, curr);
                q.enqueue(Adjcell);
                //qDebug() << "pod to origin" << warehouse[row][col]->getType() << row << col << "has pod";
            } else if((isValid(row, col) && (weight == WGT_CHARGE && //is valid and going to charge
                (warehouse[row][col]->getType() == "empty" || warehouse[row][col]->getType() == "pod" || warehouse[row][col]->getType() == "target" || warehouse[row][col]->getType() == "dock")) && //can go on empty, pod, target or dock squares
                !visited[col][row])) {
                //qDebug() << warehouse[row][col]->getType() << row << col << "to charge";
                visited[col][row] = true;
                Node* Adjcell = new Node({row, col},curr->distance + 1, curr);
                q.enqueue(Adjcell);
            }
        }
    }
}

void Model::createPathVector(Node *n, QVector<QPoint> &path) {
    if(n == nullptr) {
        return;
    }
    createPathVector(n->parent, path);
    path.append(n->location);
    //qDebug() << n->location.x() << ", " << n->location.y() << warehouse[n->location.x()][n->location.y()]->getType();
}

QQueue<Task> Model::generatePathQueue(QVector<QPoint> path, Weight w, Robot *r) {
    QQueue<Task> tasks;
    Task task;
    task.weight = w;
    Robot::Direction dir = r->getDirection();
    //qDebug() << "path size: " << path.size();
    for (int i = 1; i < path.size(); i++)
    {
        if (path[i].x() < path[i - 1].x())
        {
            if (dir != Robot::Direction::NORTH)
            {
                //qDebug() << "dir north";
                switch (dir)
                {
                    case Robot::Direction::WEST:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        //qDebug() << "west turn right";
                        break;
                    case Robot::Direction::SOUTH:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        tasks.append(task);
                        //qDebug() << "south turn right x2";
                        break;
                    case Robot::Direction::EAST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        //qDebug() << "east turn left";
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::NORTH;
            }
        }
        else if (path[i].x() > path[i - 1].x())
        {
            if (dir != Robot::Direction::SOUTH)
            {
                //qDebug() << "dir south";
                switch (dir)
                {
                    case Robot::Direction::WEST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        //qDebug() << "west turn left";
                        break;
                    case Robot::Direction::NORTH:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        tasks.append(task);
                        //qDebug() << "north turn left x2";
                        break;
                    case Robot::Direction::EAST:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        //qDebug() << "east turn right";
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::SOUTH;
            }
        }
        else if (path[i].y() < path[i - 1].y())
        {
            if (dir != Robot::Direction::WEST)
            {
                //qDebug() << "dir west";
                switch (dir)
                {
                    case Robot::Direction::EAST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        tasks.append(task);
                        //qDebug() << "east turn left x2";
                        break;
                    case Robot::Direction::NORTH:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        //qDebug() << "north turn left";
                        break;
                    case Robot::Direction::SOUTH:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        //qDebug() << "south turn right";
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::WEST;
            }
        }
        else if (path[i].y() > path[i - 1].y())
        {
            if (dir != Robot::Direction::EAST)
            {
                //qDebug() << "dir east";
                switch (dir)
                {
                    case Robot::Direction::WEST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        tasks.append(task);
                        //qDebug() << "west turn left x2";
                        break;
                    case Robot::Direction::NORTH:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        //qDebug() << "north turn right";
                        break;
                    case Robot::Direction::SOUTH:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        //qDebug() << "south turn left";
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::EAST;
            }
        }
        task.op = OP_MOVE;
        tasks.append(task);
    }
    /*switch (w)
    {
        case WGT_POD_TO_ORIGIN:
            task.op = OP_DROP;
            break;
        case WGT_TO_POD:
            task.op = OP_LIFT;
            break;
        case WGT_WAIT:
        case WGT_POD_TO_TARGET:
        case WGT_CHARGE:
            task.op = OP_CHARGE;
            tasks.append(task);
            tasks.append(task);
            tasks.append(task);
            tasks.append(task);
            task.op = OP_CHARGE_STOP;
            break;
    }
    tasks.append(task);*/ //will be done in another method
    return tasks;
}

bool Model::isValid(int row, int col) {
    return (row >= 0) && (row < size) &&
           (col >= 0) && (col < size);
}

void Model::saveResults(QString filename, QList<int> energyUsed, int allEnergy, int allSteps) {
    QFile file(filename);

    if (!file.open(QFile::WriteOnly))
    {
        qDebug() << "Failed to open file" << filename;
        return;
    }
    file.write("Results:\n\nAll steps: " + QString::number(allSteps).toUtf8() + "\nAll energy used: " + QString::number(allEnergy).toUtf8() + "\nEnergy used by individual robots:\n");
    for(int i = 0; i < energyUsed.size(); i++) {
        file.write("Robot #" + QString::number(i+1).toUtf8() + ": " + QString::number(energyUsed[i]).toUtf8() + "\n");
    }
}

void Model::prodTypes()
{
    for(int i=0;i< pods.count();i++)
        prodSet.unite(pods[i]->getProducts());
}
