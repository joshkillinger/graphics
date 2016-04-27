#ifndef BEHAVIOR_H
#define BEHAVIOR_H

//#include "Object.h"
#include "gametime.h"

class Object;

class Behavior
{
protected:
    Object *parent;
public:
    Behavior();
    void SetParent(Object *parent);
    virtual void Update()=0;
    virtual void Reset(){};
};

#endif // BEHAVIOR_H
