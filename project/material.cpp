#include "material.h"

Material::Material(SciShieldOpengl *context, float ambient, float diffuse, float specular, float shininess)
{
    tint = QVector4D(1, 1, 1, 1);
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;

    //  Texture
    tex = 0;
    //spec = 0;
    //bump = 0;
    //illum = 0;

    visibleStage = 0; //default value

    glContext = context;
}

//
//  Load shader
//
void Material::SetShader(QString vert, QString frag)
{
    //  Vertex shader
    if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
        glContext->Fatal("Error compiling "+vert+"\n"+shader.log(),"Object");
    //  Fragment shader
    if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
        glContext->Fatal("Error compiling "+frag+"\n"+shader.log(),"Object");
    //  Link
    if (!shader.link())
        glContext->Fatal("Error linking shader\n"+shader.log(),"Object");
}

void Material::SetTexture(QString file)
{
    // Texture
    texture = QPixmap(file);
    tex = glContext->bindTexture(texture,GL_TEXTURE_2D);
}

void Material::SetTint(QVector4D color)
{
    tint = color;
}

void Material::PreRender(QMatrix4x4 modelview, QMatrix3x3 norm)
{
    // Enable shader
    shader.bind();
    //  Set Modelview and Projection Matrix
    shader.setUniformValue("ProjectionMatrix",glContext->proj);
    shader.setUniformValue("ModelViewMatrix",modelview);
    shader.setUniformValue("NormalMatrix",norm);

    shader.setUniformValue("Light.Position",glContext->Light.Position);
    shader.setUniformValue("Light.Color",glContext->Light.Color);

    shader.setUniformValue("Tint",tint);
    shader.setUniformValue("Ambient",ambient);
    shader.setUniformValue("Diffuse",diffuse);
    shader.setUniformValue("Specular",specular);
    shader.setUniformValue("Shininess",shininess);

    tex = glContext->bindTexture(texture,GL_TEXTURE_2D);
    shader.setUniformValue("Texture",0);
}

void Material::PostRender()
{
    shader.release();
}
