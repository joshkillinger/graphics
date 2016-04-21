#ifndef SHIELD_H
#define SHIELD_H

#include "material.h"

class Shield : public Material
{
    QVector3D hitPoint;
    float hitTime;

public:
    Shield(SciShieldOpengl *context);
    void Hit(QVector3D location);
};

#endif // SHIELD_H
