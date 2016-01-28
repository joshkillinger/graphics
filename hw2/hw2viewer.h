#ifndef EX03V_H
#define EX03V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw2opengl.h"

class Hw2viewer : public QWidget
{
Q_OBJECT
private:
   QDoubleSpinBox* Xpos;
   QDoubleSpinBox* Ypos;
   QSlider*        Zpos;
   QDoubleSpinBox* Speed;
   QPushButton*    light;
   Hw2opengl*     ogl;
private slots:
   void reset();        //  Reset angles
   void lmove();        //  Pause/animate light
   void izoom(int iz);  //  Zoom level (percent)
public:
    Hw2viewer();
};

#endif
