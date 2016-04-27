#include "material.h"
#include "Object.h"

Material::Material(SciShieldOpengl *context, float ambient, float diffuse, float specular, float shininess)
{
    tint = QVector4D(1, 1, 1, 1);
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;

    visibleStage = 0; //default value

    glContext = context;

    texture = NULL;
    luminanceMap = NULL;
}

//
//  Load shader
//
void Material::SetShader(QString vert, QString geom, QString frag)
{
    //  Vertex shader
    if (vert.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Vertex,vert))
        glContext->Fatal("Error compiling "+vert+"\n"+shader.log(),"Object");
    //  Geometry shader
    if (geom.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Geometry,geom))
        glContext->Fatal("Error compiling "+geom+"\n"+shader.log(),"Object");
    //  Fragment shader
    if (frag.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Fragment,frag))
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

void Material::SetLuminanceMap(QString file)
{
    // Texture
    luminanceMap = new QOpenGLTexture(QImage(file).mirrored());
    luminanceMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    luminanceMap->setMagnificationFilter(QOpenGLTexture::Linear);
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
    shader.setUniformValue("ViewMatrix",glContext->view);
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
        texture->bind(0);
    }
    if (luminanceMap != NULL)
    {
        luminanceMap->bind(1);
    }
    shader.setUniformValue("Texture",0);
    shader.setUniformValue("LuminanceMap",1);
}

void Material::PostRender()
{
    if (texture != NULL)
    {
        texture->release(0);
    }
    if (luminanceMap != NULL)
    {
        luminanceMap->release(1);
    }

    shader.release();
}

void Material::SetParent(Object *p)
{
    parent = p;
}
