#ifndef TARGET_H
#define TARGET_H

#include "wtile.h"
#include <QObject>

class Target : public WTile {
    Q_OBJECT
public:
    Target(int id, int prodNum);
    bool isCorrectProduct(int prodNum);
private:
    int productNumber;
};

#endif // TARGET_H
