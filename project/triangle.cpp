#include "triangle.h"
#include <QtOpenGL>
#include <iostream>

using namespace std;

const float triangle_data[] =  // Vertex data
{
//  X  Y  Z  W   Nx Ny Nz    R G B   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,  1,1,
   -1,+1,+1,+1,   0, 0,+1,  0,1,
   +1,-1,+1,+1,   0, 0,+1,  1,0,
   };

Triangle::Triangle(SciShieldOpengl *context) : Object(context)
{
    //
    //  Cube Vertexes
    //
    vertexCount = 3;

    //  Copy data to vertex buffer object
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    cout << "allocating buffer space of " << sizeof(triangle_data) << endl;

    vertexBuffer.allocate(sizeof(triangle_data));
    vertexBuffer.write(0,triangle_data, sizeof(triangle_data));
    //  Unbind this buffer
    vertexBuffer.release();

}

void Triangle::display()
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

    tex = glContext->bindTexture(qtex,GL_TEXTURE_2D);
    shader.setUniformValue("Texture",0);

    //   Attribute 0: vertex coordinate (vec3) at offset 0
    vertexBuffer.bind();
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(float)*8);

    shader.enableAttributeArray(1);
    shader.setAttributeBuffer(1,GL_FLOAT,3,3,sizeof(float)*8);

    shader.enableAttributeArray(2);
    shader.setAttributeBuffer(2,GL_FLOAT,6,2,sizeof(float)*8);

    // Draw the cube
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    //  Disable vertex arrays
    shader.disableAttributeArray(0);
    shader.disableAttributeArray(1);
    shader.disableAttributeArray(2);

    vertexBuffer.release();

    // Back to fixed pipeline
    shader.release();
}
