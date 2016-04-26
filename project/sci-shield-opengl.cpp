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
#include "gametime.h"
#include "spherehitbox.h"
#include "squadron.h"
#include "bullet.h"
#include "particleobject.h"
#include "plasmamaterial.h"

#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

using namespace std;

//
//  Constructor
//
SciShieldOpengl::SciShieldOpengl(QWidget* parent)
    : QGLWidget(parent)
{
    init  = false;
    mouse = false;
    asp = 1;
    dim = 20;
    fov = 55;
    th = 0;
    ph = 30;
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
//    cout << "light move " << on << endl;

//    move = on;
}

//
//  Set light position
//
void SciShieldOpengl::setPos(int Zh)
{
    //zh = Zh;
}

//
//  Set light elevation
//
void SciShieldOpengl::setElev(int Z)
{
    //z0 = 0.02*Z;
}

void SciShieldOpengl::hit()
{
    //objects[sphere]->Hit(QVector3D(1,0,0));
}

//
//  Initialize
//
void SciShieldOpengl::initializeGL()
{
    if (init) return;
    init = true;

    GameTime::Start();

    cout << "instantiating triangle" << endl;

    //  Start 100 fps timer connected to updateGL
    timer.setInterval(10);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start();
    time.start();
    cout << "timers set" << endl;

    Object *obj;
    Material *mat;

    //squadron group
    cout << "squadron" << endl;
    obj = new Object(this);
    obj->transform.Rotate(-90, QVector3D(0,1,0));
    obj->SetBehavior(new Squadron(QVector3D(15,0,0)));
    objects.push_back(obj);

    cout << "fighter1" << endl;
    // fighter
    obj=0;
    try
    {
        obj = new WaveOBJ(this,"dark_fighter_6.obj",":/models/fighter/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Material(this, 0.3f, 0.6f, 0.3f, 32.0f);
        mat->SetShader(":/object.vert","",":/object.frag");
        mat->SetTexture(":/models/fighter/dark_fighter_6_color.png");
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(0.05f,0.05f,0.05f));
        obj->transform.Rotate(45, QVector3D(1,0,0));
        obj->transform.SetPosition(QVector3D(1,-1,-3));
        //obj->transform.SetPosition(QVector3D(0,0,-10));
        obj->transform.SetParent(&(objects[0]->transform));
        objects.push_back(obj);
    }

    cout << "fighter shield" << endl;
    // fighter shield
    obj=0;
    try
    {
        obj = new WaveOBJ(this, "sphere.obj", ":/models/primitives/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Shield(this);
        mat->SetShader(":/shield.vert","",":/shield.frag");
        mat->SetTint(QVector4D(0,0.7f,1,1));
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(40,40,40));
        obj->transform.SetPosition(QVector3D(0,0,0));
        obj->transform.SetParent(&(objects[objects.count()-1]->transform));
        obj->SetHitbox(new SphereHitbox(2));
        objects.push_back(obj);
    }

    cout << "fighter2" << endl;
    // fighter
    obj=0;
    try
    {
        obj = new WaveOBJ(this,"dark_fighter_6.obj",":/models/fighter/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Material(this, 0.3f, 0.6f, 0.3f, 32.0f);
        mat->SetShader(":/object.vert","",":/object.frag");
        mat->SetTexture(":/models/fighter/dark_fighter_6_color.png");
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(0.05f,0.05f,0.05f));
        obj->transform.Rotate(45, QVector3D(1,0,0));
        obj->transform.SetPosition(QVector3D(-1,1,0));
        obj->transform.SetParent(&(objects[0]->transform));
        objects.push_back(obj);
    }

    cout << "fighter shield" << endl;
    // fighter shield
    obj=0;
    try
    {
        obj = new WaveOBJ(this, "sphere.obj", ":/models/primitives/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Shield(this);
        mat->SetShader(":/shield.vert","",":/shield.frag");
        mat->SetTint(QVector4D(0,0.7f,1,1));
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(40,40,40));
        obj->transform.SetParent(&(objects[objects.count()-1]->transform));
        obj->SetHitbox(new SphereHitbox(2));
        objects.push_back(obj);
    }

    cout << "fighter3" << endl;
    // fighter
    obj=0;
    try
    {
        obj = new WaveOBJ(this,"dark_fighter_6.obj",":/models/fighter/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Material(this, 0.3f, 0.6f, 0.3f, 32.0f);
        mat->SetShader(":/object.vert","",":/object.frag");
        mat->SetTexture(":/models/fighter/dark_fighter_6_color.png");
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(0.05f,0.05f,0.05f));
        obj->transform.Rotate(45, QVector3D(1,0,0));
        obj->transform.SetPosition(QVector3D(1,-1,3));
        obj->transform.SetParent(&(objects[0]->transform));
        objects.push_back(obj);
    }

    cout << "fighter shield" << endl;
    // fighter shield
    obj=0;
    try
    {
        obj = new WaveOBJ(this, "sphere.obj", ":/models/primitives/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Shield(this);
        mat->SetShader(":/shield.vert","",":/shield.frag");
        mat->SetTint(QVector4D(0,0.7f,1,1));
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(40,40,40));
        obj->transform.SetParent(&(objects[objects.count()-1]->transform));
        obj->SetHitbox(new SphereHitbox(2));
        objects.push_back(obj);
    }

    cout << "cruiser" << endl;
    // cruiser
    obj=0;
    try
    {
        obj = new WaveOBJ(this, "cruiser.obj", ":/models/cruiser/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Material(this, 0.3f, 0.6f, 0.3f, 32.0f);
        mat->SetShader(":/object.vert","",":/object.frag");
        mat->SetTexture(":/models/cruiser/cruiser.bmp");
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(5,5,5));
        obj->transform.SetPosition(QVector3D(-15,0,0));
        objects.push_back(obj);
    }

    cout << "cruiser shield" << endl;
    // cruiser shield
    obj=0;
    try
    {
        obj = new WaveOBJ(this, "sphere.obj", ":/models/primitives/");
    }
    catch (QString err)
    {
        Fatal("Error loading object\n"+err);
    }
    if (obj)
    {
        mat = new Shield(this);
        mat->SetShader(":/shield.vert","",":/shield.frag");
        mat->SetTint(QVector4D(0,1,0.7f,1));
        obj->SetMaterial(mat);
        obj->transform.SetScale(QVector3D(1.7,1.7,1.7));
        obj->transform.SetPosition(QVector3D(0,0,0.125));
        obj->transform.SetParent(&(objects[objects.count()-1]->transform));
        obj->SetHitbox(new SphereHitbox(8.5));
        objects.push_back(obj);
    }

    cout << "plasma" << endl;
    obj=0;
    try
    {
        obj = new ParticleObject(this, 25);
    }
    catch (QString err)
    {
        Fatal("Error loading plasma\n"+err);
    }
    if (obj)
    {
        mat = new PlasmaMaterial(this, 0.3f, 0.6f, 0.3f, 32.0f);
        mat->SetShader(":/plasma.vert",":/plasma.geom",":/plasma.frag");
        mat->SetTexture(":/models/radial_gradient.png");
        mat->SetTint(QVector4D(1,.4,.4,1));
        obj->SetMaterial(mat);
        obj->transform.SetPosition(QVector3D(-8,0,30));
        //obj->transform.SetPosition(QVector3D(-1,1,0));
        obj->transform.SetScale(QVector3D(.5,.5,.5));
        obj->SetBehavior(new Bullet());
        objects.push_back(obj);
    }


    Light.Direction = QVector3D(1.5, -1, -2).normalized();
    Light.Color = QVector4D(1,1,1,1);
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

static inline void PrintQVector3D(const QVector3D vec, QString label)
{
    cout << label.toStdString() << ": " << vec[0] << ", " << vec[1] << ", " << vec[2] << endl;
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

    GameTime::Tick();

    UpdateObjects();

    GLenum error = glGetError();
    if (error)
    {
        cerr << "error at start of paintGL: " << error << endl;
    }

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

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

    error = glGetError();
    if (error)
    {
        cerr << "error prior to display call: " << error << endl;
    }

    //opaque pipeline
    RenderObjects(0);

    //blended pipeline
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);

    RenderObjects(1);

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
}

