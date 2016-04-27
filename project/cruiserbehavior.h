#ifndef CRUISERBEHAVIOR_H
#define CRUISERBEHAVIOR_H

#include <QVector3D>
#include <QVector4D>
#include "behavior.h"

class CruiserBehavior : public Behavior
{
private:
    QVector3D gun[3];
    QVector3D size[3];
    QVector4D color[3];
    float lastShot[3];
    float frequency[3];

    void Fire(int index);

public:
    CruiserBehavior();
    void Update();
};

#endif // CRUISERBEHAVIOR_H
