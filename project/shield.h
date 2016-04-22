#ifndef SHIELD_H
#define SHIELD_H

#include "material.h"

class Shield : public Material
{
    QVector3D hitPoint;
    float hitTime;

public:
    Shield(SciShieldOpengl *context);
    void PreRender(QMatrix4x4 modelview, QMatrix3x3 norm);
    void Hit(QVector3D location);
    void Update();
};

#endif // SHIELD_H
