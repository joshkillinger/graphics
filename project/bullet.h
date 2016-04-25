#ifndef BULLET_H
#define BULLET_H

#include "behavior.h"

class Bullet : public Behavior
{
private:
    float speed;
    float life;

public:
    Bullet();
    void Update();
};

#endif // BULLET_H
