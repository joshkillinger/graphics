#ifndef HW8V_H
#define HW8V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw8opengl.h"

class Hw8viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*     Lpos;
   QSlider*     Zpos;
   QPushButton* light;
   Hw8opengl*  ogl;
private slots:
   void lmove();        //  Light movement
public:
    Hw8viewer();
};

#endif
