#ifndef SQUADRON_H
#define SQUADRON_H

#include <QtGui>
#include "behavior.h"

class Squadron : public Behavior
{
private:
    float radius;
    float speed;
    QVector3D center;

public:
    Squadron(QVector3D center);
    void Update();
};

#endif // SQUADRON_H
