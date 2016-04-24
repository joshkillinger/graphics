#include "hitbox.h"

void Hitbox::SetParent(Transform *p)
{
    parent = p;
}

QVector3D Hitbox::IsHit(QVector3D origin, QVector3D direction)
{
    return QVector3D();
}