void SciShieldOpengl::UpdateObjects()
{
    for (int i = 0; i < objects.count(); i++)
    {
        //cout << "udpating " << i << endl;
        objects[i]->Update();
    }
}


void SciShieldOpengl::RenderObjects(int stage)
{
    //cout << "rendering" << endl;

    for (int i = 0; i < objects.count(); i++)
    {
        objects[i]->display(stage);
    }
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
    float zmin = dim/10;
    float zmax = 10*dim;
    float ydim = zmin*tan(fov*M_PI/360);
    float xdim = ydim*asp;
    glFrustum(-xdim,+xdim,-ydim,+ydim,zmin,zmax);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //  Set GL4 transformation
    proj.setToIdentity();
    proj.perspective(fov,asp,dim/10,10*dim);
}

QVector3D SciShieldOpengl::ScreenToWorldVector(QPointF screenPoint, float z)
{
    //cout << "screen position: " << screenPoint.x() << ", " << screenPoint.y() << endl;

    float x = screenPoint.x() * 2.0f / (float)width() - 1;
    float y = -(screenPoint.y() * 2.0f / (float)height() - 1);

    //cout << "relative position: " << x << ", " << y << endl;

    QVector4D vec(x,y,z,1);

    QMatrix4x4 invView = view.inverted();
    QMatrix4x4 invProj = proj.inverted();

    vec = invProj * vec;
    vec = invView * vec;
    vec /= vec.w();

    //cout << "vector: " << vec.x() << ", " << vec.y() << ", " << vec.z() << ", " << vec.w() << endl;

    return vec.toVector3D();
}

