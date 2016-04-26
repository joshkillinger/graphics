#include <math.h>
#include <iostream>
#include "particleobject.h"

using namespace std;

/*
 *  Random numbers with range and offset
 */
static float frand(float rng,float off)
{
   return rand()*rng/RAND_MAX+off;
}

ParticleObject::ParticleObject(SciShieldOpengl *context, int numParticles) : Object(context)
{
    vertexCount = numParticles;

    float *data = (float *)malloc(sizeof(float) * numParticles * 4);

    for (int i = 0; i < numParticles; i++)
    {
        data[(i*4)+0] = frand(2,-1); //x
        data[(i*4)+1] = frand(2,-1); //y
        data[(i*4)+2] = frand(2,-1); //z
        data[(i*4)+3] = frand(.5,0); //color
    }

    //  Copy data to vertex buffer object
    vertexBuffer.create();
    if (!vertexBuffer.bind())
    {
        cerr << "error binding vertexBuffer for particles" << endl;
    }
    vertexBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    cout << "allocating buffer space of " << sizeof(float) * numParticles * 4 << endl;

    vertexBuffer.allocate(sizeof(float) * numParticles * 4);
    vertexBuffer.write(0, data, sizeof(float) * numParticles * 4);

    cout << "particle vertex buffer is size " << vertexBuffer.size() << endl;

    //  Unbind this buffer
    vertexBuffer.release();

    free(data);
}

void ParticleObject::Render()
{
    //cout << "rendering particles" << endl;

    //   Attribute 0: vertex coordinate (vec3) at offset 0
    vertexBuffer.bind();
    material->shader.enableAttributeArray(0);
    material->shader.setAttributeBuffer(0,GL_FLOAT,0,3,sizeof(float)*4);

    material->shader.enableAttributeArray(1);
    material->shader.setAttributeBuffer(1,GL_FLOAT,3*sizeof(float),1,sizeof(float)*4);

    // Draw the points
    glDrawArrays(GL_POINTS,0,vertexCount);

    //  Disable vertex arrays
    material->shader.disableAttributeArray(0);
    material->shader.disableAttributeArray(1);

    vertexBuffer.release();
}
