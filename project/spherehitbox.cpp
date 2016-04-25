#include <algorithm>
#include <iostream>
#include "spherehitbox.h"

using namespace std;

float SphereHitbox::IsHit(QVector3D origin, QVector3D direction)
{
    QVector3D h = parent->GetWorldPosition() - origin;

    float mu = QVector3D::dotProduct(h,direction);

    float delta = (mu*mu) - QVector3D::dotProduct(h,h) + (r*r);

    //cout << "mu = " << mu << ", delta = " << delta << endl;

    if (delta <= 0)
    {
        return -1;
    }
    else
    {
        float t1 = mu - sqrt(delta);
        float t2 = mu + sqrt(delta);

        //cout << "t1 = " << t1 << ", t2 = " << t2 << endl;

        return min(t1, t2);
    }
}
