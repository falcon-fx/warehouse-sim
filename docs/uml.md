```plantuml
Class WTile {
    + Point pos
    + Color col
    + Bool isEmpty
}
WTile "many" *-- "1" Controller : contains

Class Robot extends WTile {
    + int id
    + int power
    + Bool hasPod
    + int podNum
    + Direction direction
    + void SetTarget(int podNum)
}
Robot "many" *-- "1" Controller : contains

Class Dock extends WTile {
    + int id
    + void Charge(int botNum)
}
Dock "many" *-- "1" Controller : contains

Class Pod extends WTile {
    + int id
    + Bool isPickedUp
    + Vector<int> items
    + Point originalPos
    + void AddItem(int itemNum)
    + void RemoveItem(int itemNum)
}
Pod "many" *-- "1" Controller : contains

Class Target extends WTile {
    + int productNumber
    + Bool isOccupied
    + void Collect(int podNum)
}
Target "many" *-- "1" Controller : contains

Class Controller {
    + int width
    + int height
    + Vector<Vector<WTile>> tiles
    + Vector<int> orders
    + Vector<Robot*> robots
    + Vector<Pod*> pods
    + Vector<Dock*> docks
    + Vector<Target*> targets
    + void Tick()
    + bool Save(string)
    + bool Load(string)
    - void SendCommand(Robot*, Instruction)
    - bool CalcPath(Robot*, WTile*)
    - void CreatePath(Robot*, WTile*)
}

Enum Direction {
    NORTH
    EAST
    SOUTH
    WEST
}
Direction *- Robot : use

Class MainWindow {
    - QPushButton loadButton
    - QPushButton saveButton
    - QPushButton openEditorButton
    - QPushButton startButton
    - Vector<Vector<QPushButton*>> buttonGrid
    - EditorWindow editorWindow
    - QTimer timer
}

EditorWindow "1" *-- "1" MainWindow : contains

Class EditorWindow {
    - QPushButton newRobotButton
    - QPushButton newPodButton
    - QPushButton newTargetButton
    - QPushButton newDockButton
    - QPushButton newProductButton
}
```