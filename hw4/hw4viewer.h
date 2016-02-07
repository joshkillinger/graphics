#ifndef EX06V_H
#define EX06V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw4opengl.h"

class Hw4viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*     Lpos;
   QSlider*     Zpos;
   QPushButton* light;
   Hw4opengl*  ogl;
private slots:
   void lmove();        //  Light movement
public:
    Hw4viewer();
};

#endif
