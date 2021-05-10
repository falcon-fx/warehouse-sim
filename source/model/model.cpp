#include "model.h"
#include <QDebug>
#include <QFile>

/*!
 * \brief Model::Model
 * Construct a new warehouse model with the
 * specified size and max power.
 * \param s The size
 * \param maxP The maximum power
 */
Model::Model(int s, int maxP) : size(s), maxPower(maxP)
{
    rowNum = {-1,0,0,1};
    colNum = {0,-1,1,0};
    makeWarehouse();
}

/*!
 * \brief Model::setSize
 * Resize the warehouse to the specified size
 * \param s The specified size
 */
void Model::setSize(int s)
{
    size = s;
    makeWarehouse();
}

/*!
 * \brief Model::getWarehouse
 * Get the current warehouse state.
 * \return The warehouse as a two-dimensional vector
 */
QVector<QVector<WTile*>> Model::getWarehouse()
{
    return this->warehouse;
}

/*!
 * \brief Model::makeWarehouse
 * Create the warehouse grid.
 */
void Model::makeWarehouse()
{
    totalTasks = 0;
    tasksDone = 0;
    robotCount = 0;
    robots.clear();
    orders.clear();
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

/*!
 * \brief Model::createRobot
 * Create a new robot at the x and y coordinates
 * \param x The x coordinate
 * \param y The y coordinate
 */
void Model::createRobot(int x, int y)
{
    robots.append(new Robot(robotCount++, this->maxPower, y, x));
    QQueue<Task> task_list;
    tasks.append(task_list);
    warehouse[y][x]->setType(3);
}

/*!
 * \brief Model::createPod
 * Create a new pod at the x and y coordinates,
 * holding a set of products.
 * \param x The x coordinate
 * \param y The y coordinate
 * \param prods The set of products
 */
void Model::createPod(int x, int y, QSet<int> prods)
{
    pods.append(new Pod(prods, y, x));
    foreach (const int &p, prods)
        if (!products.contains(p))
            products.append(p);
    warehouse[y][x]->setType(2);
}

/*!
 * \brief Model::createPod
 * Create a new displaced pod at the x and y coordinates,
 * with ox and oy as the origin coordinates, holding a
 * set of products.
 * \param x The x coordinate
 * \param y The y coordinate
 * \param ox The origin x coordinate
 * \param oy The origin y coordinate
 * \param prods The set of products
 */
void Model::createPod(int x, int y, int ox, int oy, QSet<int> prods)
{
    pods.append(new Pod(prods, y, x, oy, ox));
    foreach (const int &p, prods)
        if (!products.contains(p))
            products.append(p);
    warehouse[y][x]->setType(2);
}

/*!
 * \brief Model::getRobots
 * Get a list of all robots in the warehouse.
 * \return A list with all robot objects
 */
QList<Robot*> Model::getRobots()
{
    return this->robots;
}

/*!
 * \brief Model::getRobot
 * Get a robot at the given x and y coordinates.
 * \param x The x coordinate
 * \param y The y coordinate
 * \return A robot object or a null pointer
 */
Robot* Model::getRobot(int x, int y)
{
    for (int i = 0; i < robots.count(); i++)
        if (robots[i]->getPosition().x() == x && robots[i]->getPosition().y() == y)
            return robots[i];
    return nullptr;
}

/*!
 * \brief Model::getPods
 * Get a list of all pods in the warehouse.
 * \return A list of all pod objects
 */
QList<Pod*> Model::getPods()
{
    return this->pods;
}

/*!
 * \brief Model::getPod
 * Get a pod at the given x and y coordinates.
 * \param x The x coordinate
 * \param y The y coordinate
 * \return A pod object or a null pointer
 */
Pod* Model::getPod(int x, int y)
{
    for (int i = 0; i < pods.count(); i++)
        if (pods[i]->getPosition().x() == x && pods[i]->getPosition().y() == y)
            return pods[i];
    return nullptr;
}

/*!
 * \brief Model::getProducts
 * Get a vector of all products in the warehouse.
 * \return A vector of all products in the
 */
QVector<int> Model::getProducts()
{
    return this->products;
}

/*!
 * \brief Model::getTargets
 * Get a list of all targets in the warehouse, as a pair
 * of the position and the product number.
 * \return A list of pairs containing the position and
 * associated product number.
 */
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

/*!
 * \brief Model::getDocks
 * Get a list of all the dock points in the warehouse.
 * \return Get a list of all docks
 */
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

/*!
 * \brief Model::createTarget
 * Create a target at the x and y coordinates for the
 * specified product number.
 * \param x The x coordinate
 * \param y The y coordinate
 * \param prodNum The product number
 */
void Model::createTarget(int x, int y, int prodNum)
{
    if (!this->warehouse[y][x]->isDock())
    {
        this->warehouse[y][x]->setTarget(prodNum);
        this->warehouse[y][x]->setType(4);
    }
}

/*!
 * \brief Model::createDock
 * Create a dock at the x and y coordinates.
 * \param x The x coordinate
 * \param y The y coordinate
 */
void Model::createDock(int x, int y)
{
    if (!this->warehouse[y][x]->isTarget())
    {
        this->warehouse[y][x]->setDock(true);
        this->warehouse[y][x]->setType(1);
    }
}

/*!
 * \brief Model::createOrder
 * Create an order for the product number.
 * \param prodNum The product number
 */
void Model::createOrder(int prodNum)
{
    this->totalTasks++;
    this->orders.append(prodNum);
}

/*!
 * \brief Model::getOrders
 * Get a queue of all the placed orders.
 * \return
 */
QQueue<int> Model::getOrders()
{
    return this->orders;
}

/*!
 * \brief Model::getTotalTasks
 * Get the number of all the tasks.
 * \return
 */
int Model::getTotalTasks()
{
    return totalTasks;
}

/*!
 * \brief Model::getTasksDone
 * Get the number of all the done tasks.
 * \return
 */
int Model::getTasksDone()
{
    return tasksDone;
}

/*!
 * \brief Model::getMaxPower
 * Get the maximum power.
 * \return
 */
int Model::getMaxPower()
{
    return maxPower;
}

/*!
 * \brief Model::setMaxPower
 * Set the maximum power.
 * \param pwr The power
 */
void Model::setMaxPower(int pwr)
{
    maxPower = pwr;
}

/*!
 * \brief Model::getSize
 * Get the size of the warehouse.
 * \return
 */
int Model::getSize()
{
    return this->size;
}

/*!
 * \brief Model::tick
 * This function executes every tick and runs the
 * simulation.
 */
void Model::tick()
{
    if (reservedPoints.size() > 0)
        reservedPoints.pop_front();

    for (int i = 0; i < robotCount; i++)
    {
        if (!orders.isEmpty() && tasks[i].isEmpty() && !robots[i]->hasPod() && robots[i]->getProdNum() == -1)
        {
            int order = orders.dequeue();
            robots[i]->setProdNum(order);
        }
    }

    for(int i = 0; i < robotCount; i++)
    {
        Robot* r = robots[i];

        if (r->getProdNum() != -1 && !r->hasPod() && tasks[i].isEmpty())
            gotoPod(r, i);
        else if (r->getProdNum() != -1 && r->hasPod() && r->getPod()->getOriginalPosition() == r->getPosition() && tasks[i].isEmpty())
            gotoTarget(r, i);
        else if (r->getProdNum() == -1 && r->hasPod() && r->getPod()->getOriginalPosition() != r->getPosition() && tasks[i].isEmpty())
            bringBackPod(r, i);

        executeTask(i);
    }

    emit onTick();

    bool tasksFinished = orders.isEmpty();
    for (int i = 0; i < robots.size() && tasksFinished; i++)
        tasksFinished = tasksFinished && robots[i]->getProdNum() == -1;
    for(int i = 0; i < tasks.size() && tasksFinished; i++)
        tasksFinished = tasksFinished && tasks[i].isEmpty();
    for(int i = 0; i < pods.size() && tasksFinished; i++)
        tasksFinished = tasksFinished && pods[i]->getOriginalPosition() == pods[i]->getPosition();
    if(tasksFinished)
        emit onFinished();
}

/*!
 * \brief Model::gotoDock
 * Command the robot to go to the closest dock and charge.
 * \param robot The robot
 * \param robotID The robot's ID
 */
void Model::gotoDock(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestDock = findClosestDock(robot->getPosition());
    warehouse[closestDock.x()][closestDock.y()]->setReserved(true);
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestDock, path, energyNeeded, tasks[robotID], WGT_CHARGE, robot);
}

