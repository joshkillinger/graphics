#include "shield.h"
#include "gametime.h"
#include <iostream>

using namespace std;

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

    cout << "Shield hit" << endl;
}

void Shield::PreRender(QMatrix4x4 modelview, QMatrix3x3 norm)
{
    // Enable shader
    shader.bind();
    //  Set Modelview and Projection Matrix
    shader.setUniformValue("ProjectionMatrix",glContext->proj);
    shader.setUniformValue("ModelViewMatrix",modelview);
    shader.setUniformValue("NormalMatrix",norm);

    shader.setUniformValue("Tint",tint);

    shader.setUniformValue("HitPoint",hitPoint);
    shader.setUniformValue("Time",hitTime);

    tex = glContext->bindTexture(texture,GL_TEXTURE_2D);
    shader.setUniformValue("Texture",0);
}

void Shield::Update()
{
    if (hitTime < 0)
    {
        return;
    }

    hitTime += GameTime::DeltaTime();
}
