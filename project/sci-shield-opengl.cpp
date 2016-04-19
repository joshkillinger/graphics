//
//  OpenGL SciShieldOpengl widget
//
#include "sci-shield-opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>
#include <iostream>
#include "WaveOBJ.h"
#include "cube.h"
#include "triangle.h"

#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

using namespace std;

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
SciShieldOpengl::SciShieldOpengl(QWidget* parent)
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
}

//
//  Reset view
//
void SciShieldOpengl::reset()
{
    cout << "resetting" << endl;

    th = ph = 0;
    dim = 3;
    Projection();

    cout << "reset done" << endl;

}

//
//  Light animation
//
void SciShieldOpengl::setLightMove(bool on)
{
    cout << "light move " << on << endl;

    move = on;
}

//
//  Set light position
//
void SciShieldOpengl::setPos(int Zh)
{
    zh = Zh;
}

//
//  Set light elevation
//
void SciShieldOpengl::setElev(int Z)
{
    z0 = 0.02*Z;
}

//
//  Initialize
//
void SciShieldOpengl::initializeGL()
{
    if (init) return;
    init = true;

    cout << "instantiating triangle" << endl;

    cube = new Triangle(this);
    cout << "triangle instantiated" << endl;
    cube->SetShader(":/object.vert",":/object.frag");
    cout << "shader created" << endl;


    //  Start 100 fps timer connected to updateGL
    move = true;
    timer.setInterval(10);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start();
    time.start();
    cout << "timers set" << endl;


    // Cruiser
//    WaveOBJ* fighter=0;
//    try
//    {
//        fighter = new WaveOBJ("dark_fighter_6.obj",":/models/fighter/");
//    }
//    catch (QString err)
//    {
//        Fatal("Error loading object\n"+err);
//    }
//    if (fighter)
//    {
//        //cruiser->color(1,1,0);
//        ship1 = fighter;
//    }

}

static void PrintQMatrix(const float *f, int size, QString label)
{
    cout << label.toStdString() << " = " << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << f[(i * size) + j] << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

//
//  Set projection when window is resized
//
void SciShieldOpengl::resizeGL(int width, int height)
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
void SciShieldOpengl::paintGL()
{
    //cout << "painting" << endl;

    GLenum error = glGetError();
    if (error)
    {
        cerr << "error at start of paintGL: " << error << endl;
    }

    //  Wall time (seconds)
    float t = 0.001*time.elapsed();
    if (move) zh = fmod(90*t,360);

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //  Translate intensity to color vectors
    float Ambient[]  = {0.3,0.3,0.3,1.0};
    float Diffuse[]  = {0.8,0.8,0.8,1.0};
    float Specular[] = {1.0,1.0,1.0,1.0};
    float Position[] = {(float)(3*Cos(zh)),z0,(float)(3*Sin(zh)),1.0};

    //  Draw light position (no lighting yet)
    glColor3f(1,1,1);
    ball(Position[0],Position[1],Position[2] , 0.1);

    //  Set view
    glLoadIdentity();
    glTranslated(0,0,-2*dim);
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);

    //  Create view matrix
    view.setToIdentity();
    view.translate(0,0,-2*dim);
    view.rotate(ph,1,0,0);
    view.rotate(th,0,1,0);

    Light.Position = QVector4D(Position[0], Position[1], Position[2], Position[3]);
    Light.Ambient = QVector4D(Ambient[0], Ambient[1], Ambient[2], Ambient[3]);
    Light.Diffuse = QVector4D(Diffuse[0], Diffuse[1], Diffuse[2], Diffuse[3]);
    Light.Specular = QVector4D(Specular[0], Specular[1], Specular[2], Specular[3]);

    //move the light by the view matrix (model matrix is the identity matrix in this case)
    Light.Position = view * Light.Position;

    error = glGetError();
    if (error)
    {
        cerr << "error prior to display call: " << error << endl;
    }


    cube->display();

    //  Axes for reference
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(2,0,0);
    glVertex3f(0,0,0);
    glVertex3f(0,2,0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,2);
    glEnd();
    glDisable(GL_DEPTH_TEST);
    renderText(2,0,0,"X");
    renderText(0,2,0,"Y");
    renderText(0,0,2,"Z");

    error = glGetError();
    if (error)
    {
        cerr << "error after all rendering: " << error << endl;
    }


    //  Emit angles to display
    emit angles(QString::number(th)+","+QString::number(ph));
    //  Emit light angle
    emit light((int)zh);
}

//
//  Throw a fatal error and die
//
void SciShieldOpengl::Fatal(QString message, QString caller)
{
   QMessageBox::critical(this,caller,message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void SciShieldOpengl::Projection()
{
    //  Set fixed pipeline transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float zmin = dim/4;
    float zmax = 4*dim;
    float ydim = zmin*tan(fov*M_PI/360);
    float xdim = ydim*asp;
    glFrustum(-xdim,+xdim,-ydim,+ydim,zmin,zmax);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //  Set GL4 transformation
    proj.setToIdentity();
    proj.perspective(fov,asp,dim/4,4*dim);
}

/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void SciShieldOpengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void SciShieldOpengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void SciShieldOpengl::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;  //  Change in mouse location
      th = (th+d.x())%360;      //  Translate x movement to azimuth
      ph = (ph+d.y())%360;      //  Translate y movement to elevation
      pos = e->pos();           //  Remember new location
      //updateGL();               //  Request redisplay
   }
}

//
//  Mouse wheel
//
void SciShieldOpengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      dim += 0.1;
   //  Zoom in
   else if (dim>1)
      dim -= 0.1;
   //  Request redisplay
   Projection();
   //updateGL();
}

