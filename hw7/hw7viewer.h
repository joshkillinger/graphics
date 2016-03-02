#ifndef HW7V_H
#define HW7V_H

#include <QWidget>
#include <QSlider>
#include <QDoubleSpinBox>
#include "hw7opengl.h"

class Hw7viewer : public QWidget
{
Q_OBJECT
public:
    Hw7viewer();
    QLabel*  flab;
    QSlider* frac;
private slots:
    void setShader(int mode);
};

#endif
