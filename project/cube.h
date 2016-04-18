#ifndef CUBE_H
#define CUBE_H

#include "Object.h"

class Cube : public Object
{
public:
    Cube(SciShieldOpengl *context);
    void display();
};

#endif // CUBE_H
