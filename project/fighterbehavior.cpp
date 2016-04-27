#include <iostream>
#include "gametime.h"
#include "fighterbehavior.h"
#include "Object.h"

#define PI 3.14159265358979
#define DEGREES_TO_RAD PI / 180
#define RAD_TO_DEGREES 180 / PI

using namespace std;

static float frand(float rng,float off)
{
   return rand()*rng/RAND_MAX+off;
}

FighterBehavior::FighterBehavior()
{
    left = QVector3D(-30,-2,28);
    right = QVector3D(-30,-2,-28);
    frequency = 2 * PI / .75f;
    lastShot = frequency * .3;
    nextShotOffset = frand(.5f, -.25f);
}


void FighterBehavior::Update()
{
    lastShot -= GameTime::DeltaTime();

    if ((lastShot + nextShotOffset) < 0)
    //if (lastShot < 0)
    {
        Fire();
    }
}

void FighterBehavior::Fire()
{
    lastShot += frequency;
    nextShotOffset = frand(.5f, -.25f);

    cout << "firing!" << endl;

    QVector3D direction = parent->transform.GetModelMatrix().mapVector(QVector3D(-1,0,0)).normalized();
    QVector3D leftOrigin = parent->transform.GetModelMatrix().mapVector(left);
    leftOrigin += parent->transform.GetWorldPosition();
    QVector3D rightOrigin = parent->transform.GetModelMatrix().mapVector(right);
    rightOrigin += parent->transform.GetWorldPosition();

    parent->glContext->Fire(leftOrigin, direction, QVector3D(.5f,.5f,.5f), QVector4D(1,.4f,.4f,1));
    parent->glContext->Fire(rightOrigin, direction, QVector3D(.5f,.5f,.5f), QVector4D(1,.4f,.4f,1));
}
