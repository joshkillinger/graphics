//
//  Generic Object class
//
#include "Object.h"

//
//  Constructor
//
Object::Object(SciShieldOpengl *context)
{
    transform = Transform();

    tint = QColor(200, 200, 200, 255);

    //  Texture
    tex = 0;

    glContext = context;
}

//
//  Enable texture
//
void Object::EnableTex()
{
   glBindTexture(GL_TEXTURE_2D,tex);
   if (tex) glEnable(GL_TEXTURE_2D);
}

//
//  Disable texture
//
void Object::DisableTex()
{
   if (tex) glDisable(GL_TEXTURE_2D);
}

//
//  Load shader
//
void Object::SetShader(QString vert, QString frag)
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
