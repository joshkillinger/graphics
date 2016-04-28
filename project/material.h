#ifndef MATERIAL_H
#define MATERIAL_H

#include <QtGui>
#include <QOpenGLFunctions>
#include "sci-shield-opengl.h"

class Object;

class Material
{
protected:
    SciShieldOpengl *glContext;
    Object *parent;

    QVector4D tint;
    float ambient;
    float diffuse;
    float specular;
    float shininess;

    QOpenGLTexture *texture;
    //QPixmap specmap;
    QOpenGLTexture *luminanceMap;
    //QPixmap bumpmap;

public:
    QOpenGLShaderProgram shader;   //  Shader
    int visibleStage;

    Material(SciShieldOpengl *context, float ambient, float diffuse, float specular, float shininess);
    void SetShader(QString vert, QString geom, QString frag);
    void SetTexture(QString file);
    void SetLuminanceMap(QString file);
    void SetTint(QVector4D color);
    void SetParent(Object *p);

    virtual void PreRender(QMatrix4x4 modelview, QMatrix3x3 norm);
    virtual void PostRender();

    virtual void Hit(QVector3D point){(void)point;};
    virtual void Update(){};
};

#endif // MATERIAL_H
