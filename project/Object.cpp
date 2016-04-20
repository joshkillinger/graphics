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

    visibleStage = 0; //default value

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

void Object::display(int stage)
{
    if (stage == visibleStage)
    {
        PreRender();
        Render();
        PostRender();
    }
}

void Object::PreRender()
{
    QMatrix4x4 modelview = glContext->view * transform.GetModelMatrix();
    QMatrix3x3 norm =  modelview.normalMatrix();

    // Enable shader
    shader.bind();
    //  Set Modelview and Projection Matrix
    shader.setUniformValue("ProjectionMatrix",glContext->proj);
    shader.setUniformValue("ModelViewMatrix",modelview);
    shader.setUniformValue("NormalMatrix",norm);

    shader.setUniformValue("Light.Position",glContext->Light.Position);
    shader.setUniformValue("Light.Ambient",glContext->Light.Ambient);
    shader.setUniformValue("Light.Diffuse",glContext->Light.Diffuse);
    shader.setUniformValue("Light.Specular",glContext->Light.Specular);

    shader.setUniformValue("shininess",32.0f);

    tex = glContext->bindTexture(texture,GL_TEXTURE_2D);
    shader.setUniformValue("Texture",0);
}

void Object::Render()
{
    //   Attribute 0: vertex coordinate (vec3) at offset 0
    vertexBuffer.bind();
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(float)*8);

    shader.enableAttributeArray(1);
    shader.setAttributeBuffer(1,GL_FLOAT,3*sizeof(float),3,sizeof(float)*8);

    shader.enableAttributeArray(2);
    shader.setAttributeBuffer(2,GL_FLOAT,6*sizeof(float),2,sizeof(float)*8);

    // Draw the cube
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    //  Disable vertex arrays
    shader.disableAttributeArray(0);
    shader.disableAttributeArray(1);
    shader.disableAttributeArray(2);

    vertexBuffer.release();
}

void Object::PostRender()
{
    // Back to fixed pipeline
    shader.release();
}
