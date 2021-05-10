#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QQueue>
#include <QSet>
#include "wtile.h"
#include "robot.h"
#include "pod.h"

enum Operation
{
    OP_MOVE,
    OP_TURN_LEFT,
    OP_TURN_RIGHT,
    OP_LIFT,
    OP_DROP,
    OP_DELIVER,
    OP_CHARGE,
    OP_CHARGE_STOP,
    OP_WAIT
};

enum Weight
{
    WGT_CHARGE,
    WGT_TO_POD,
    WGT_POD_TO_ORIGIN,
    WGT_POD_TO_TARGET
};

struct Task
{
    Operation op;
    Weight weight;
};

struct Node {
    QPoint location;
    int distance;
    Node* parent;
    Node() {parent = nullptr;}
    Node(QPoint loc, int dist, Node* par) : location(loc), distance(dist), parent(par) {}
};

class Model : public QObject
{
    Q_OBJECT
public:
    Model(int s, int maxP);
    void setSize(int s);
    QVector<QVector<WTile*>> getWarehouse();
    void createRobot(int x, int y);
    QList<Robot*> getRobots();
    Robot* getRobot(int x, int y);
    void createPod(int x, int y, QSet<int> prods);
    void createPod(int x, int y, int ox, int oy, QSet<int> prods);
    QList<Pod*> getPods();
    Pod* getPod(int x, int y);
    QVector<int> getProducts();
    QList<QPair<QPoint, int>> getTargets();
    QList<QPoint> getDocks();
    void createTarget(int x, int y, int prodNum);
    void createDock(int x, int y);
    void createOrder(int prodNum);
    QQueue<int> getOrders();
    int getTotalTasks();
    int getTasksDone();
    int getMaxPower();
    void setMaxPower(int pwr);
    QSet<int> getProdSet();
    int getSize();
    void save(QString filename);
    void load(QString filename);
    void saveResults(QString filename, QList<int> energyUsed, int allEnergy, int allSteps);
public slots:
    void tick();
signals:
    void onTick();
    void onLoad();
    void onFinished();
private:
    int size;
    QVector<QVector<WTile*>> warehouse;
    void makeWarehouse();
    int maxPower;
    QList<Robot*> robots;
    int robotCount;
    QList<Pod*> pods;
    QVector<int> products;
    QQueue<int> orders;
    QVector<QQueue<Task>> tasks;
    QVector<int> rowNum;
    QVector<int> colNum;
    int totalTasks;
    int tasksDone;
    QSet<int> prodSet;

    QVector<QVector<QPoint>> reservedPoints;

    void executeTask(int id);

    QPoint findClosestTarget(QPoint pos, int prodNum);
    bool isPodAvailable(int prodNum);
    QPoint findClosestPod(QPoint pos, int prodNum);
    QPoint findClosestDock(QPoint pos);
    void createPath(QPoint start, QPoint end, int &shortestPath, int &energyNeeded, QQueue<Task> &tasks, Weight weight, Robot* robot);
    void createPathVector(Node* n, QVector<QPoint> &path);
    QQueue<Task> generatePathQueue(QVector<QPoint> path, Weight w, Robot* r);
    bool isValid(int row, int col);
    void gotoDock(Robot* robot, int robotID);
    void gotoPod(Robot* robot, int robotID);
    void gotoTarget(Robot* robot, int robotID);
    void bringBackPod(Robot* robot, int robotID);
    void gotoPodUnfinished(Robot* robot, int robotID);
};

#endif // MODEL_H
