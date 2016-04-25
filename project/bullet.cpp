#include "Object.h"
#include "bullet.h"

Bullet::Bullet()
{
    speed = 10;
    life = 5;
}

void Bullet::Update()
{
    QVector3D dir = parent->transform.GetRotation().rotatedVector(QVector3D(0,0,-1));
    parent->transform.Translate(dir * speed * GameTime::DeltaTime());
    life -= GameTime::DeltaTime();
}
