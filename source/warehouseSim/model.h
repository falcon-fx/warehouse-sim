#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QQueue>
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
    OP_CHARGE,
    OP_CHARGE_STOP,
    OP_WAIT
};

enum Weight
{
    WGT_WAIT,
    WGT_POD_TO_TARGET,
    WGT_POD_TO_ORIGIN,
    WGT_TO_POD,
    WGT_CHARGE
};

struct Task
{
    Operation op;
    Weight weight;
};

class Model : public QObject
{
    Q_OBJECT
public:
    Model(int w, int h, int maxP);
    void setSize(int w, int h);
    QVector<QVector<WTile*>> getWarehouse();
    void createRobot(int x, int y);
    QList<Robot*> getRobots();
    Robot* getRobot(int x, int y);
    void createPod(int x, int y, QSet<int> prods);
    QList<Pod*> getPods();
    Pod* getPod(int x, int y);
    QList<QPair<QPoint, int>> getTargets();
    QList<QPoint> getDocks();
    void createTarget(int x, int y, int prodNum);
    void createDock(int x, int y);
    void createOrder(int prodNum);
    QQueue<int> getOrders();

    void save(QString filename);
    void load(QString filename);
public slots:
    void tick();
signals:
    void onTick();
    void onLoad();
private:
    int width;
    int height;
    QVector<QVector<WTile*>> warehouse;
    void makeWarehouse();
    int maxPower;
    QList<Robot*> robots;
    int robotCount;
    QList<Pod*> pods;
    QQueue<int> orders;
    QVector<QQueue<Task>> tasks;

    void executeTask(int id);

    QPoint findClosestTarget(QPoint pos, int prodNum);
    bool isPodAvailable(int prodNum);
    QPoint findClosestPod(QPoint pos, int prodNum);
    QPoint findClosestDock(QPoint pos);
};

#endif // MODEL_H
