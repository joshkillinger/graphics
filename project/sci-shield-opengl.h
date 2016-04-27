//
//  OpenGL Hw4opengl Widget
//

#ifndef SCISHIELDOPENGL_H
#define SCISHIELDOPENGL_H

class Object;

#include <QtGui>
#include <QtWidgets/QOpenGLWidget>
#include <QString>
#include <QVector>
#include <QMatrix4x4>

struct light
{
    QVector3D Direction;
    QVector4D Color;
};

class SciShieldOpengl : public QOpenGLWidget, public QOpenGLFunctions
{
Q_OBJECT
private:
    bool    init;      //  Initialized
    int     fov;
    int     th,ph;     //  Display angles
    bool    mouse;     //  Mouse pressed
    QPoint  pos;       //  Mouse position
    double  dim;       //  Display size
    double  asp;       //  Sceen aspect ratio
    unsigned int tex;  //  Textures

    QTimer           timer;    //  Timer for animations
    QPixmap   qtex;

public:
    QVector<Object*> objects;
    QVector<Object*> particlePool;
    struct light Light;

    QMatrix4x4       proj;     //  Projection matrix
    QMatrix4x4       view;

    SciShieldOpengl(QWidget* parent=0);                  //  Constructor
    QSize sizeHint() const {return QSize(400,400);} //  Default size of widget

    float TraceRay(QVector3D origin, QVector3D direction, int *hitIndex);
    void Fire(QVector3D origin, QVector3D direction, QVector3D scale, QVector4D tint);

    void Fatal(QString message, QString caller = "SciShieldOpengl");            //  Error handler
public slots:
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
    void Projection();                     //  Update projection
    void UpdateObjects();
    void RenderObjects(int stage);
    QVector3D ScreenToWorldVector(QPointF screenPoint, float z);
    void FireFromClick(QPointF screenPoint);
};

#endif
