//
//  OpenGL widget
//
#include "hw9opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <QGLFunctions>
#include <math.h>
#include <iostream>

using namespace std;

//  Set up array indexes for program
#define VELOCITY_ARRAY 4
#define START_ARRAY    5
#define AMPFREQ_ARRAY  6
#define LIFETIME_ARRAY 7
const char* Name[] = {"","","","","Vel","Start","AmpFreq","Lifetime",NULL};

/*
 *  Random numbers with range and offset
 */
static float frand(float rng,float off)
{
   return rand()*rng/RAND_MAX+off;
}

/*
 *  Initialize particles
 */
void Hw9opengl::InitPart(void)
{
    //  Array Pointers
    float* vert  = Vert;
    float* color = Color;
    float* vel   = Vel;
    float* start = Start;
    float* ampfreq = AmpFreq;
    float* lifetime = Lifetime;

    //  Loop over NxN patch
    n = mode ? 15 : N;
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            if (mode < 2)
            {
                //  Location x,y,z
                *vert++ = (i+0.5)/n-0.75;
                *vert++ = 0;
                *vert++ = (j+0.5)/n-0.75;
                //  Color r,g,b (0.5-1.0)
                *color++ = frand(0.5,0.5);
                *color++ = frand(0.5,0.5);
                *color++ = frand(0.5,0.5);
                //  Velocity
                *vel++ = frand( 1.0,3.0);
                *vel++ = frand(10.0,0.0);
                *vel++ = frand( 1.0,3.0);
                *vel++ = 0.0;
                //  Launch time
                *start++ = frand(2.0,0.0);
            }
            else
            {
                //  Location x,y,z
                *vert++ = ((i+0.5)/n-0.75) * 2;
                *vert++ = 1.0;
                *vert++ = ((j+0.5)/n-0.75) * 2;
                //  Color r,g,b (0.5-1.0)
                *color++ = 0.0;
                *color++ = 0.0;
                *color++ = 0.0;
                //  Velocity
                *vel++ = 0.0;
                *vel++ = -.2;//frand( 0.5,-0.5);
                *vel++ = 0.0;
                *vel++ = frand(10.0,-5.0);
                //  Launch time
                *start++ = frand(10.0,0.0);
                //  Lifetime
                *lifetime++ = 10.0;
                //  Amplitude
                *ampfreq++ = frand(.5,-.25);
                *ampfreq++ = frand(.5,-.25);
                //  Frequency
                *ampfreq++ = frand(2.0,0.0);
                *ampfreq++ = frand(2.0,0.0);

            }
        }
    }
}

/*
 *  Draw particles
 */
void Hw9opengl::DrawPart(void)
{
    QGLFunctions glf(QGLContext::currentContext());
    //  Set particle size
    if (mode < 2)
    {
        int size = 2;
        if (mode == 1) size = 50;
        glPointSize(size);
        //  Point vertex location to local array Vert
        glVertexPointer(3,GL_FLOAT,0,Vert);
        //  Point color array to local array Color
        glColorPointer(3,GL_FLOAT,0,Color);
    }
    else if (mode == 2)
    {
        //  Point vertex location to local array Vert
        glVertexPointer(3,GL_FLOAT,3*sizeof(float),Vert);
        //  Point color array to local array Color
        glColorPointer(3,GL_FLOAT,3*sizeof(float),Color);
    }
    //  Point attribute arrays to local arrays
    glf.glVertexAttribPointer(VELOCITY_ARRAY,4,GL_FLOAT,GL_FALSE,4*sizeof(float),Vel);
    glf.glVertexAttribPointer(START_ARRAY,1,GL_FLOAT,GL_FALSE,sizeof(float),Start);
    glf.glVertexAttribPointer(AMPFREQ_ARRAY,4,GL_FLOAT,GL_FALSE,4*sizeof(float),AmpFreq);
    glf.glVertexAttribPointer(LIFETIME_ARRAY,1,GL_FLOAT,GL_FALSE,sizeof(float),Lifetime);
    //  Enable arrays used by DrawArrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glf.glEnableVertexAttribArray(VELOCITY_ARRAY);
    glf.glEnableVertexAttribArray(START_ARRAY);
    glf.glEnableVertexAttribArray(AMPFREQ_ARRAY);
    glf.glEnableVertexAttribArray(LIFETIME_ARRAY);
    //  Set transparent large particles
    if (mode == 1)
    {
        glEnable(GL_POINT_SPRITE);
        glTexEnvi(GL_POINT_SPRITE,GL_COORD_REPLACE,GL_TRUE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glDepthMask(0);
    }
    else if (mode == 2)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    }

    //  Draw arrays
    glDrawArrays(GL_POINTS,0,n*n);
    //  Reset
    if (mode == 1)
    {
        glDisable(GL_POINT_SPRITE);
        glDisable(GL_BLEND);
        glDepthMask(1);
    }
    else if (mode == 2)
    {
        glDisable(GL_BLEND);
    }
    //  Disable arrays
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glf.glDisableVertexAttribArray(VELOCITY_ARRAY);
    glf.glDisableVertexAttribArray(START_ARRAY);
}

