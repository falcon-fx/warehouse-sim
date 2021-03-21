#ifndef OBJECT_H
#define OBJECT_H

enum ObjectType {
    OBJECT_
};

class Object
{
public:
    Object();
    void setID(int id) { this->id = id; }
    int getID() { return this->id; }
private:
    int id;
};

#endif // OBJECT_H
