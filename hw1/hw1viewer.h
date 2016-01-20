#ifndef HW1V_H
#define HW1V_H

#include <QWidget>
#include <QSlider>

class Hw1viewer : public QWidget
{
Q_OBJECT
private:
    QSlider* xSlider;
    QSlider* ySlider;
    QSlider* zSlider;

public:
    Hw1viewer();
    void SetXYZ(float x, float y, float z);
};

#endif
