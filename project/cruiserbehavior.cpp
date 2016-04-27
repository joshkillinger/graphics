#include <iostream>
#include "gametime.h"
#include "cruiserbehavior.h"
#include "Object.h"

#define PI 3.14159265358979
#define DEGREES_TO_RAD PI / 180
#define RAD_TO_DEGREES 180 / PI

using namespace std;

static float frand(float rng,float off)
{
   return rand()*rng/RAND_MAX+off;
}

CruiserBehavior::CruiserBehavior()
{
    gun[0] = QVector3D(.8f,-.07f,1.125f);
    gun[1] = QVector3D(1,-.07f,-.3f);
    gun[2] = QVector3D(1.2f,-.07f,-.7f);

    size[0] = QVector3D(.8f,.8f,.8f);
    size[1] = QVector3D(.4f,.4f,.4f);
    size[2] = QVector3D(.4f,.4f,.4f);

    color[0] = QVector4D(1,1,.4f,1);
    color[1] = QVector4D(1,.4f,1,1);
    color[2] = QVector4D(1,.4f,1,1);

    frequency[0] = 4;
    frequency[1] = .8f;
    frequency[2] = .8f;

    lastShot[0] = frequency[0] * .9f;
    lastShot[1] = frequency[1] * .5f;
    lastShot[2] = frequency[2];
}


void CruiserBehavior::Update()
{
    for (int i = 0; i < 3; i++)
    {
        lastShot[i] -= GameTime::DeltaTime();

        if (lastShot[i] < 0)
        {
            Fire(i);
        }
    }
}

void CruiserBehavior::Fire(int index)
{
    lastShot[index] += frequency[index];

    cout << "firing!" << endl;

    QVector3D jitter(0,frand(.25f,-.125f),frand(.25f,-.125f));

    QVector3D direction = parent->transform.GetModelMatrix().mapVector(QVector3D(1,0,0)+jitter).normalized();
    QVector3D origin = parent->transform.GetModelMatrix().mapVector(gun[index]);
    origin += parent->transform.GetWorldPosition();

    parent->glContext->Fire(origin, direction, size[index], color[index]);
}
