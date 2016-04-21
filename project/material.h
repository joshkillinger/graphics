#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtOpenGL>
#include "sci-shield-opengl.h"

class Material
{
protected:
    SciShieldOpengl *glContext;

    QVector4D tint;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    QPixmap texture;
    //QPixmap specmap;
    //QPixmap illummap;
    //QPixmap bumpmap;
    unsigned int tex;  //  Texture
    //unsigned int spec;  //  Texture
    //unsigned int illum;  //  Texture
    //unsigned int bump;  //  Texture

public:
    QGLShaderProgram shader;   //  Shader
    int visibleStage;

    Material(SciShieldOpengl *context, float ambient, float diffuse, float specular, float shininess);
    void SetShader(QString vert, QString frag);
    void SetTexture(QString file);
    void SetTint(QVector4D color);

    virtual void PreRender(QMatrix4x4 modelview, QMatrix3x3 norm);
    virtual void PostRender();

    virtual void Hit(QVector3D point);
};

#endif // MATERIAL_H
