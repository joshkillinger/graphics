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
#include "material.h"
#include "shield.h"

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
    dim = 10;
    fov = 55;
    th = ph = 0;
    x0 = y0 = 0;
    z0 = 3;
    zh = 0;
}

//
//  Reset view
//
void SciShieldOpengl::reset()
{
    cout << "resetting" << endl;

    th = ph = 0;
    dim = 10;
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

void SciShieldOpengl::hit()
{
    sphere->Hit(QVector3D(2,0,0));
}

//
//  Initialize
//
void SciShieldOpengl::initializeGL()
{
    if (init) return;
    init = true;

    cout << "instantiating triangle" << endl;

    cube = new Cube(this);
    cout << "cube instantiated" << endl;
    Material *mat = new Material(this, 0.2f, 0.5f, 0.3f, 32.0f);
    mat->SetShader(":/object.vert",":/object.frag");
    cout << "shader created" << endl;
    mat->SetTexture(":/crate.png");
    cout << "texture created" << endl;
    cube->SetMaterial(mat);
    cube->transform.SetPosition(QVector3D(0,3,0));


    //  Start 100 fps timer connected to updateGL
    move = true;
    timer.setInterval(10);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start();
    time.start();
    cout << "timers set" << endl;


    // obj model
    WaveOBJ* fighter=0;
    try
    {
        fighter = new WaveOBJ(this,"dark_fighter_6.obj",":/models/fighter/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (fighter)
    {
        Material *fighterMat = new Material(this, 0.3f, 0.6f, 0.3f, 32.0f);
        fighterMat->SetShader(":/object.vert",":/object.frag");
        fighterMat->SetTexture(":/models/fighter/dark_fighter_6_color.png");
        fighter->SetMaterial(fighterMat);
        fighter->transform.SetScale(QVector3D(0.05f,0.05f,0.05f));
        fighter->transform.SetPosition(QVector3D(0,0,0));
        ship1 = fighter;
    }

    // obj model
    WaveOBJ* cruiser=0;
    try
    {
        cruiser = new WaveOBJ(this, "cruiser.obj", ":/models/cruiser/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (fighter)
    {
        Material *cruiserMat = new Material(this, 0.3f, 0.6f, 0.3f, 32.0f);
        cruiserMat->SetShader(":/object.vert",":/object.frag");
        cruiserMat->SetTexture(":/models/cruiser/cruiser.bmp");
        cruiser->SetMaterial(cruiserMat);
        cruiser->transform.SetScale(QVector3D(5,5,5));
        cruiser->transform.SetPosition(QVector3D(-4,-1,0));
        ship2 = cruiser;
    }

    // obj model
    sphere=0;
    try
    {
        sphere = new WaveOBJ(this, "sphere.obj", ":/models/primitives/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (sphere)
    {
        Material *sphereMat = new Shield(this);
        sphereMat->SetShader(":/shield.vert",":/shield.frag");
        sphereMat->SetTexture(":/crate.png");
        sphereMat->SetTint(QVector4D(0,.7,1,1));
        sphere->SetMaterial(sphereMat);
        sphere->transform.SetScale(QVector3D(2,2,2));
        sphere->transform.SetPosition(QVector3D(0,0,0));
    }
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
    float Position[] = {(float)(6*Cos(zh)),z0,(float)(6*Sin(zh)),1.0};

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
    Light.Color = QVector4D(1,1,1,1);

    //move the light by the view matrix (model matrix is the identity matrix in this case)
    Light.Position = view * Light.Position;

    error = glGetError();
    if (error)
    {
        cerr << "error prior to display call: " << error << endl;
    }

    //opaque pipeline

    //cube->display(0);
    ship1->display(0);
    //ship2->display(0);

    //blended pipeline
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sphere->display(1);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);


    ShowAxes();

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

void SciShieldOpengl::ShowAxes()
{
    //  Axes for reference
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(4,0,0);
    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,4,0);
    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,4);
    glEnd();
    glDisable(GL_DEPTH_TEST);
    glColor3f(1,0,0);
    renderText(4.5,0,0,"X");
    glColor3f(0,1,0);
    renderText(0,4.5,0,"Y");
    glColor3f(0,0,1);
    renderText(0,0,4.5,"Z");
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
   }
}

//
//  Mouse wheel
//
void SciShieldOpengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      dim += 0.25;
   //  Zoom in
   else if (dim>1)
      dim -= 0.25;

   Projection();
}

