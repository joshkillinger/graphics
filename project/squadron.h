#ifndef SQUADRON_H
#define SQUADRON_H

#include "behavior.h"

class Squadron : public Behavior
{
private:
    float radius;
    float speed;

public:
    Squadron();
    void Update();
};

#endif // SQUADRON_H
