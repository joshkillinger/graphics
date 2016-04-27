#include "Object.h"
#include "bullet.h"

Bullet::Bullet()
{
    Reset();
}

void Bullet::Reset()
{
    speed = 25;
    life = 10;
}

void Bullet::Update()
{
    QVector3D dir = parent->transform.GetRotation().rotatedVector(QVector3D(0,0,-1)).normalized();

    QVector3D origin = parent->transform.GetWorldPosition();

    int hitIndex = -1;
    float rayDistance = parent->glContext->TraceRay(origin,dir, &hitIndex);

    float travelDistance = speed * GameTime::DeltaTime();
    if (hitIndex != -1)
    {
        if (rayDistance < travelDistance)
        {
            // we hit something, inform it
            QVector3D hitPoint = origin + (dir * rayDistance);
            parent->glContext->objects[hitIndex]->Hit(hitPoint);
            parent->Hit(hitPoint);
        }
    }

    parent->transform.Translate(dir * travelDistance);
    life -= GameTime::DeltaTime();

    if (life < 0)
    {
        parent->Hit(parent->transform.GetWorldPosition());
    }
}
