#ifndef HITBOX_H
#define HITBOX_H

#include <QtOpenGL>
#include "transform.h"

class Hitbox
{
private:
    Transform *parent;

public:
    Hitbox(){};
    void SetParent(Transform *t);

    virtual QVector3D IsHit(QVector3D origin, QVector3D direction);
};

#endif // HITBOX_H
