//
//  OpenGL widget
//
#include "hw7opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include "Cube.h"
#include "WaveOBJ.h"
#include "Teapot.h"
#include <math.h>
#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

//
//  Draw vertex in polar coordinates
//
static void Vertex(double th,double ph)
{
   glVertex3d(Sin(th)*Cos(ph),Cos(th)*Cos(ph),Sin(ph));
}

//
//  Draw a ball at (x,y,z) radius r
//
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Bands of latitude
   for (int ph=-90;ph<90;ph+=10)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=20)
      {
         Vertex(th,ph);
         Vertex(th,ph+10);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

//
//  Constructor
//
Hw7opengl::Hw7opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   N = 1;
   init  = false;
   mouse = false;
   asp = 1;
   dim = 3;
   fov = 55;
   th = ph = 0;
   x0 = y0 = 0;
   z0 = 1;
   zh = 0;
   framebuf[0] = 0;
   framebuf[1] = 0;
}

//
//  Reset view
//
void Hw7opengl::reset()
{
   th = ph = 0;
   dim = 3;
   //  Request redisplay
   //updateGL();
}

//
//  Light animation
//
void Hw7opengl::setLightMove(bool on)
{
   move = on;
   //updateGL();
}

//
//  Set passes
//
void Hw7opengl::setPasses(int n)
{
   N = n;
   //  Request redisplay
   //updateGL();
}

//
//  Set shader
//
void Hw7opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   //updateGL();
}

//
//  Set light position
//
void Hw7opengl::setPos(int Zh)
{
   zh = Zh;
   //  Request redisplay
   //updateGL();
}

//
//  Set light elevation
//
void Hw7opengl::setElev(int Z)
{
   z0 = 0.02*Z;
   //  Request redisplay
   //updateGL();
}

//
//  Set projection
//
void Hw7opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   //  Request redisplay
   //updateGL();
}

//
//  Initialize
//
void Hw7opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(shader[1],"",":/ex11a.frag");
   Shader(shader[2],"",":/ex11b.frag");
   Shader(shader[3],"",":/ex11c.frag");
   Shader(shader[4],"",":/ex11d.frag");
   Shader(shader[5],"",":/ex11e.frag");
   Shader(shader[6],"",":/ex11f.frag");
   Shader(shader[7],"",":/ex11g.frag");
   Shader(shader[8],"",":/ex11h.frag");

   // Cube
   Cube* cube = new Cube();
   cube->scale(0.5,0.5,0.5);
   cube->translate(1,1,0);
   objects.push_back(cube);

   // Teapot
   Teapot* pot = new Teapot(8);
   pot->scale(0.3);
   pot->texture(":/water.png");
   pot->translate(-1,1,0);
   objects.push_back(pot);

   // Cruiser
   WaveOBJ* cruiser=0;
   try
   {
      cruiser = new WaveOBJ("cruiser.obj",":/");
   }
   catch (QString err)
   {
      Fatal("Error loading object\n"+err);
   }
   if (cruiser)
   {
      cruiser->color(1,1,0);
      cruiser->scale(1.0);
      cruiser->translate(0,-1,0);
      objects.push_back(cruiser);
   }

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
void Hw7opengl::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = height ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Allocate frame buffer objects
   for (int k=0;k<2;k++)
   {
      //  Allocate frame buffer objects
      if (framebuf[k]) delete framebuf[k];
      framebuf[k] = new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth);
   }
   dX = 1.0/width;
   dY = 1.0/height;
}

//
//  Draw the window
//
void Hw7opengl::paintGL()
{
   //  Wall time (seconds)
   float t = 0.001*time.elapsed();
   if (move) zh = fmod(90*t,360);

   //  Set projection
   Projection();
   //  Set view
   glLoadIdentity();
   if (fov) glTranslated(0,0,-2*dim);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Translate intensity to color vectors
   float Ambient[]  = {0.3,0.3,0.3,1.0};
   float Diffuse[]  = {0.8,0.8,0.8,1.0};
   float Specular[] = {1.0,1.0,1.0,1.0};
   float Position[] = {(float)(3*Cos(zh)),z0,(float)(3*Sin(zh)),1.0};

   //  Send output to framebuf[0]
   if (mode) framebuf[0]->bind();


   //  Z-buffer
   glEnable(GL_DEPTH_TEST);
   //  Clear screen and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //  Draw light position (no lighting yet)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);

   //  Draw scene
   glPushMatrix();
   for (int k=0;k<objects.size();k++)
      objects[k]->display();
   glPopMatrix();

   //  Disable lighting and depth
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);

   //  Apply shader
   if (mode)
   {
      //  Reset projections
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //  Enable shader
      shader[mode].bind();

      //  Set shader increments
      shader[mode].setUniformValue("dX",dX);
      shader[mode].setUniformValue("dY",dY);

      //  Ping-Pong
      for (int k=0;k<N;k++)
      {
         int last = k%2;
         int next = 1-last;
         //  Set output to next framebuffer except for the last pass
         if (k+1<N)
            framebuf[next]->bind();
         else
            framebuf[last]->release();
         //  Get the texture
         glBindTexture(GL_TEXTURE_2D,framebuf[last]->texture());
         //  Exercise shader
         glClear(GL_COLOR_BUFFER_BIT);
         glBegin(GL_QUADS);
         glTexCoord2f(0,0); glVertex2f(-1,-1);
         glTexCoord2f(1,0); glVertex2f(+1,-1);
         glTexCoord2f(1,1); glVertex2f(+1,+1);
         glTexCoord2f(0,1); glVertex2f(-1,+1);
         glEnd();
      }

      //  Release shader
      shader[mode].release();
   }
   
   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
   //  Emit light angle
   emit light((int)zh);
}

//
//  Throw a fatal error and die
//
void Hw7opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw7opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Hw7opengl::Projection()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (fov)
   {
      float zmin = dim/4;
      float zmax = 4*dim;
      float ydim = zmin*tan(fov*M_PI/360);
      float xdim = ydim*asp;
      glFrustum(-xdim,+xdim,-ydim,+ydim,zmin,zmax);
   }
   else
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
void Hw7opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Hw7opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Hw7opengl::mouseMoveEvent(QMouseEvent* e)
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
void Hw7opengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      dim += 0.1;
   //  Zoom in
   else if (dim>1)
      dim -= 0.1;
   //  Request redisplay
   updateGL();
}

//
//  Load shader
//
void Hw7opengl::Shader(QGLShaderProgram& shader,QString vert,QString frag)
{
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
