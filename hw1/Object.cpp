//
//  Generic Object class
//
#include "Object.h"

//
//  Constructor
//
Object::Object(float x,float y,float z)
{
   x0 = x;
   y0 = y;
   z0 = z;
}

//
//  Set translation
//
void Object::translation(float x,float y,float z)
{
   x0 = x;
   y0 = y;
   z0 = z;
}

void Object::setX(float x)
{
    x0 = x;
}

void Object::setY(float y)
{
    y0 = y;
}

void Object::setZ(float z)
{
    z0 = z;
}
