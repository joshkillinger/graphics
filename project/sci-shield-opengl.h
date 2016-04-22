//
//  OpenGL Hw4opengl Widget
//

#ifndef SCISHIELDOPENGL_H
#define SCISHIELDOPENGL_H

class Object;

#include <QtOpenGL>
#include <QString>
#include <QVector>
#include <QMatrix4x4>

struct light
{
    QVector4D Position;
    QVector4D Color;
};

class SciShieldOpengl : public QGLWidget
{
Q_OBJECT
private:
    bool    init;      //  Initialized
    bool    move;      //  Light animation
    float   zh;        //  Light position
    int     fov;
    float   x0,y0;     //  Object center
    float   z0;        //  Zoom
    int     mode;      //  Display mode
    int     th,ph;     //  Display angles
    bool    mouse;     //  Mouse pressed
    QPoint  pos;       //  Mouse position
    double  dim;       //  Display size
    double  asp;       //  Sceen aspect ratio
    unsigned int tex;  //  Textures

    QVector<Object*> objects;
    int sphere;

    QTimer           timer;    //  Timer for animations
    QElapsedTimer    time;     //  Track elapsed time
    QPixmap   qtex;

public:
    struct light Light;

    QMatrix4x4       proj;     //  Projection matrix
    QMatrix4x4       view;

    SciShieldOpengl(QWidget* parent=0);                  //  Constructor
    QSize sizeHint() const {return QSize(400,400);} //  Default size of widget

    void Fatal(QString message, QString caller = "SciShieldOpengl");            //  Error handler
public slots:
    void hit();                            //  Slot to hit shield (testing only)
    void setPos(int ang);                  //  Slot to light position
    void setElev(int z);                   //  Slot to light elevation (%)
    void setLightMove(bool on);            //  Slot to set light movement
    void reset();                          //  Reset view
signals:
    void angles(QString text);             //  Signal for view angles
    void light(int);                       //  Light position
protected:
    void initializeGL();                   //  Initialize widget
    void resizeGL(int width, int height);  //  Resize widget
    void paintGL();                        //  Draw widget
    void mousePressEvent(QMouseEvent*);    //  Mouse pressed
    void mouseReleaseEvent(QMouseEvent*);  //  Mouse released
    void mouseMoveEvent(QMouseEvent*);     //  Mouse moved
    void wheelEvent(QWheelEvent*);         //  Mouse wheel
private:
    void Projection();                      //  Update projection
    void UpdateObjects();
    void RenderObjects(int stage);
    void ShowAxes();
};

#endif
