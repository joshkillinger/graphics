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
