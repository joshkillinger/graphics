//
//  Ex01viewer Widget
//

#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "hw1viewer.h"
#include "hw1opengl.h"

//
//  Constructor
//
Hw1viewer::Hw1viewer()
{
    //  Set window title
    setWindowTitle(tr("Assignment 1: Josh Killinger"));

    //  Create new OpenGL widget
    Hw1opengl* hw1opengl = new Hw1opengl;

    //  Select shader
    QComboBox* shader = new QComboBox();
    shader->addItem("None");
    shader->addItem("Basic");
    shader->addItem("NDC to RGB");

    //  Select projection
    QComboBox* projection = new QComboBox();
    projection->addItem("Orthogonal");
    projection->addItem("Perspective");

    //  Select object
    QComboBox* object = new QComboBox();
    object->addItem("Cube");
    object->addItem("Teapot");
    object->addItem("Tyra");

    //  Lighting
    QCheckBox* lighting = new QCheckBox("");

    //  View angle
    QLabel* angles = new QLabel();

    //  Position Sliders
    QSlider* xSlider = new QSlider(Qt::Horizontal);
    QSlider* ySlider = new QSlider(Qt::Horizontal);
    QSlider* zSlider = new QSlider(Qt::Horizontal);
    xSlider->setRange(-2000, 2000);
    ySlider->setRange(-2000, 2000);
    zSlider->setRange(-2000, 2000);
    xSlider->setTracking(true);
    ySlider->setTracking(true);
    zSlider->setTracking(true);
    xSlider->setValue(0);
    ySlider->setValue(0);
    zSlider->setValue(0);

    //  Quit
    QPushButton* quit = new QPushButton("Quit");

    //  Set layout of child widgets
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(hw1opengl,0,0,10,1);
    layout->addWidget(new QLabel("Shader"),0,1);
    layout->addWidget(shader,0,2);
    layout->addWidget(new QLabel("Projection"),1,1);
    layout->addWidget(projection,1,2);
    layout->addWidget(new QLabel("Object"),2,1);
    layout->addWidget(object,2,2);
    layout->addWidget(new QLabel("Lighting"),3,1);
    layout->addWidget(lighting,3,2);
    layout->addWidget(new QLabel("X Pos"),4,1);
    layout->addWidget(xSlider,4,2);
    layout->addWidget(new QLabel("Y Pos"),5,1);
    layout->addWidget(ySlider,5,2);
    layout->addWidget(new QLabel("Z Pos"),6,1);
    layout->addWidget(zSlider,6,2);
    layout->addWidget(new QLabel("Angles"),7,1);
    layout->addWidget(angles,7,2);
    layout->addWidget(quit,9,2);
    //  Manage resizing
    layout->setColumnStretch(0,100);
    layout->setColumnMinimumWidth(0,100);
    layout->setRowStretch(8,100);
    setLayout(layout);

    //  Connect valueChanged() signals to ex01opengl
    connect(shader,SIGNAL(currentIndexChanged(int))      , hw1opengl, SLOT(setShader(int)));
    connect(object,SIGNAL(currentIndexChanged(int))      , hw1opengl, SLOT(setObject(int)));
    connect(projection,SIGNAL(currentIndexChanged(int))  , hw1opengl, SLOT(setPerspective(int)));
    connect(lighting,SIGNAL(stateChanged(int))           , hw1opengl, SLOT(setLighting(int)));
    connect(xSlider,SIGNAL(valueChanged(int))            , hw1opengl, SLOT(setXPos(int)));
    connect(ySlider,SIGNAL(valueChanged(int))            , hw1opengl, SLOT(setYPos(int)));
    connect(zSlider,SIGNAL(valueChanged(int))            , hw1opengl, SLOT(setZPos(int)));

    //  Connect angles() signal to label
    connect(hw1opengl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
    //  Connect quit() signal to qApp::quit()
    connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}
