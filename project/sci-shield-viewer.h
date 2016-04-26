#ifndef SCISHIELDV_H
#define SCISHIELDV_H

#include <QtGui>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>

#include "sci-shield-opengl.h"

class SciShieldViewer : public QWidget
{
Q_OBJECT
private:
    QSlider*         Lpos;
    QSlider*         Zpos;
    QPushButton*     light;
    SciShieldOpengl* ogl;
private slots:
    void lmove();        //  Light movement
public:
    SciShieldViewer();
};

#endif