float SciShieldOpengl::TraceRay(QVector3D origin, QVector3D direction, int *hitIndex)
{
    *hitIndex = -1;
    float hitDistance = FLT_MAX;

    //find closest hittable object from origin along direction
    for (int i = 0; i < objects.count(); i++)
    {
        float hit = objects[i]->IsHit(origin, direction);
        if (hit > 0)
        {
            if ((hit < hitDistance))
            {
                *hitIndex = i;
                hitDistance = hit;
            }
        }
    }

    return hitDistance;
}

void SciShieldOpengl::Fire(QPointF screenPoint)
{
    QVector3D origin = ScreenToWorldVector(screenPoint, 0);
    QVector3D direction = ScreenToWorldVector(screenPoint, 1) - origin;
    direction.normalize();

    PrintQVector3D(origin, "Origin");
    PrintQVector3D(direction, "Direction");

    int hitIndex = -1;

    float hitDistance = TraceRay(origin, direction, &hitIndex);

    //if we hit something, tell it we hit it
    if (hitIndex != -1)
    {
        QVector3D hitPoint = origin + (hitDistance * direction);
        objects[hitIndex]->Hit(hitPoint);
        PrintQVector3D(hitPoint, "got a hit at");
    }
    else
    {
        cout << "no hit" << endl;
    }
}



/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void SciShieldOpengl::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MouseButton::RightButton)
    {
        mouse = true;
        pos = e->pos();  //  Remember mouse location
    }

    if (e->button() == Qt::MouseButton::LeftButton)
    {
        Fire(e->localPos());
    }
}

//
//  Mouse released
//
void SciShieldOpengl::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MouseButton::RightButton)
    {
        mouse = false;
    }
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

