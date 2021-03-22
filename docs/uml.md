```plantuml
Class WTile {
 + void setTarget(int prodNum)
 + void clearTarget()
 + bool isTarget()
 + int getTarget()
 + bool isDock()
 + void setDock(bool dock)
 + bool isEmptyTile()
 + void setEmpty(bool empty)
 # Point location
 # int target
 # bool dock
 # bool isEmpty
}
WTile "many" *-- "1" Model : contains

Class Robot extends WTile {
 + int getID()
 + int getPower()
 + void setPower(int p)
 + void lowerPower()
 + int getUsedPower()
 + void setUsedPower(int up)
 + Direction getDirection()
 + QPoint getPosition()
 + QPoint getNextPosition()
 + void setPosition(QPoint pos)
 + void move()
 + void turnLeft()
 + void turnRight()
 + Pod* getPod()
 + void liftPod(Pod* pod)
 + void dropPod()
 + bool hasPod()
 - int id
 - int power
 - int maxPower
 - int usedPower
 - Direction direction
 - QPoint position
 - Pod* pod
}
Robot "many" *-- "1" Model : contains

Class Pod {
 + bool hasProduct(int prodNum)
 + void addProduct(int prodNum)
 + void removeProduct(int prodNum)
 + QSet<int> getProducts()
 + QPoint getPosition()
 + void setPosition(QPoint pos)
 + QPoint getOriginalPosition()
 + void setPickedUp(bool picked)
 + bool isPickedUp()
 + void setReserved(bool reserved)
 + bool isReserved()
 - QSet<int> products
 - QPoint pos
 - QPoint originalPos
 - bool pickedUp
 - bool reserved
}
Pod "many" *-- "1" Model : contains

Class Model {
 + Model(int w, int h, int maxP)
 + void setSize(int w, int h)
 + QVector<QVector<WTile*>> getWarehouse()
 + void createRobot(int x, int y)
 + QList<Robot*> getRobots()
 + Robot* getRobot(int x, int y)
 + void createPod(int x, int y, QSet<int> prods)
 + QList<Pod*> getPods()
 + Pod* getPod(int x, int y)
 + QList<QPair<QPoint, int>> getTargets()
 + QList<QPoint> getDocks()
 + void createTarget(int x, int y, int prodNum)
 + void createDock(int x, int y)
 + void createOrder(int prodNum)
 + QQueue<int> getOrders()
 + void save(QString filename)
 + void load(QString filename)
 + void tick()
 + void onTick()
 - int width
 - int height
 - QVector<QVector<WTile*>> warehouse
 - void makeWarehouse()
 - int maxPower
 - QList<Robot*> robots
 - int robotCount
 - QList<Pod*> pods
 - QQueue<int> orders
 - QVector<QQueue<Task>> tasks
 - void executeTask(int id)
 - QPoint findClosestTarget(QPoint pos, int prodNum)
 - bool isPodAvailable(int prodNum)
 - QPoint findClosestPod(QPoint pos, int prodNum)
 - QPoint findClosestDock(QPoint pos)
}

Enum Direction {
    NORTH
    EAST
    SOUTH
    WEST
}
Direction *- Robot : use

Class MainWindow {
 + QWidget *window
 + QHBoxLayout* _mainLayout
 + QVBoxLayout* _leftsideLayout
 + QVBoxLayout* _titleLayout
 + QVBoxLayout* _infoLayout
 + QGridLayout* _buttonLayout
 + QVBoxLayout *_rightsideLayout
 + QGridLayout* _gridLayout
 + QVector<QVector<QPushButton*>> _gridButtons
 + QHBoxLayout* buttonContainer
 + QTimer* timer
 - Model* _model
 - Editor* _editor
 - void setupWindow()
 - void drawTable()
 - void refreshTable()
 - int Size
 - int _height
 - int _width
 - void editorButtonClicked()
 - void startButtonClicked()
 - void saveButtonClicked()
 - void loadButtonClicked()
 - void editorApplyAndClose()
 - void onTick()
}

EditorWindow "1" *-- "1" MainWindow : contains
Controller "1" *- "1" MainWindow : contains

Class EditorWindow {
 + int getHeight()
 + int getWidth()
 + QVector<QPoint> getRobots()
 + QVector<QPair<QPoint, QSet<int>>> getPods()
 + QVector<QPair<QPoint, int>> getTargets()
 + QVector<QPoint> getDocks()
 + void applyAndClose()
 - QWidget *editor
 - QWidget *sizeWindow
 - QPushButton* okButton
 - QPushButton* closeButton
 - QLineEdit* _h
 - QLineEdit* _w
 - QVBoxLayout* _mainLayout
 - QGridLayout* _gridLayout
 - QVBoxLayout* _bottomLayout
 - QHBoxLayout* _editButtonsLayout
 - QGridLayout* _infoLayout
 - QHBoxLayout* _infoButtonsLayout
 - QHBoxLayout* buttonContainer
 - QVector<QVector<QPushButton*>> _gridButtons
 - QPushButton* _selectButton
 - QPushButton* _robotButton
 - QPushButton* _podButton
 - QPushButton* _targetButton
 - QPushButton* _dockButton
 - QPushButton* _deleteButton
 - QPushButton* _undoButton
 - QPushButton* _redoButton
 - QLineEdit* _prodNumsLEdit
 - QComboBox* _prodNumCBox
 - QPushButton* _newButton
 - QPushButton* _loadButton
 - QPushButton* _saveButton
 - QPushButton* _applyButton
 - int _height
 - int _width
 - int status
 - QVector<QPoint> robots
 - QVector<QPair<QPoint, QSet<int>>> pods
 - QVector<QPair<QPoint, int>> targets
 - QVector<QPoint> docks
 - QSet<int> prodNums
 - void setupEditor()
 - void setupSizeWindow()
 - void setupTable()
 - private slots:
 - void okButtonClicked()
 - void closeButtonClicked()
 - void editButtonsClicked()
 - void controlButtonsClicked()
 - void gridButtonClicked()
}
```
