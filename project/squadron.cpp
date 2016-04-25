#include "Object.h"
#include "squadron.h"
#include <math.h>
#include <iostream>

using namespace std;

#define PI 3.14159265358979
#define DEGREES_TO_RAD PI / 180
#define RAD_TO_DEGREES 180 / PI

Squadron::Squadron()
{
    radius = 10;
    speed = .75;
}

void Squadron::Update()
{
    QVector3D pos = parent->transform.GetPosition();
    pos.setX(radius * cos(GameTime::Time()*speed));
    pos.setZ(-radius * sin(GameTime::Time()*speed));
    parent->transform.SetPosition(pos);

    parent->transform.Rotate(speed * GameTime::DeltaTime() * RAD_TO_DEGREES,QVector3D(0,1,0));
}