//
//  Constructor
//
Hw9opengl::Hw9opengl(QWidget* parent)
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
   N = 100;
   Vert  = new float[3*N*N];
   Color = new float[3*N*N];
   Vel   = new float[4*N*N];
   Start = new float[N*N];
   AmpFreq = new float[4*N*N];
   Lifetime = new float[N*N];
   InitPart();
}

//
//  Reset view
//
void Hw9opengl::reset()
{
   th = ph = 0;
   dim = 3;
   Projection();
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Hw9opengl::setShader(int sel)
{
    mode = sel;
    if (mode)
    {
        bindTexture(particleSprites[mode - 1], GL_TEXTURE_2D);
    }
    InitPart();
}

//
//  Set projection
//
void Hw9opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   Projection();
}

//
//  Initialize
//
void Hw9opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(shader[0],":/ex19a.vert","","");
   Shader(shader[1],":/ex19b.vert","",":/ex19b.frag");
   Shader(shader[2],":/snowflake.vert",":/snowflake.geom",":/snowflake.frag");

   //  Load random texture
   //CreateNoise3D(GL_TEXTURE1);
   //  Load smoke particle
   particleSprites[0] = QPixmap(":/particle.png");
   particleSprites[1] = QPixmap(":/snowflake.png");
   //bindTexture(smoke,GL_TEXTURE_2D);

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
void Hw9opengl::resizeGL(int width, int height)
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
void Hw9opengl::paintGL()
{
   //  Wall time (seconds)
   float t = 0.001*time.elapsed();
   if (move) zh = fmod(90*t,360);

   int err;
   //err = glGetError();

   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glEnable(GL_DEPTH_TEST);

   //  Set view
   glLoadIdentity();
   if (fov) glTranslated(0,0,-2*dim);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Apply shader
   shader[mode].bind();
   shader[mode].setUniformValue("time",t);
   shader[mode].setUniformValue("img",0);
   shader[mode].setUniformValue("Noise3D",1);
   shader[mode].setUniformValue("size",0.3f);

   //  Draw scene
   DrawPart();
   if (glGetError() != GL_NO_ERROR) cerr << "there was an error" << endl;

   //  Release shader
   shader[mode].release();
   glDisable(GL_LIGHTING);
   //glDisable(GL_DEPTH_TEST);

   //  Draw axes
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,1);
   glEnd();
   //  Label axes
   renderText(1,0,0,"X");
   renderText(0,1,0,"Y");
   renderText(0,0,1,"Z");
   
   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
}

//
//  Throw a fatal error and die
//
void Hw9opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw9opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Hw9opengl::Projection()
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
void Hw9opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Hw9opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Hw9opengl::mouseMoveEvent(QMouseEvent* e)
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
void Hw9opengl::wheelEvent(QWheelEvent* e)
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
void Hw9opengl::Shader(QGLShaderProgram& shader, QString vert, QString geom, QString frag)
{
   QGLFunctions glf(QGLContext::currentContext());
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Geometry shader
   if (geom.length() && !shader.addShaderFromSourceFile(QGLShader::Geometry,geom))
      Fatal("Error compiling "+geom+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Bind Attribute Locations
   for (int k=0;Name[k];k++)
      if (Name[k][0])
         glf.glBindAttribLocation(shader.programId(),k,Name[k]);
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