/*!
 * \brief Model::gotoDock
 * Command the robot to pick up the closest pod that has
 * the product assigned to the robot.
 * \param robot The robot
 * \param robotID The robot's ID
 */
void Model::gotoPod(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestPod = findClosestPod(robot->getPosition(), robot->getProdNum());
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestPod, path, energyNeeded, tasks[robotID], WGT_TO_POD, robot);
}

/*!
 * \brief Model::gotoTarget
 * Command the robot to take the pod to the target that
 * matches the product assigned to the robot.
 * \param robot The robot
 * \param robotID The robot's ID
 */
void Model::gotoTarget(Robot *robot, int robotID) {
    tasks[robotID].clear();
    QPoint closestTarget = findClosestTarget(robot->getPosition(), robot->getProdNum());
    int energyNeeded = 0;
    int path = 0;
    createPath(robot->getPosition(), closestTarget, path, energyNeeded, tasks[robotID], WGT_POD_TO_TARGET, robot);
    if(energyNeeded * 2 > robot->getPower()) {
        orders.push_front(robot->getProdNum());
        robot->setProdNum(-1);
        robot->dropPod();
        gotoDock(robot, robotID);
    } else {
        tasksDone++;
    }
}

/*!
 * \brief Model::bringBackPod
 * Command the robot to take the pod back to it's
 * original position.
 * \param robot The robot
 * \param robotID The robot's ID
 */
