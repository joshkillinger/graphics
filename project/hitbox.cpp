#include "hitbox.h"

void Hitbox::SetParent(Transform *p)
{
    parent = p;
}

float Hitbox::IsHit(QVector3D origin, QVector3D direction)
{
    (void)origin;
    (void)direction;
    return -1;
}
