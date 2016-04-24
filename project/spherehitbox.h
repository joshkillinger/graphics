#ifndef SPHEREHITBOX_H
#define SPHEREHITBOX_H

#include "hitbox.h"

class SphereHitbox : public Hitbox
{
private:
    float r;

public:
    SphereHitbox(float size = 1){r = size;};
    float IsHit(QVector3D origin, QVector3D direction);
};

#endif // SPHEREHITBOX_H