void Model::bringBackPod(Robot *robot, int robotID) {
    tasks[robotID].clear();
    int energyNeeded = 0;
    int path = 0;
    QPoint origin = robot->getPod()->getOriginalPosition();
    createPath(robot->getPosition(), origin, path, energyNeeded, tasks[robotID], WGT_POD_TO_ORIGIN, robot);
}

/*!
 * \brief Model::executeTask
 * Executes the first task in the specified robot's
 * task queue.
 * \param id The robot's ID
 */
void Model::executeTask(int id)
{
    if (!tasks[id].empty())
    {
        Operation op = tasks[id].dequeue().op;
        Robot* r = robots[id];
        switch (op)
        {
            case OP_MOVE: {
                warehouse[r->getPosition().x()][r->getPosition().y()]->setReserved(false);
                r->move();
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
                warehouse[r->getPod()->getPosition().x()][r->getPod()->getPosition().y()]->setType(2);
                r->dropPod();
                break;
            case OP_DELIVER:
                r->setProdNum(-1);
                break;
            case OP_CHARGE_STOP:
                r->setPower(maxPower);
                warehouse[r->getPosition().x()][r->getPosition().y()]->setReserved(false);
            case OP_CHARGE:
            case OP_WAIT:
                break;
        }
    }
}

/*!
 * \brief Model::findClosestTarget
 * Find the closest target from origin that
 * matches the specified product number.
 * \param pos The origin position
 * \param prodNum The searched product number
 * \return
 */
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

/*!
 * \brief Model::isPodAvailable
 * Check if the specified product number is
 * available on any pod that is not being moved.
 * \param prodNum The product number
 * \return
 */
bool Model::isPodAvailable(int prodNum)
{
    for (int i = 0; i < pods.size(); i++)
        if (pods[i]->hasProduct(prodNum) && !pods[i]->isPickedUp())
            return true;
    return false;
}

/*!
 * \brief Model::findClosestPod
 * Find the closest pod from origin that
 * has the specified product number.
 * \param pos The origin position
 * \param prodNum The product number
 * \return
 */
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
                closest.setY(pods[i]->getPosition().y());
                distance = (pos - pods[i]->getPosition()).manhattanLength();
            }
        }
    }
    return closest;
}

/*!
 * \brief Model::findClosestDock
 * Find the closest free dock from origin.
 * \param pos The origin position
 * \return
 */
