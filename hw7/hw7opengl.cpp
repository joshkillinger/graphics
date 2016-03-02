//
//  OpenGL widget
//
#include "hw7opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>

//
//  Constructor
//
Hw7opengl::Hw7opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   init  = false;
   mouse = false;
   x0 = y0 = 0;
   zoom = 1;

   gl = QOpenGLFunctions();
}

//
//  Reset view
//
void Hw7opengl::reset()
{
   x0 = y0 = 0;
   zoom = 1;
   frac = 0;
   updateGL();
}

//
//  Set shader
//
void Hw7opengl::setFrac(int pct)
{
   frac = pct;
   updateGL();
}

//
//  Set shader
//
void Hw7opengl::setShader(int sel)
{
   mode = sel;
   updateGL();
}

//
//  Load image to texture unit
//
void Hw7opengl::LoadImage(GLenum unit,const QString file)
{
   //  Load image
   QImage img(file);
   //  Select texture unit
   gl.glActiveTexture(unit);
   //  Bind texture
   unsigned int tex;
   glGenTextures(1,&tex);
   glBindTexture(GL_TEXTURE_2D,tex);
   //  Copy image to texture
   QImage rgba = QGLWidget::convertToGLFormat(img);
   w = rgba.width();
   h = rgba.height();
   glTexImage2D(GL_TEXTURE_2D,0,4,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
   //  Set pixel interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

//
//  Initialize
//
void Hw7opengl::initializeGL()
{
    if (init) return;
    init = true;

    gl.initializeOpenGLFunctions();

    //  Load shader
    Shader(shader,"",":/ex13.frag");

    //  Load images
    LoadImage(GL_TEXTURE0,":/20090602.png");
    LoadImage(GL_TEXTURE1,":/20090706.png");
}

//
//  Set projection
//
void Hw7opengl::resizeGL(int width, int height)
{
   //  Window aspect ratio
   float asp = width/(float)height;
   //  Viewport is entire window
   glViewport(0,0,width,height);
   //  Set Projection
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-asp, +asp, -1, +1, -1, +1);
   //  Set Projection
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   updateGL();
}

//
//  Draw the window
//
void Hw7opengl::paintGL()
{
   //  Image aspect ratio
   float asp = w / (float)h;

   //  Enable shader
   shader.bind();

   //  Set pixel increments
   shader.setUniformValue("img0",0);
   shader.setUniformValue("img1",1);
   shader.setUniformValue("frac",(float)(frac/100.0));
   shader.setUniformValue("mode",mode);

   //  Set ModelView
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glScaled(zoom,zoom,1);
   glTranslated(x0,y0,0);
   //  Draw to screen
   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex2f(-asp,-1);
   glTexCoord2f(1,0); glVertex2f(+asp,-1);
   glTexCoord2f(1,1); glVertex2f(+asp,+1);
   glTexCoord2f(0,1); glVertex2f(-asp,+1);
   glEnd();

   //  Done with shader
   shader.release();
}

//
//  Throw a fatal error and die
//
void Hw7opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Hw7opengl",message);
   QApplication::quit();
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
      QPoint d = e->pos()-pos;      //  Change in mouse location
      x0 += d.x()/(zoom*width());   //  Translate x movement to azimuth
      y0 -= d.y()/(zoom*height());  //  Translate y movement to elevation
      pos = e->pos();               //  Remember new location
      updateGL();
   }
}

//
//  Mouse wheel
//
void Hw7opengl::wheelEvent(QWheelEvent* e)
{
    //  Zoom out
    if (e->delta()<0)
        zoom *= 1.05;
    //  Zoom in
    else if (zoom>1)
        zoom /= 1.05;
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
