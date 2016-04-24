#include "hitbox.h"

void Hitbox::SetParent(Transform *p)
{
    parent = p;
}

float Hitbox::IsHit(QVector3D origin, QVector3D direction)
{
    return -1;
}
