#ifndef FIGHTERBEHAVIOR_H
#define FIGHTERBEHAVIOR_H

#include <QVector3D>
#include "behavior.h"

class FighterBehavior : public Behavior
{
private:
    QVector3D left;
    QVector3D right;
    float lastShot;
    float frequency;
    float nextShotOffset;

    void Fire();

public:
    FighterBehavior();
    void Update();
};

#endif // FIGHTERBEHAVIOR_H
