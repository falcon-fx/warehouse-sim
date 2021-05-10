#include <view/editor.h>

#include <QDebug>

Editor::Editor(Model* model)
{
    setupSizeWindow();
    this->model = model;
}

void Editor::okButtonClicked()
{
    if (sizeLEdit->text().toInt() < 3) // The minimal size of the warehouse
        size = 3;
    else
        size = sizeLEdit->text().toInt();

    model->setMaxPower(maxPowerLEdit->text().toInt()); // Setting the maximum power

    firstClicked = true;
    isSelected = false;

    setupEditor();
    sizeWindow->close();
}

void Editor::newTableButtonClicked() // Opens a new, empty editor
{
    loadTable = false;
    sizeLEdit->setReadOnly(false);
    maxPowerLEdit->setReadOnly(false);
    okButton->setDisabled(false);
}

void Editor::loadTableButtonClicked() // Loads an existing warehouse into the editor
{
    loadTable = true;
    size = model->getSize();

    firstClicked = true;
    isSelected = false;

    sizeWindow->close();
    setupEditor();
}

void Editor::closeButtonClicked()
{
    sizeWindow->close();
}

void Editor::setupTable() // Setting up the table
{
    if (gridLayout->rowCount() != 0 || gridLayout->columnCount() != 0) // Deletion of the grid buttons
    {
        while (QLayoutItem* item = gridLayout->takeAt(0))
        {
            Q_ASSERT(!item->layout());
            delete item->widget();
            delete item;
        }
     }

    gridButtons.clear();

    gridLayout->setAlignment(Qt::AlignCenter);
    if (size <= 0)
        gridLayout->addWidget(new QLabel("Size cannot be 0 or smaller"));
    else // Creating the button grid
    {
        gridButtons.clear();
        gridButtons.resize(size);
        for (int i = 0; i < size; i++)
        {
            gridButtons[i].resize(size);
            for (int j = 0; j < size; j++)
            {
                gridButtons[i][j] = new QPushButton();
                gridButtons[i][j]->setFixedSize(QSize(40, 40));
                gridButtons[i][j]->setEnabled(false);
                gridButtons[i][j]->setStyleSheet("QPushButton { background-color: white; border:0.5px solid gray;}");
                gridLayout->setSpacing(0);
                gridLayout->addWidget(gridButtons[i][j], i, j);
                connect(gridButtons[i][j], SIGNAL(clicked()), this,SLOT(gridButtonClicked()));
            }
        }
    }

}

