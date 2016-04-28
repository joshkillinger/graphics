#include "plasmamaterial.h"
#include "gametime.h"

PlasmaMaterial::PlasmaMaterial(SciShieldOpengl *context, float ambient, float diffuse, float specular, float shininess)
    : Material(context, ambient, diffuse, specular, shininess)
{
    visibleStage = 1;
    time = 0;
}

void PlasmaMaterial::PreRender(QMatrix4x4 modelview, QMatrix3x3 norm)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Enable shader
    shader.bind();
    //  Set Modelview and Projection Matrix
    shader.setUniformValue("ProjectionMatrix",glContext->proj);
    shader.setUniformValue("ModelViewMatrix",modelview);
    shader.setUniformValue("ViewMatrix",glContext->view);

    shader.setUniformValue("Tint",tint);

    shader.setUniformValue("Time",time);

    if (texture != NULL)
    {
        texture->bind();
    }
    shader.setUniformValue("Texture",0);
}

void PlasmaMaterial::Update()
{
    if (time >= 0)
    {
        time += GameTime::DeltaTime();
    }
}
