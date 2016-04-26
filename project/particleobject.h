#ifndef PARTICLEOBJECT_H
#define PARTICLEOBJECT_H

#include "Object.h"

class ParticleObject : public Object
{
protected:
    float lifetime;

    void Render();

public:
    ParticleObject(SciShieldOpengl *context, int numParticles);
};

#endif // PARTICLEOBJECT_H
