#include <QtTest>

// add necessary includes here
#include <../source/model/model.h>

class ModelTest : public QObject
{
    Q_OBJECT

public:
    ModelTest();
    ~ModelTest();

private:
    Model* model;

private slots:
    void init_model();
    void create_docks();
    void create_robots();
    void create_pods();
    void create_target();
    void resize_model();
    void pathfind_to_pod();
    void pathfind_to_target();
    void pathfind_to_dock();
};

ModelTest::ModelTest()
{

}

ModelTest::~ModelTest()
{

}

void ModelTest::init_model()
{
    model = new Model(10, 30);
    QVERIFY(model->getSize() == 10);
}

void ModelTest::create_docks()
{
    model->createDock(0, 1);
    model->createDock(0, 3);
    QList<QPoint> docks = model->getDocks();
    QVERIFY(docks.count() == 2);
    QCOMPARE(docks[0], QPoint(0, 1));
    QCOMPARE(docks[1], QPoint(0, 3));
}

void ModelTest::create_robots()
{
    model->createRobot(0, 5);
    model->createRobot(0, 6);
    QList<Robot*> robots = model->getRobots();
    QVERIFY(robots.count() == 2);
    QCOMPARE(robots[0]->getPosition(), QPoint(5, 0));
    QCOMPARE(robots[1]->getPosition(), QPoint(6, 0));
}

void ModelTest::create_pods()
{
    model->createPod(2, 4, {1});
    model->createPod(2, 5, {2});
    model->createPod(2, 6, {3});
    model->createPod(2, 7, {1, 3});
    QList<Pod*> pods = model->getPods();
    QVERIFY(pods.count() == 4);
    QCOMPARE(pods[0]->getPosition(), QPoint(4, 2));
    QVERIFY(pods[0]->hasProduct(1));
    QCOMPARE(pods[1]->getPosition(), QPoint(5, 2));
    QVERIFY(pods[1]->hasProduct(2));
    QCOMPARE(pods[2]->getPosition(), QPoint(6, 2));
    QVERIFY(pods[2]->hasProduct(3));
    QCOMPARE(pods[3]->getPosition(), QPoint(7, 2));
    QVERIFY(pods[3]->hasProduct(1));
    QVERIFY(pods[3]->hasProduct(3));
}

void ModelTest::create_target()
{
    model->createTarget(7, 7, 1);
    model->createTarget(7, 8, 2);
    model->createTarget(7, 9, 3);
    QList<QPair<QPoint, int>> targets = model->getTargets();
    QVERIFY(targets.count() == 3);
    QCOMPARE(targets[0].first, QPoint(7, 7));
    QCOMPARE(targets[0].second, 1);
    QCOMPARE(targets[1].first, QPoint(7, 8));
    QCOMPARE(targets[1].second, 2);
    QCOMPARE(targets[2].first, QPoint(7, 9));
    QCOMPARE(targets[2].second, 3);
}

void ModelTest::resize_model()
{
    model->setSize(6);
    QVERIFY(model->getSize() == 6);
    QVERIFY(model->getDocks().count() == 0);
    QVERIFY(model->getRobots().count() == 0);
    QVERIFY(model->getPods().count() == 0);
}

void ModelTest::pathfind_to_pod()
{
    model = new Model(10,50);
    QVERIFY(model->getSize() == 10);
    model->createRobot(0, 5);
    QList<Robot*> robots = model->getRobots();
    QVERIFY(robots.count() == 1);
    QCOMPARE(robots[0]->getPosition(), QPoint(5, 0));
    model->createPod(2, 4, {1});
    QList<Pod*> pods = model->getPods();
    QVERIFY(pods.count() == 1);
    QCOMPARE(pods[0]->getPosition(), QPoint(4, 2));
    QVERIFY(pods[0]->hasProduct(1));
    QPoint closest = model->findClosestPod(robots[0]->getPosition(), 1);
    QCOMPARE(closest, QPoint(4,2));
    int shortest = 0;
    int energy = 0;
    QQueue<Task> tasks;
    Weight weight = Weight::WGT_TO_POD;
    model->createPath(robots[0]->getPosition(), closest, shortest, energy, tasks, weight, robots[0]);
    QVERIFY(tasks.size() == 5);
    QVERIFY(shortest == 3);
    QVERIFY(energy == 5);
}

void ModelTest::pathfind_to_target()
{
    model = new Model(10,50);
    QVERIFY(model->getSize() == 10);
    model->createRobot(2, 4);
    QList<Robot*> robots = model->getRobots();
    QVERIFY(robots.count() == 1);
    QCOMPARE(robots[0]->getPosition(), QPoint(4, 2));
    model->createTarget(2, 9, 1);
    QList<QPair<QPoint, int>> targets = model->getTargets();
    QVERIFY(targets.size() == 1);
    QCOMPARE(targets[0].first, QPoint(2, 9));
    QCOMPARE(targets[0].second, 1);
    QPoint closest = model->findClosestTarget(robots[0]->getPosition(), 1);
    QCOMPARE(closest, QPoint(9,2));
    int shortest = 0;
    int energy = 0;
    QQueue<Task> tasks;
    Weight weight = Weight::WGT_POD_TO_TARGET;
    model->createPath(robots[0]->getPosition(), closest, shortest, energy, tasks, weight, robots[0]);
    qDebug() << tasks.size() << shortest << energy;
    QVERIFY(tasks.size() == 8);
    QVERIFY(shortest == 5);
    QVERIFY(energy == 7);
}

void ModelTest::pathfind_to_dock()
{
    model = new Model(10,50);
    QVERIFY(model->getSize() == 10);
    model->createRobot(2, 9);
    QList<Robot*> robots = model->getRobots();
    QVERIFY(robots.count() == 1);
    QCOMPARE(robots[0]->getPosition(), QPoint(9, 2));
    model->createDock(4, 5);
    QList<QPoint> docks = model->getDocks();
    QVERIFY(docks.count() == 1);
    QCOMPARE(docks[0], QPoint(4, 5));
    QPoint closest = model->findClosestDock(robots[0]->getPosition());
    QCOMPARE(closest, QPoint(5,4));
    int shortest = 0;
    int energy = 0;
    QQueue<Task> tasks;
    Weight weight = Weight::WGT_CHARGE;
    model->createPath(robots[0]->getPosition(), closest, shortest, energy, tasks, weight, robots[0]);
    qDebug() << tasks.size() << shortest << energy;
    QVERIFY(tasks.size() == 12);
    QVERIFY(shortest == 6);
    QVERIFY(energy == 7);
}

QTEST_APPLESS_MAIN(ModelTest)

#include "tst_modeltest.moc"