QPoint Model::findClosestDock(QPoint pos)
{
    int distance = -1;
    QPoint closest;
    for (int i = 0; i < getDocks().size(); i++)
    {
        bool isFree = !warehouse[getDocks()[i].y()][getDocks()[i].x()]->isReserved();
        for (int i = 0; i < robots.size() && isFree; i++)
            isFree = robots[i]->getPosition() != getDocks()[i];

        if (isFree && (distance == -1 || ((pos - getDocks()[i]).manhattanLength() < distance)))
        {
            closest.setX(getDocks()[i].y());
            closest.setY(getDocks()[i].x());
            distance = (pos - getDocks()[i]).manhattanLength();
        }
    }
    return closest;
}

/*!
 * \brief Model::save
 * Save the game as the specified filename.
 * \param filename The filename to save as
 */
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
        return;

    file.write(QString::number(size).toUtf8() + "\n"); // Size

    // Robots
    file.write(QString::number(robots.count()).toUtf8() + "\n"); // count
    for (int i = 0; i < robots.count(); i++)
    {
        file.write(QString::number(robots[i]->getPosition().x()).toUtf8() + " "); //
        file.write(QString::number(robots[i]->getPosition().y()).toUtf8() + " "); // position
        file.write(QString::number(robots[i]->getPower()).toUtf8() + " "); // power
        file.write(QString::number(robots[i]->getUsedPower()).toUtf8() + "\n"); // used power
    }

    // Pods
    file.write(QString::number(pods.count()).toUtf8() + "\n"); // count
    for (int i = 0; i < pods.count(); i++)
    {
        file.write(QString::number(pods[i]->getPosition().x()).toUtf8() + " "); //
        file.write(QString::number(pods[i]->getPosition().y()).toUtf8() + " "); // position
        file.write(QString::number(pods[i]->getOriginalPosition().x()).toUtf8() + " "); //
        file.write(QString::number(pods[i]->getOriginalPosition().y()).toUtf8() + " "); // original position
        file.write(QString::number(pods[i]->getProducts().count()).toUtf8() + " "); // count of products
        foreach (const int &v, pods[i]->getProducts())
            file.write(QString::number(v).toUtf8() + " "); // product number
        file.write("\n");
    }

    // Targets
    QList<QPair<QPoint, int>> targets = getTargets();
    file.write(QString::number(targets.count()).toUtf8() + "\n"); // count
    for (int i = 0; i < targets.count(); i++)
    {
        file.write(QString::number(targets[i].first.x()).toUtf8() + " "); //
        file.write(QString::number(targets[i].first.y()).toUtf8() + " "); // position
        file.write(QString::number(targets[i].second).toUtf8() + "\n"); // product number
    }

    // Docks
    QList<QPoint> docks = getDocks();
    file.write(QString::number(docks.count()).toUtf8() + "\n"); // count
    for (int i = 0; i < docks.count(); i++)
    {
        file.write(QString::number(docks[i].x()).toUtf8() + " ");  //
        file.write(QString::number(docks[i].y()).toUtf8() + "\n"); // position
    }

    // Orders
    file.write(QString::number(orders.count()).toUtf8() + "\n"); // count
    for (int i = 0; i < orders.count(); i++)
        file.write(QString::number(orders[i]).toUtf8() + "\n"); // product number

    file.close();
}

/*!
 * \brief Model::load
 * Load the game from the specified file.
 * \param filename The file to load from
 */
