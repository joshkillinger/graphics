#ifndef PLASMAMATERIAL_H
#define PLASMAMATERIAL_H

#include "material.h"

class PlasmaMaterial : public Material
{
    float time;
public:
    PlasmaMaterial(SciShieldOpengl *context, float ambient, float diffuse, float specular, float shininess);

    void PreRender(QMatrix4x4 modelview, QMatrix3x3 norm);
    void Update();
};

#endif // PLASMAMATERIAL_H
