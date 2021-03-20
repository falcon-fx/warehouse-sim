#include "target.h"

Target::Target(int id, int prodNum) {
    this->setID(id);
    this->productNumber = prodNum;
    this->setEmpty(false);
}

bool Target::isCorrectProduct(int prodNum) {
    return(prodNum == this->productNumber);
}
