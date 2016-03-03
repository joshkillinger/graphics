//
//  OpenGL widget
//
#include "hw7opengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include <math.h>
#include <iostream>

using namespace std;

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
   pic = 0;
   N = 1;
   asp = 1;
   range = 5.0f;

   //gl = QOpenGLFunctions();

   framebuf[0] = 0;
   framebuf[1] = 0;
}

void Hw7opengl::BindImage()
{
   glBindTexture(GL_TEXTURE_2D,textures[pic]);
   w = images[pic].width();
   h = images[pic].height();
   glTexImage2D(GL_TEXTURE_2D,0,4,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,images[pic].bits());
   //  Set pixel interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

//
//  Set image
//
void Hw7opengl::setImage(int sel)
{
    pic = sel;

    BindImage();

    updateGL();
}

//
//  Reset view
//
void Hw7opengl::reset()
{
   x0 = y0 = 0;
   zoom = 1;
   updateGL();
}

//
//  Set number of colors to flatten to
//
void Hw7opengl::setRange(int r)
{
   range = r;
   updateGL();
}

//
//  Set number of shader passes
//
void Hw7opengl::setPasses(int n)
{
   N = n;
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
void Hw7opengl::LoadImage(int index, const QString file)
{
   //  Load image
   QImage img(file);
   //  Select texture unit
   //gl.glActiveTexture(unit);
   //  Bind texture
   glGenTextures(1,&(textures[index]));
   glBindTexture(GL_TEXTURE_2D,textures[index]);
   //  Copy image to texture
   QImage rgba = QGLWidget::convertToGLFormat(img);
   w = rgba.width();
   h = rgba.height();
   glTexImage2D(GL_TEXTURE_2D,0,4,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
   //  Set pixel interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   images.append(rgba);
}

//
//  Initialize
//
void Hw7opengl::initializeGL()
{
    if (init) return;
    init = true;

    //  Load shader
    Shader(shader[0],"",":/ex11a.frag");
    Shader(shader[1],"",":/ex11c.frag");
    Shader(shader[2],"",":/ex11b.frag");
    Shader(shader[3],"",":/flatten.frag");

    //  Load images
    LoadImage(1,":/landscape2.jpg");
    LoadImage(2,":/landscape3.jpg");
    LoadImage(3,":/landscape4.jpg");
    LoadImage(0,":/landscape1.jpg");
}

//
//  Set projection
//
void Hw7opengl::resizeGL(int width, int height)
{
   //  Window aspect ratio
   asp = width/(float)height;
   //  Viewport is entire window
   glViewport(0,0,width,height);

   for (int k=0;k<2;k++)
   {
      //  Allocate frame buffer objects
      if (framebuf[k]) delete framebuf[k];
      framebuf[k] = new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth);
      framebuf[k]->release();
   }

   dX = 1.0/width;
   dY = 1.0/height;

   updateGL();
}

void Hw7opengl::Projection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-asp, +asp, -1, +1, -1, +1);
    //  Set Projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//
//  Draw the window
//
void Hw7opengl::paintGL()
{
    if (mode == 4)
    {
        mode = 1;
        Paint(true, false, N);
        mode = 3;
        Paint(false, true, N);
        mode = 4;
    }
    else
    {
        Paint(true, true, N);
    }
}

void Hw7opengl::Paint(bool firstPass, bool output, int n)
{
    if (mode == 3) n = 1;

    //  Image aspect ratio
    float asp = w / (float)h;

    //  Send output to framebuf[0]
    if (mode) framebuf[0]->bind();

    if (firstPass)
    {
        //  Enable shader
        shader[0].bind();
        //  Set pixel increments
        shader[0].setUniformValue("img",0);

        Projection();

        glLoadIdentity();

        glScaled(zoom,zoom,1);
        glTranslated(x0,y0,0);

        BindImage();

        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(-asp,-1);
        glTexCoord2f(1,0); glVertex2f(+asp,-1);
        glTexCoord2f(1,1); glVertex2f(+asp,+1);
        glTexCoord2f(0,1); glVertex2f(-asp,+1);
        glEnd();

        shader[0].release();
    }

    if (mode)
    {
        shader[mode].bind();
        //  Set shader increments
        shader[mode].setUniformValue("dX",dX);
        shader[mode].setUniformValue("dY",dY);
        shader[mode].setUniformValue("range",range);

        glDisable(GL_DEPTH_TEST);

        //  Reset projections
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //  Ping-Pong
        for (int k=0;k<n;k++)
        {
            //in mode 3, if we had a previous shader running, and there were an odd number of passes, swap buffers
            if ((mode == 3) && (N % 2) &! firstPass)
            {
                k ++;
            }

            int last = k%2;
            int next = 1-last;
            //  Set output to next framebuffer except for the last pass
            if (k+1<n)
            {
               framebuf[next]->bind();
            }
            else
            {
                if (output)
                {
                    framebuf[last]->release();
                }
                else
                {
                    framebuf[next]->bind();
                }
            }
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
        //  Done with shader
        shader[mode].release();
    }
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
