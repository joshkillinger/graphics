#ifndef QUAD_H
#define QUAD_H

#include "Object.h"

class Quad: public Object
{
private:
   float sx,sz;  //  Dimensions in X/Y/Z directions
public:
   Quad();                                  //  Constructor
   void scale(float dx,float dy,float dz);  //  Set scale
   void display();                          //  Render the cube
};

#endif // QUAD_H
