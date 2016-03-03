//
//  OpenGL Hw7opengl Widget
//

#ifndef HW7OPENGL_H
#define HW7OPENGL_H

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include <QImage>
#include <QOpenGLFunctions>

class Hw7opengl : public QGLWidget
{
Q_OBJECT
private:
    bool    init;         //  Initialized
    float   x0,y0;        //  Image center
    float   zoom;         //  Zoom
    int     w,h;          //  Image size
    float   dX,dY;
    int     mode;         //  Display mode
    bool    mouse;        //  Mouse pressed
    int     N;
    int     pic;
    double  asp;
    float   range;
    QGLFramebufferObject* framebuf[2]; //  Framebuffers
    GLuint  textures[4];
    QVector<QImage> images;
    QPoint  pos;          //  Mouse position
    QGLShaderProgram shader[4]; //  Shaders
    //QOpenGLFunctions gl;
public:
    Hw7opengl(QWidget* parent=0);                  //  Constructor
    QSize sizeHint() const {return QSize(400,400);} //  Default size of widget
public slots:
    void setImage(int sel);                //  Slot to set image
    void setShader(int sel);               //  Slot to set shader
    void setPasses(int n);                 //  Slot to set fraction
    void setRange(int r);                 //  Slot to set fraction
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
   void Shader(QGLShaderProgram& shader,QString vert,QString frag);  //  Create shader
   void LoadImage(int index, const QString file);  //  Load image to texture unit
   void Projection();
   void BindImage();
   void Paint(bool firstPass, bool output); //  Draw widget
};

#endif
