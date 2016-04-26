#include "shield.h"
#include "gametime.h"
#include "Object.h"
#include <iostream>

using namespace std;

Shield::Shield(SciShieldOpengl *context)
    : Material(context, 0, 0, 0, 0)
{
    visibleStage = 1;
    for (int i = 0; i < 10; i++)
        hitTime[i] = -1;
    nextHit = 0;
}

void Shield::Hit(QVector3D location)
{
    //convert world space to model space
    hitPoint[nextHit] = parent->transform.GetModelMatrix().inverted() * location;
    hitTime[nextHit] = 0;

    nextHit = (nextHit + 1) % 10;

    cout << "Shield hit" << endl;
}

void Shield::PreRender(QMatrix4x4 modelview, QMatrix3x3 norm)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable shader
    shader.bind();
    //  Set Modelview and Projection Matrix
    shader.setUniformValue("ProjectionMatrix",glContext->proj);
    shader.setUniformValue("ModelViewMatrix",modelview);
    shader.setUniformValue("ViewMatrix",glContext->view);

    shader.setUniformValue("Tint",tint);

    shader.setUniformValueArray("HitPoint",hitPoint,10);
    shader.setUniformValueArray("Time",hitTime,10,1);

    tex = glContext->bindTexture(texture,GL_TEXTURE_2D);
    shader.setUniformValue("Texture",0);
}

void Shield::Update()
{
    for(int i = 0; i < 10; i++)
    {
        if (hitTime[i] >= 0)
        {
            hitTime[i] += GameTime::DeltaTime();
        }
    }
}
