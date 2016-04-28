#ifndef SCISHIELDV_H
#define SCISHIELDV_H

#include <QtGui>
#include <QtWidgets\QSlider>
#include <QtWidgets\QPushButton>

#include "sci-shield-opengl.h"

class SciShieldViewer : public QWidget
{
Q_OBJECT
private:
    SciShieldOpengl* ogl;
public:
    SciShieldViewer();
};

#endif
