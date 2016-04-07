#ifndef HW10V_H
#define HW10V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw10opengl.h"

class Hw10viewer : public QWidget
{
Q_OBJECT
private:
    Hw10opengl*  ogl;
public:
    Hw10viewer();
};

#endif
