//
//  OpenGL widget
//
#include "Hw10opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>
#include <iostream>

using namespace std;

//  Set up array indexes for program
#define OLD_ARRAY 4
#define NEXT_ARRAY    5
const char* Name[] = {"","","","","old","next",NULL};

/*
 *  Random numbers with range and offset
 */
static float frand(float rng,float off)
{
   return rand()*rng/RAND_MAX+off;
}

//
//  Constructor
//
Hw10opengl::Hw10opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   init  = false;
   mouse = false;
   asp = 1;
   dim = 3;
   fov = 55;
   th = ph = 0;
   x0 = y0 = 0;
   z0 = 1;
   zh = 0;
   last = 0;
}

//
//  Reset view
//
void Hw10opengl::reset()
{
   th = ph = 0;
   dim = 3;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Initialize
//
void Hw10opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(shader[0],":/cgol.cs","","","");
   //Shader(shader[1],"",":/ex19b.vert","",":/ex19b.frag");

   buffers[0].create();
   buffers[0].bind();
   buffers[0].allocate(size*size*sizeof(unsigned int));
   buffers[1].create();
   buffers[1].bind();
   buffers[1].allocate(size*size*sizeof(unsigned int));

   //  Start 100 fps timer connected to updateGL
   move = true;
   timer.setInterval(10);
   connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
   timer.start();
   time.start();
}

//
//  Set projection when window is resized
//
void Hw10opengl::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = height ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Set projection
   Projection();
}

//
//  Draw the window
//
void Hw10opengl::paintGL()
{
   //  Wall time (seconds)
   float t = 0.001*time.elapsed();
   if (move) zh = fmod(90*t,360);

   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //  Set view
   glLoadIdentity();
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Compute shader
   int next = (last + 1) % 2;
   shader[0].bind();
   shader[0].setUniformValue("size",size);
   //shader[0].enableAttributeArray();
   //  Release shader
   shader[mode].release();
}

//
//  Throw a fatal error and die
//
void Hw10opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw10opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Hw10opengl::Projection()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-dim*asp, +dim*asp, -dim, +dim, -dim, +dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void Hw10opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Hw10opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Hw10opengl::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;  //  Change in mouse location
      th = (th+d.x())%360;      //  Translate x movement to azimuth
      ph = (ph+d.y())%360;      //  Translate y movement to elevation
      pos = e->pos();           //  Remember new location
      updateGL();               //  Request redisplay
   }
}

//
//  Mouse wheel
//
void Hw10opengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      dim += 0.1;
   //  Zoom in
   else if (dim>1)
      dim -= 0.1;
   //  Request redisplay
   Projection();
   updateGL();
}

//
//  Load shader
//
void Hw10opengl::Shader(QOpenGLShaderProgram& shader, QString compute, QString vert, QString geom, QString frag)
{
   QGLFunctions glf(QGLContext::currentContext());
   //  Compute shader
   if (compute.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Compute,compute))
       Fatal("Error compiling "+compute+"\n"+shader.log());
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Geometry shader
   if (geom.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Geometry,geom))
      Fatal("Error compiling "+geom+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QOpenGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Bind Attribute Locations
   for (int k=0;Name[k];k++)
      if (Name[k][0])
         glf.glBindAttribLocation(shader.programId(),k,Name[k]);
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