void Model::load(QString filename)
{
    QFile file(filename);

    if (!file.open(QFile::ReadOnly))
        return;

    QTextStream in(&file);

    // Size
    int s;
    in >> s;
    this->setSize(s);

    // Robots
    int rc; // count
    in >> rc;
    for (int i = 0; i < rc; i++)
    {
        int x, y, pwr, upwr; // x, y, power, used power
        in >> x >> y >> pwr >> upwr;
        createRobot(x, y);
        QPoint p(x, y);
        robots[i]->setPosition(p);
        robots[i]->setPower(pwr);
        robots[i]->setUsedPower(upwr);
    }

    // Pods
    int pc; // count
    in >> pc;
    for (int i = 0; i < pc; i++)
    {
        int x, y, ox, oy, prc; // x, y, ox, oy, count of products
        in >> x >> y >> ox >> oy >> prc;
        QSet<int> pr;
        for (int j = 0; j < prc; j++)
        {
            int prn; // product number
            in >> prn;
            pr.insert(prn);
        }
        createPod(y, x, oy, ox, pr);
    }

    // Targets
    int tc; // count
    in >> tc;
    for (int i = 0; i < tc; i++)
    {
        int x, y, tn; // x, y, product number
        in >> x >> y >> tn;
        createTarget(x, y, tn);
    }

    // Docks
    int dc; // count
    in >> dc;
    for (int i = 0; i < dc; i++)
    {
        int x, y; // x, y
        in >> x >> y;
        createDock(x, y);
    }

    // Orders
    int oc; // count
    in >> oc;
    for (int i = 0; i < oc; i++)
    {
        int id; // product number
        in >> id;
        orders.append(id);
    }
    emit onLoad();
}

/*!
 * \brief Model::createPath
 * \param start
 * \param end
 * \param shortestPath
 * \param energyNeeded
 * \param tasks
 * \param weight
 * \param robot
 */
