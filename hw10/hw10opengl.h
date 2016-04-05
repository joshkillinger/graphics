//
//  OpenGL Ex19opengl Widget
//

#ifndef HW10OPENGL_H
#define HW10OPENGL_H

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include <QGLFunctions>
#include <QOpenGLBuffer>

class Hw10opengl : public QGLWidget
{
Q_OBJECT
private:
   int     N;         //  Particle count
   bool    init;      //  Initialized
   bool    move;      //  Light animation
   float   zh;        //  Light position
   float   x0,y0;     //  Object center
   float   z0;        //  Zoom
   int     n;         //  Particle count
   int     mode;      //  Display mode
   int     th,ph;     //  Display angles
   bool    mouse;     //  Mouse pressed
   QPoint  pos;       //  Mouse position
   int     fov;       //  Field of view
   double  dim;       //  Display size
   double  asp;       //  Sceen aspect ratio
   static const int size = 1024;
   char    board[2][size*size];
   int     last;

   QOpenGLShaderProgram shader[2]; //  Shaders
   QTimer           timer;     //  Timer for animations
   QElapsedTimer    time;      //  Track elapsed time

   QOpenGLBuffer buffers[2];

public:
   Hw10opengl(QWidget* parent=0);                  //  Constructor
   QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
public slots:
    void reset();                          //  Reset view
protected:
    void initializeGL();                   //  Initialize widget
    void resizeGL(int width, int height);  //  Resize widget
    void paintGL();                        //  Draw widget
    void mousePressEvent(QMouseEvent*);    //  Mouse pressed
    void mouseReleaseEvent(QMouseEvent*);  //  Mouse released
    void mouseMoveEvent(QMouseEvent*);     //  Mouse moved
    void wheelEvent(QWheelEvent*);         //  Mouse wheel
private:
   void Fatal(QString message);            //  Error handler
   void Projection();                      //  Update projection
   void Shader(QOpenGLShaderProgram& shader, QString compute, QString vert, QString geom, QString frag);  //  Create shader
};

#endif
