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

   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(hw1opengl,0,0,7,1);
   layout->addWidget(new QLabel("Shader"),0,1);
   layout->addWidget(shader,0,2);
   layout->addWidget(new QLabel("Projection"),1,1);
   layout->addWidget(projection,1,2);
   layout->addWidget(new QLabel("Object"),2,1);
   layout->addWidget(object,2,2);
   layout->addWidget(new QLabel("Lighting"),3,1);
   layout->addWidget(lighting,3,2);
   layout->addWidget(new QLabel("Angles"),4,1);
   layout->addWidget(angles,4,2);
   layout->addWidget(quit,6,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(5,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ex01opengl
   connect(shader,SIGNAL(currentIndexChanged(int))     , hw1opengl,SLOT(setShader(int)));
   connect(object,SIGNAL(currentIndexChanged(int))     , hw1opengl,SLOT(setObject(int)));
   connect(projection,SIGNAL(currentIndexChanged(int)) , hw1opengl,SLOT(setPerspective(int)));
   connect(lighting,SIGNAL(stateChanged(int))          , hw1opengl,SLOT(setLighting(int)));
   //  Connect angles() signal to label
   connect(hw1opengl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}
