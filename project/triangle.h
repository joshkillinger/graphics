#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"

class Triangle : public Object
{
public:
    Triangle(SciShieldOpengl *context);
    void display();
};

#endif // TRIANGLE_H
