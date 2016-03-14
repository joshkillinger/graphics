#ifndef HW9V_H
#define HW9V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw9opengl.h"

class Hw9viewer : public QWidget
{
Q_OBJECT
private:
   Hw9opengl*  ogl;
public:
    Hw9viewer();
};

#endif
