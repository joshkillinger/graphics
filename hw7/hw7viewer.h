#ifndef HW7V_H
#define HW7V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw7opengl.h"

class Hw7viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*     Lpos;
   QSlider*     Zpos;
   QPushButton* light;
   Hw7opengl*  ogl;
private slots:
   void lmove();        //  Light movement
public:
    Hw7viewer();
};

#endif
