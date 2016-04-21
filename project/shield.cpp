#include "shield.h"

Shield::Shield(SciShieldOpengl *context)
    : Material(context, 0, 0, 0, 0)
{
    visibleStage = 1;
    hitTime = -1;
}

void Shield::Hit(QVector3D location)
{
    hitPoint = location;
    hitTime = 0;
}
