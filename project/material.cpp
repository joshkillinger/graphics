#include "material.h"
#include "Object.h"

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

    texture = NULL;
}

//
//  Load shader
//
void Material::SetShader(QString vert, QString geom, QString frag)
{
    //  Vertex shader
    if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
        glContext->Fatal("Error compiling "+vert+"\n"+shader.log(),"Object");
    //  Geometry shader
    if (geom.length() && !shader.addShaderFromSourceFile(QGLShader::Geometry,geom))
        glContext->Fatal("Error compiling "+geom+"\n"+shader.log(),"Object");
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
    texture = new QOpenGLTexture(QImage(file).mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
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

    shader.setUniformValue("Light.Direction",glContext->Light.Direction);
    shader.setUniformValue("Light.Color",glContext->Light.Color);

    shader.setUniformValue("Tint",tint);
    shader.setUniformValue("Ambient",ambient);
    shader.setUniformValue("Diffuse",diffuse);
    shader.setUniformValue("Specular",specular);
    shader.setUniformValue("Shininess",shininess);

    if (texture != NULL)
    {
        texture->bind();
    }
    shader.setUniformValue("Texture",0);
}

void Material::PostRender()
{
    shader.release();
}

void Material::Hit(QVector3D point)
{}

void Material::SetParent(Object *p)
{
    parent = p;
}
