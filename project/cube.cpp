#include "cube.h"
#include <QtOpenGL>

const float cube_data[] =  // Vertex data
{
//  X  Y  Z  W   Nx Ny Nz    R G B   s t
   //  Front
   +1,+1,+1,   0, 0,+1,  1,1,
   -1,+1,+1,   0, 0,+1,  0,1,
   +1,-1,+1,   0, 0,+1,  1,0,
   -1,+1,+1,   0, 0,+1,  0,1,
   +1,-1,+1,   0, 0,+1,  1,0,
   -1,-1,+1,   0, 0,+1,  0,0,
   //  Back
   -1,-1,-1,   0, 0,-1,  1,0,
   +1,-1,-1,   0, 0,-1,  0,0,
   -1,+1,-1,   0, 0,-1,  1,1,
   +1,-1,-1,   0, 0,-1,  0,0,
   -1,+1,-1,   0, 0,-1,  1,1,
   +1,+1,-1,   0, 0,-1,  0,1,
   //  Right
   +1,+1,+1,  +1, 0, 0,  0,1,
   +1,-1,+1,  +1, 0, 0,  0,0,
   +1,+1,-1,  +1, 0, 0,  1,1,
   +1,-1,+1,  +1, 0, 0,  0,0,
   +1,+1,-1,  +1, 0, 0,  1,1,
   +1,-1,-1,  +1, 0, 0,  1,0,
   //  Left
   -1,+1,+1,  -1, 0, 0,  1,1,
   -1,+1,-1,  -1, 0, 0,  0,1,
   -1,-1,+1,  -1, 0, 0,  1,0,
   -1,+1,-1,  -1, 0, 0,  0,1,
   -1,-1,+1,  -1, 0, 0,  1,0,
   -1,-1,-1,  -1, 0, 0,  0,0,
   //  Top
   +1,+1,+1,   0,+1, 0,  1,0,
   +1,+1,-1,   0,+1, 0,  1,1,
   -1,+1,+1,   0,+1, 0,  0,0,
   +1,+1,-1,   0,+1, 0,  1,1,
   -1,+1,+1,   0,+1, 0,  0,0,
   -1,+1,-1,   0,+1, 0,  0,1,
   //  Bottom
   -1,-1,-1,   0,-1, 0,  0,0,
   +1,-1,-1,   0,-1, 0,  1,0,
   -1,-1,+1,   0,-1, 0,  0,1,
   +1,-1,-1,   0,-1, 0,  1,0,
   -1,-1,+1,   0,-1, 0,  0,1,
   +1,-1,+1,   0,-1, 0,  1,1,
   };

Cube::Cube(SciShieldOpengl *context) : Object(context)
{
    //
    //  Cube Vertexes
    //
    vertexCount = 36;

    // Texture
    texture = QPixmap(":/crate.png");
    tex = glContext->bindTexture(texture,GL_TEXTURE_2D);

    //  Cube vertex buffer object
    //  Copy data to vertex buffer object
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    vertexBuffer.allocate(sizeof(cube_data));
    vertexBuffer.write(0, cube_data, sizeof(cube_data));
    //  Unbind this buffer
    vertexBuffer.release();
}

void Cube::display()
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

    // Back to fixed pipeline
    shader.release();
}
