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
#include "behavior.h"

class Object
{
protected:
    Material *material;
    Hitbox *hitbox;
    Behavior *behavior;

    int vertexCount;
    QGLBuffer vertexBuffer;

    virtual void Render();
public:
    Transform transform;
    SciShieldOpengl *glContext;

    Object(SciShieldOpengl *context);          //  Constructor

    void SetMaterial(Material *mat);
    Material *GetMaterial();
    void SetHitbox(Hitbox *hb);
    void SetBehavior(Behavior *b);
    void display(int stage);                         //  Render the object
    virtual ~Object() {};                             //  Do nothing destructor

    virtual void Hit(QVector3D point);

    virtual void Update();

    float IsHit(QVector3D origin, QVector3D direction);

protected:
};

#endif
