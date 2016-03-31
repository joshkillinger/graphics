#ifndef SCISHIELDV_H
#define SCISHIELDV_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
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
