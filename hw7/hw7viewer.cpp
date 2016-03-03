//
//  Hw7viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "hw7viewer.h"

//
//  Constructor
//
Hw7viewer::Hw7viewer()
{
   //  Set window title
   setWindowTitle(tr("Josh Killinger: Hw7"));

   //  Create new OpenGL widget
   Hw7opengl* ogl = new Hw7opengl;

   //  Select image
   QComboBox* picture = new QComboBox();
   picture->addItem("Image 1");
   picture->addItem("Image 2");
   picture->addItem("Image 3");
   picture->addItem("Image 4");

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("None");
   shader->addItem("Blur");
   shader->addItem("Sharpen");
   shader->addItem("Flatten");
   shader->addItem("Posterize");

   // Passes
   QSpinBox* passes = new QSpinBox();
   passes->setRange(0,100);

   // Colors
   QSpinBox* colors = new QSpinBox();
   colors->setRange(1,100);
   colors->setValue(10);

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,5,1);
   layout->addWidget(new QLabel("Image"), 0,1);
   layout->addWidget(picture,0,2);
   layout->addWidget(new QLabel("Shader"),1,1);
   layout->addWidget(shader,1,2);
   layout->addWidget(new QLabel("Passes"),2,1);
   layout->addWidget(passes,2,2);
   layout->addWidget(new QLabel("Colors"),3,1);
   layout->addWidget(colors,3,2);
   layout->addWidget(rst,5,1);
   layout->addWidget(quit,5,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(4,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(picture,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setImage(int)));
   connect(shader,SIGNAL(currentIndexChanged(int))  , ogl,SLOT(setShader(int)));
   connect(passes,SIGNAL(valueChanged(int))         , ogl,SLOT(setPasses(int)));
   connect(colors,SIGNAL(valueChanged(int))         , ogl,SLOT(setRange(int)));
   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}
