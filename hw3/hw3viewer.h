#ifndef EX04V_H
#define EX04V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw3opengl.h"

class Hw3viewer : public QWidget
{
Q_OBJECT
private:
   QSlider*     Lpos;
   QSlider*     Zpos;
   QPushButton* light;
   Hw3opengl*  ogl;
   QLabel*      fps;
private slots:
   void lmove();        //  Light movement
public:
    Hw3viewer();
};

#endif