void Model::createPath(QPoint start, QPoint end, int &shortestPath, int &energyNeeded, QQueue<Task> &tasks, Weight weight, Robot* robot) {
    QVector<QVector<bool>> visited;
    for(int i = 0; i < size; i++)
    {
        QVector<bool> tmp;
        for(int j = 0; j < size; j++)
            tmp.push_back(false);
        visited.push_back(tmp);
    }

    visited[start.y()][start.x()] = true;

    QQueue<Node*> q;

    Node* s = new Node(start, 0, nullptr);
    q.enqueue(s);
    int currentTime = 0;
    while (!q.empty())
    {
        Node* curr = q.front();
        QPoint loc = curr->location;

        if (reservedPoints.size() <= currentTime)
            reservedPoints.resize(currentTime + 1);

        if (loc.x() == end.x() && loc.y() == end.y())
        {
            QVector<QPoint> tmpPath;
            createPathVector(q.front(), tmpPath);
            tasks.append(generatePathQueue(tmpPath, weight, robot));
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

        q.dequeue();

        for (int i = 0; i < 4; i++) {
            int row = loc.x() + rowNum[i];
            int col = loc.y() + colNum[i];
            if ((isValid(row, col) && (weight == WGT_TO_POD && //is valid and going to pod
                (warehouse[row][col]->getType() == "empty"
              || warehouse[row][col]->getType() == "pod"
              || warehouse[row][col]->getType() == "target"
              || warehouse[row][col]->getType() == "dock")) && //can go on empty, pod, target or dock squares
                !visited[col][row]))
            {
                if (reservedPoints[currentTime].contains(QPoint(row, col)))
                    continue;
                visited[col][row] = true;
                Node* adjcell = new Node({row, col}, curr->distance + 1, curr);
                q.enqueue(adjcell);
            }
            else if ((isValid(row, col) && (weight == WGT_POD_TO_TARGET && //is valid and carrying a pod
                (warehouse[row][col]->getType() == "empty"
              || warehouse[row][col]->getType() == "target")) && //can go on empty, target or dock squares
                !visited[col][row]))
            {
                if (reservedPoints[currentTime].contains(QPoint(row, col)))
                    continue;
                visited[col][row] = true;
                Node* adjcell = new Node({row, col}, curr->distance + 1, curr);
                q.enqueue(adjcell);
            }
            else if ((isValid(row, col) && (weight == WGT_POD_TO_ORIGIN && //is valid and carrying a pod back to origin
                (warehouse[row][col]->getType() == "empty"
              || warehouse[row][col]->getType() == "target")) && //can go on empty, target or dock squares
                !visited[col][row]))
            {
                if (reservedPoints[currentTime].contains(QPoint(row, col)))
                    continue;
                visited[col][row] = true;
                Node* adjcell = new Node({row, col}, curr->distance + 1, curr);
                q.enqueue(adjcell);
            }
            else if ((isValid(row, col) && (weight == WGT_CHARGE && //is valid and going to charge
                (warehouse[row][col]->getType() == "empty"
              || warehouse[row][col]->getType() == "pod"
              || warehouse[row][col]->getType() == "target"
              || warehouse[row][col]->getType() == "dock")) && //can go on empty, pod, target or dock squares
                !visited[col][row]))
            {
                if (reservedPoints[currentTime].contains(QPoint(row, col)))
                    continue;
                visited[col][row] = true;
                Node* adjcell = new Node({row, col}, curr->distance + 1, curr);
                q.enqueue(adjcell);
            }
        }
        currentTime++;
    }
}

/*!
 * \brief Model::createPathVector
 * \param n
 * \param path
 */
void Model::createPathVector(Node *n, QVector<QPoint> &path) {
    if(n == nullptr) {
        return;
    }
    createPathVector(n->parent, path);
    path.append(n->location);
}

/*!
 * \brief Model::generatePathQueue
 * Convert path vector to a queue of tasks with
 * the specified weight for the specified robot.
 * \param path The path to convert
 * \param w The weight of the task
 * \param r The robot
 * \return
 */
QQueue<Task> Model::generatePathQueue(QVector<QPoint> path, Weight w, Robot *r)
{
    QQueue<Task> tasks;
    Task task;
    task.weight = w;
    Robot::Direction dir = r->getDirection();
    int currentTime = 0;
    QPoint currentPos = r->getPosition();
    for (int i = 1; i < path.size(); i++)
    {
        if (reservedPoints.size() < currentTime)
            reservedPoints.resize(currentTime + 1);
        bool moves = false;
        if (path[i].x() < path[i - 1].x())
        {
            if (dir != Robot::Direction::NORTH)
            {
                switch (dir)
                {
                    case Robot::Direction::WEST:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::SOUTH:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::EAST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::NORTH;
            }
            currentPos.rx()--;
            moves = true;
        }
        else if (path[i].x() > path[i - 1].x())
        {
            if (dir != Robot::Direction::SOUTH)
            {
                switch (dir)
                {
                    case Robot::Direction::WEST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::NORTH:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::EAST:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::SOUTH;
            }
            currentPos.rx()++;
            moves = true;
        }
        else if (path[i].y() < path[i - 1].y())
        {
            if (dir != Robot::Direction::WEST)
            {
                switch (dir)
                {
                    case Robot::Direction::EAST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::NORTH:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::SOUTH:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::WEST;
            }
            currentPos.ry()--;
            moves = true;
        }
        else if (path[i].y() > path[i - 1].y())
        {
            if (dir != Robot::Direction::EAST)
            {
                switch (dir)
                {
                    case Robot::Direction::WEST:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::NORTH:
                        task.op = OP_TURN_RIGHT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    case Robot::Direction::SOUTH:
                        task.op = OP_TURN_LEFT;
                        tasks.append(task);
                        if (reservedPoints.size() < currentTime)
                            reservedPoints.resize(currentTime + 1);
                        reservedPoints[currentTime].append(currentPos);
                        currentTime++;
                        break;
                    default:
                        break;
                }
                dir = Robot::Direction::EAST;
            }
            currentPos.ry()++;
            moves = true;
        }
        task.op = moves ? OP_MOVE : OP_WAIT;
        tasks.append(task);
        if (reservedPoints.size() < currentTime)
            reservedPoints.resize(currentTime + 1);
        reservedPoints[currentTime].append(currentPos);
        currentTime++;
    }
    return tasks;
}

/*!
 * \brief Model::isValid
 * Check if the specified row and column values
 * are within the bounds of the warehouse.
 * \param row The row
 * \param col The col
 * \return
 */
bool Model::isValid(int row, int col)
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

/*!
 * \brief Model::saveResults
 * Save the results of the simulation into a plain text file.
 * \param filename The filename to save as
 * \param energyUsed Used power per robots
 * \param allEnergy The total used power
 * \param allSteps The total steps taken
 */
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

/*!
 * \brief Model::getProdSet
 * Get a set of all the available products in the warehouse.
 * \return
 */
QSet<int> Model::getProdSet()
{
    for (int i = 0; i < pods.count(); i++)
        prodSet.unite(pods[i]->getProducts());
    return prodSet;
}
