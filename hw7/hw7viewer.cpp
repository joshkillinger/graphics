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
   setWindowTitle(tr("Hw7:  Image Processing"));

   //  Create new OpenGL widget
   Hw7opengl* ogl = new Hw7opengl;

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("Image 0");
   shader->addItem("Image 1");
   shader->addItem("Transition");
   shader->addItem("Difference");
   shader->addItem("False Color");

   // Fraction
   flab = new QLabel("Fraction");
   frac = new QSlider(Qt::Horizontal);
   frac->setRange(0,100);
   flab->hide();
   frac->hide();

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,4,1);
   layout->addWidget(new QLabel("Shader"),0,1);
   layout->addWidget(shader,0,2);
   layout->addWidget(flab,1,1);
   layout->addWidget(frac,1,2);
   layout->addWidget(rst,3,1);
   layout->addWidget(quit,3,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(2,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(shader,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setShader(int)));
   connect(frac,SIGNAL(valueChanged(int))          , ogl,SLOT(setFrac(int)));
   connect(shader,SIGNAL(currentIndexChanged(int)) , this,SLOT(setShader(int)));
   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}

//
//  Shader adjust
//
void Hw7viewer::setShader(int mode)
{
   if (mode==2)
   {
      flab->show();
      frac->show();
   }
   else
   {
      flab->hide();
      frac->hide();
   }
}