void Editor::setupEditor()
{
    // Creating the main editor widget
    editor = new QWidget();
    editor->setWindowTitle("Editor");

    // Creating the layouts
    mainLayout = new QVBoxLayout(editor);
    gridLayout = new QGridLayout();
    bottomLayout = new QVBoxLayout();
    editButtonsLayout = new QHBoxLayout();
    infoLayout = new QGridLayout();
    infoButtonsLayout= new QHBoxLayout();
    selectArrowLayout = new QHBoxLayout();
    selectNewProdLayout = new QHBoxLayout();

    // Creating the buttons
    selectButton = new QPushButton("Select");
    robotButton= new QPushButton("Robot");
    podButton= new QPushButton("Pod");
    targetButton= new QPushButton("Target");
    dockButton= new QPushButton("Dock");
    deleteButton= new QPushButton("Delete");
    selectUp = new QPushButton("Move Up");
    selectUp->setVisible(false);
    selectDown = new QPushButton("Move Down");
    selectDown->setVisible(false);
    selectLeft = new QPushButton("Move Left");
    selectLeft->setVisible(false);
    selectRight = new QPushButton("Move Right");
    selectRight->setVisible(false);
    changeProdOkButton = new QPushButton("Put new products");
    changeProdOkButton->setVisible(false);
    newButton = new QPushButton("New");
    applyButton = new QPushButton("Apply and close");

    // Creating the inputs
    prodNumsLEdit = new QLineEdit();
    prodNumCBox = new QComboBox();
    changeProdNumsLEdit = new QLineEdit();
    changeProdNumsLEdit->setVisible(false);

    // Creating the table
    setupTable();

    // Putting the widgets and the layouts together
    editButtonsLayout->addWidget(selectButton);
    editButtonsLayout->addWidget(robotButton);
    editButtonsLayout->addWidget(dockButton);
    editButtonsLayout->addWidget(deleteButton);

    infoLayout->addWidget(prodNumsLEdit, 0, 0);
    infoLayout->addWidget(podButton, 0, 1);
    infoLayout->addWidget(prodNumCBox, 0, 2);
    infoLayout->addWidget(targetButton, 0, 3);

    selectArrowLayout->addWidget(selectUp);
    selectArrowLayout->addWidget(selectDown);
    selectArrowLayout->addWidget(selectLeft);
    selectArrowLayout->addWidget(selectRight);

    selectNewProdLayout->addWidget(changeProdNumsLEdit);
     selectNewProdLayout->addWidget(changeProdOkButton);

    infoButtonsLayout->addWidget(newButton);
    infoButtonsLayout->addWidget(applyButton);

    bottomLayout->addLayout(editButtonsLayout);
    bottomLayout->addLayout(selectArrowLayout);
    bottomLayout->addLayout(selectNewProdLayout);
    bottomLayout->addLayout(infoLayout);
    bottomLayout->addLayout(infoButtonsLayout);

    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(bottomLayout);

    // Connecting the signals and slots of the buttons to their respective events
    connect(selectButton, SIGNAL(clicked()), this, SLOT(editButtonsClicked()));
    connect(robotButton, SIGNAL(clicked()), this, SLOT(editButtonsClicked()));
    connect(podButton, SIGNAL(clicked()), this, SLOT(editButtonsClicked()));
    connect(targetButton, SIGNAL(clicked()), this, SLOT(editButtonsClicked()));
    connect(dockButton, SIGNAL(clicked()), this, SLOT(editButtonsClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(editButtonsClicked()));
    connect(selectUp, SIGNAL(clicked()), this, SLOT(selectMoveButtonClicked()));
    connect(selectDown, SIGNAL(clicked()), this, SLOT(selectMoveButtonClicked()));
    connect(selectRight, SIGNAL(clicked()), this, SLOT(selectMoveButtonClicked()));
    connect(selectLeft, SIGNAL(clicked()), this, SLOT(selectMoveButtonClicked()));
    connect(changeProdOkButton, SIGNAL(clicked()), this, SLOT(changeProd()));
    connect(newButton, SIGNAL(clicked()), this, SLOT(controlButtonsClicked()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(controlButtonsClicked()));

    // Clearup
    robots.clear();
    pods.clear();
    targets.clear();
    docks.clear();
    prodNums.clear();
    prodNumCBox->clear();

    if (loadTable) // If we are loading an existing table
        loadExistingTable();

    editor->show();
}

void Editor::setupSizeWindow() // The new simulation window constructor
{
    sizeWindow = new QWidget;
    sizeWindow->setWindowTitle("New");

    QGridLayout* sizeLayout = new QGridLayout(sizeWindow);
    newTableButton = new QPushButton("New simulation");
    loadTableButton = new QPushButton("Load current simulation");
    sizeLEdit = new QLineEdit("12");
    maxPowerLEdit = new QLineEdit("100");
    okButton = new QPushButton("OK");
    closeButton = new QPushButton("Close");

    sizeLayout->addWidget(loadTableButton, 0, 1, 1, 3);
    sizeLayout->addWidget(newTableButton, 1, 1, 2, 3);
    sizeLayout->addWidget(new QLabel("Set size:"), 3, 1);
    sizeLayout->addWidget(new QLabel("Set robot power:"), 4, 1);
    sizeLayout->addWidget(sizeLEdit, 3, 2);
    sizeLayout->addWidget(maxPowerLEdit, 4, 2);
    sizeLayout->addWidget(okButton, 5, 2);
    sizeLayout->addWidget(closeButton, 5, 3);

    sizeLEdit->setReadOnly(true);
    maxPowerLEdit->setReadOnly(true);
    okButton->setDisabled(true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(newTableButton, SIGNAL(clicked()), this, SLOT(newTableButtonClicked()));
    connect(loadTableButton, SIGNAL(clicked()), this, SLOT(loadTableButtonClicked()));

    sizeWindow->show();
}

void Editor::loadExistingTable() // Loading the model's warehouse into the editor
{
    // Loading of targets
    for(int i = 0; i < model->getTargets().count(); i++)
    {
        QPoint p = QPoint(model->getTargets()[i].first.x() * 40, model->getTargets()[i].first.y() * 40);
        QPair<QPoint, int> targetpair(p, model->getTargets()[i].second);
        targets.append(targetpair);
        necTargets.insert( model->getTargets()[i].second);
        gridButtons[model->getTargets()[i].first.y()][model->getTargets()[i].first.x()]->setStyleSheet("background-color: #92d050;");
        gridButtons[model->getTargets()[i].first.y()][model->getTargets()[i].first.x()]->setText(QString::number(model->getTargets()[i].second));

    }

    // Loading of docks
    for(int i = 0; i < model->getDocks().count(); i++)
    {
        QPoint p = QPoint(model->getDocks()[i].x() * 40, model->getDocks()[i].y() * 40);
        docks.append(p);
        gridButtons[model->getDocks()[i].y()][model->getDocks()[i].x()]->setStyleSheet("background-color: #5b9bd5;");
    }

    // Loading of pods
    for(int i = 0; i < model->getPods().count(); i++)
    {
        QPoint p = QPoint(model->getPods()[i]->getPosition().y() * 40, model->getPods()[i]->getPosition().x() * 40);
        QPair<QPoint, QSet<int>> podpair(p,model->getPods()[i]->getProducts());
        pods.append(podpair);
        gridButtons[model->getPods()[i]->getPosition().x()][model->getPods()[i]->getPosition().y()]->setStyleSheet("background-color: #e6e6e6;");
        for (int j = 0; j <  model->getPods()[i]->getProducts().count(); j++)
            prodNums = prodNums.unite(model->getPods()[i]->getProducts());
        // Text of the pod
        QString podText ="P\n";
        foreach (const int &value, model->getPods()[i]->getProducts())
            podText += QString::number(value) + " ";
        gridButtons[model->getPods()[i]->getPosition().x()][model->getPods()[i]->getPosition().y()]->setText(podText);
    }

    //Upgrading the dropdown list of the possible target product numbers
    foreach (const int &value, prodNums)
        if (prodNumCBox->findText(QString::number(value)) == -1)
            prodNumCBox->insertItem(prodNumCBox->count(), QString::number(value));
    prodNumCBox->model()->sort(0, Qt::AscendingOrder);

    // Loading robots
    for(int i = 0; i < model->getRobots().count(); i++){
        QPoint p = QPoint(model->getRobots()[i]->getPosition().y() * 40, model->getRobots()[i]->getPosition().x() * 40);
        robots.append(p);
        gridButtons[model->getRobots()[i]->getPosition().x()][model->getRobots()[i]->getPosition().y()]->setStyleSheet("background-color: #ffc000;");
    }
}

void Editor::editButtonsClicked() // The handler of the edit buttons
{
    if(!firstClicked) // In case we don't want to select anymore
        selectedButton->setStyleSheet("background-color: auto;");

    selectedButton = qobject_cast<QPushButton*>(sender());
    selectedButton->setStyleSheet("background-color: #ff8a8a;");

    firstClicked=false;

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
           gridButtons[i][j]->setEnabled(true);

    if (selectedButton->text() == "Select")
        status = 1;
    else if (selectedButton->text() == "Robot")
        status = 2;
    else if (selectedButton->text() == "Pod")
        status = 3;
    else if (selectedButton->text() == "Target")
        status = 4;
    else if (selectedButton->text() == "Dock")
        status = 5;
    else if(selectedButton->text() == "Delete")
        status = 6;

    if(status != 1) // Hide the move buttons when the select option is not active
    {
        isSelected = false;
        selectUp->setVisible(false);
        selectDown->setVisible(false);
        selectLeft->setVisible(false);
        selectRight->setVisible(false);
        changeProdOkButton->setVisible(false);
        changeProdNumsLEdit->setVisible(false);
    }
    else // Show the move buttons when the select option is active
    {
        isSelected = true;
        selectUp->setVisible(true);
        selectDown->setVisible(true);
        selectLeft->setVisible(true);
        selectRight->setVisible(true);
        changeProdOkButton->setVisible(true);
        changeProdNumsLEdit->setVisible(true);
    }

    selectedGridButtons.clear(); // Delete the selection
    selectedProds.clear();

    for (int i = 0; i < size; i++) // Restore the selected fields
    {
        for (int j = 0; j < size; j++)
        {
            QColor color = gridButtons[i][j]->palette().button().color();
            gridButtons[i][j]->setStyleSheet("border: 0.5px solid gray; background-color: " + color.name() + ";");
        }
    }
}

void Editor::controlButtonsClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    if (btn->text() == "New") // Start a new, empty warehouse and show the dialog window
    {
        editor->close();
        setupSizeWindow();
    }
    else if (btn->text() == "Apply and close") // Loads the warehouse into the model if it is correct
    {
        if (isTheTableGood()) // Check if it is correct
        {
            for (int i = 0; i < robots.count(); i++) // Robots
            {
                robots[i].setX(robots[i].x() / gridButtons[0][0]->size().width());
                robots[i].setY(robots[i].y() / gridButtons[0][0]->size().width());
            }
            for (int i = 0; i < pods.count(); i++) // Pods
            {
                pods[i].first.setX(pods[i].first.x() / gridButtons[0][0]->size().width());
                pods[i].first.setY(pods[i].first.y() / gridButtons[0][0]->size().width());
            }
            for (int i = 0; i < targets.count(); i++) // Targets
            {
                targets[i].first.setX(targets[i].first.x() / gridButtons[0][0]->size().width());
                targets[i].first.setY(targets[i].first.y() / gridButtons[0][0]->size().width());
            }
            for (int i = 0; i < docks.count(); i++) // Docks
            {
                docks[i].setX(docks[i].x() / gridButtons[0][0]->size().width());
                docks[i].setY(docks[i].y() / gridButtons[0][0]->size().width());
            }
            emit applyAndClose();
            editor->close();
        }
        else // In case the warehouse isn't correct
        {
            QMessageBox msgBox;
            msgBox.setText("The simulation has to have at least one robot, one dock, one product and one target for every product!");
            msgBox.exec();
        }
    }
}

void Editor::gridButtonClicked() // Placing entities on the grid
{
    if (!loadTable && firstClicked)
        model->setSize(size);
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    //btn->setText("");

    QPoint p = btn->pos() - gridButtons[0][0]->pos();

    if (IsEmptyTile(p)) // Is the selected tile empty?
    {
        QSet<int> prods;
        if (!prodNumsLEdit->text().isEmpty()) // Saving products
        {
            QStringList numbers = prodNumsLEdit->text().split(",");
            for (int i = 0; i < numbers.count(); i++)
            {
                prods.insert(numbers[i].toInt());
                prodNums.insert(numbers[i].toInt());
            }
        }
        QString podText = "P\n"; // Text of the pods
        foreach (const int &value, prods)
            podText += QString::number(value) + " ";

        int prodNum = prodNumCBox->currentText().toInt();
        QPair<QPoint, QSet<int>> podpair(p, prods);
        QPair<QPoint, int> targetpair(p, prodNum);

        switch(status)
        {
        case 2: // Robot
            btn->setStyleSheet("background-color: rgb(255, 192, 0);");
            robots.append(p);
            break;
        case 3: // Pod
            btn->setStyleSheet("background-color: rgb(230, 230, 230);");
            btn->setText(podText);
            foreach (const int &value, prodNums)
                if (prodNumCBox->findText(QString::number(value)) == -1)
                    prodNumCBox->insertItem(prodNumCBox->count(), QString::number(value));
             prodNumCBox->model()->sort(0, Qt::AscendingOrder);
            pods.append(podpair);

            break;
        case 4: // Target
            if (prodNumCBox->count() == 0)
                break;
            btn->setStyleSheet("background-color: rgb(146, 208, 80);");
            btn->setText(QString::number(prodNum));
            necTargets.insert(prodNum);
            targets.append(targetpair);
            break;
        case 5: // Dock
            btn->setStyleSheet("background-color: rgb(91, 155, 213);");
            docks.append(p);
            break;
        }
    }
    switch (status)
    {
    case 1: // Select
    {
        QColor color = btn->palette().button().color();
        if (isSelected && color.name() == "#e6e6e6")
        {
            btn->setStyleSheet("border:3px solid red; background-color: " + color.name() +";");

            bool contains = false;
            for (int i = 0; i < selectedGridButtons.count(); i++) //ellenorizzuk, hogy nincs e mar a vectorban a kivalasztott mezo
                if (selectedGridButtons[i] == p)
                    contains = true;

            if (!contains) // If the clicked field is not selected, we add the field to the selection
            {
                if (size > 7)
                    selectedGridButtons.append(QPoint((btn->pos().x() - 11) ,(btn->pos().y() - 11)));
                else
                    selectedGridButtons.append(QPoint((btn->pos().x() - 11 - 45 - (7 - size) * 20) ,(btn->pos().y()-11)));
                selectedProds.append(btn->text());
            }
        }
        break;
    }
    case 6: // Delete
    {
        btn->setStyleSheet("background-color: white;");
        btn->setText("");
        robots.removeOne(p);
        for (int i = 0; i < pods.count(); i++)
        {
            if (pods[i].first == p)
            {
                pods.remove(i);
                break;
            }
        }
        for (int i = 0; i < targets.count(); i++)
        {
            if (targets[i].first == p)
            {
                targets.remove(i);
                break;
            }
        }
        docks.removeOne(p);
        break;
    }
    }
 }

void Editor::selectMoveButtonClicked() // The move buttons function
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    int dirX = 0;
    int dirY = 0;
    if (btn->text() =="Move Up") // Setting the direction
    {
        dirX = 0;
        dirY = -40;
    }
    else if (btn->text() =="Move Down")
    {
        dirX = 0;
        dirY = 40;
    }
    else if (btn->text() =="Move Left")
    {
        dirX = -40;
        dirY = 0;
    }
    else if (btn->text() =="Move Right")
    {
        dirX = 40;
        dirY = 0;
    }
    for (int i=0; i < selectedGridButtons.count(); i++) // Move pods
    {
        int number = 0;
        while (pods[number].first != selectedGridButtons[i] && number < pods.size() - 1) // Search for the selected pods' index
            number++;

        if(selectedGridButtons[i].y()+dirY>=0 && selectedGridButtons[i].y()+dirY<size*40 && selectedGridButtons[i].x()+dirX>=0 && selectedGridButtons[i].x()+dirX<size*40)
        {
            QPoint newPlace =QPoint(selectedGridButtons[i].x()+dirX,selectedGridButtons[i].y()+dirY);
            if(IsEmptyTile(newPlace))
            {
                // Recolor the field
                gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setStyleSheet("QPushButton { background-color: white; }");
                gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setText("");
                selectedGridButtons[i] = QPoint(selectedGridButtons[i].x()+dirX,selectedGridButtons[i].y()+dirY);
                gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setStyleSheet("QPushButton {border:3px solid red; background-color: #e6e6e6; }");
                gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setText(selectedProds[i]);

                // Update the place of the field
                QStringList prodnum = selectedProds[i].split(" ");
                prodnum[0].remove(0,2);
                if(prodnum.size()!=1)
                    prodnum.removeLast();

                // Update the products
                QSet<int> prods;
                if(prodnum[0]!="")
                    for (int j = 0; j < prodnum.count(); j++)
                        prods.insert(prodnum[j].toInt());

                QPair<QPoint, QSet<int>> podpair(newPlace, prods);
                pods.replace(number,podpair);
            }
        }
    }
}

void Editor::changeProd() // Placing other products on the pods
{
    for (int i = 0; i < selectedGridButtons.count(); i++)
    {
        int number = 0;
        while(pods[number].first != selectedGridButtons[i] && number < pods.size()-1) // Search for the selected pods' index
            number++;

        QPoint newPlace = QPoint(selectedGridButtons[i].x(),selectedGridButtons[i].y());
        QSet<int> prods;
        if (!changeProdNumsLEdit->text().isEmpty()) // Adding a new product
        {
            QStringList numbers = changeProdNumsLEdit->text().split(",");
            for (int i = 0; i < numbers.count(); i++)
            {
                prods.insert(numbers[i].toInt());
                prodNums.insert(numbers[i].toInt());
            }
        }
        QString podText = "P\n"; // Updating the pods' text
        foreach (const int &value, prods)
            podText += QString::number(value) + " ";

        selectedProds[i] = podText;
        gridButtons[selectedGridButtons[i].y()/40][selectedGridButtons[i].x()/40]->setText(podText);
        QPair<QPoint, QSet<int>> podpair(newPlace, prods);
        pods.replace(number,podpair);

        foreach (const int &value, prodNums) // Updating the drop down for the target creation
            if (prodNumCBox->findText(QString::number(value)) == -1)
                prodNumCBox->insertItem(prodNumCBox->count(), QString::number(value));
         prodNumCBox->model()->sort(0, Qt::AscendingOrder);
    }

}


bool Editor::IsEmptyTile(QPoint point) // Is the tile empty?
{
    // Check for robots
    point = QPoint(point.x()/40, point.y()/40);
    for (int i = 0; i < robots.count(); i++)
        if (robots[i].x() / 40 == point.x() && robots[i].y() / 40 == point.y())
            return false;

    // Check for pods
    for (int i= 0; i < pods.count(); i++)
        if (pods[i].first.x() / 40 == point.x() && pods[i].first.y() / 40 == point.y())
            return false;

    // Check for targets
    for (int i = 0; i < targets.count(); i++)
        if (targets[i].first.x() / 40 == point.x() && targets[i].first.y() / 40 == point.y())
            return false;

    // Check for docks
    for (int i = 0; i < docks.count(); i++)
        if (docks[i].x() / 40 == point.x() && docks[i].y() / 40 == point.y())
            return false;

    return true;
}

bool Editor::isTheTableGood() // Is the table ready for the simulation? (1 robot, 1 dock, a target for every product)
{
    if (robots.count() >= 1 && docks.count() >= 1 && necTargets == prodNums && prodNums.count() >= 1)
        return true;

    return false;
}







