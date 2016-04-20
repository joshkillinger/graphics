#include "triangle.h"
#include <QtOpenGL>
#include <iostream>

using namespace std;

const float triangle_data[] =  // Vertex data
{
//  X  Y  Z   Nx Ny Nz    s  t
   //  Front
   +1,+1,+1,   0, 0,+1,   1, 1,
   -1,+1,+1,   0, 0,+1,   0, 1,
   +1,-1,+1,   0, 0,+1,   1, 0,
   };

Triangle::Triangle(SciShieldOpengl *context) : Object(context)
{
    //
    //  Cube Vertexes
    //
    vertexCount = 3;

    //  Copy data to vertex buffer object
    vertexBuffer.create();
    if (!vertexBuffer.bind())
    {
        cerr << "error binding vertexBuffer" << endl;
    }
    vertexBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    cout << "allocating buffer space of " << sizeof(triangle_data) << endl;

    vertexBuffer.allocate(sizeof(triangle_data));
    vertexBuffer.write(0,triangle_data, sizeof(triangle_data));

    cout << "triangle vertex buffer is size " << vertexBuffer.size() << endl;

    //  Unbind this buffer
    vertexBuffer.release();

}
