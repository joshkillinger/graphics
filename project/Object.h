//
//  Object class
//  Dummy generic object class
//  Location of object is common to all objects
//  Translation is defined for all objects, but must
//  be implemented in the display function of each object
//  The display function is virtual and must be reimplemented
//
#ifndef OBJECT_H
#define OBJECT_H

#include <QtOpenGL>
#include "sci-shield-opengl.h"
#include "transform.h"
#include "material.h"

class Object
{
protected:
    Transform transform;
    Material *material;

    SciShieldOpengl *glContext;

    int vertexCount;
    QGLBuffer vertexBuffer;

    Object(SciShieldOpengl *context);          //  Constructor
public:
    void SetMaterial(Material *mat);
    void display(int stage);                         //  Render the object
    virtual ~Object() {};                             //  Do nothing destructor
protected:
//    void EnableTex();
//    void DisableTex();
//    int  Increment(int n);
    void Render();
};

#endif
