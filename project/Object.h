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
#include "hitbox.h"

class Object
{
protected:
    Material *material;

    SciShieldOpengl *glContext;

    Hitbox *hitbox;

    int vertexCount;
    QGLBuffer vertexBuffer;

    int renderStage;

    Object(SciShieldOpengl *context);          //  Constructor
    void Render();
public:
    Transform transform;

    void SetMaterial(Material *mat);
    void SetHitbox(Hitbox *hb);
    void display(int stage);                         //  Render the object
    virtual ~Object() {};                             //  Do nothing destructor

    void Hit(QVector3D point);

    virtual void Update();

    float IsHit(QVector3D origin, QVector3D direction);

protected:
};

#endif
