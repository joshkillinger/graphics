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
   shader->addItem("Impressionist");

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
   layout->addWidget(new QLabel("Image"), 0,1);
   layout->addWidget(picture,0,2);
   layout->addWidget(new QLabel("Shader"),1,1);
   layout->addWidget(shader,1,2);
   layout->addWidget(flab,2,1);
   layout->addWidget(frac,2,2);
   layout->addWidget(rst,4,1);
   layout->addWidget(quit,4,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(3,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(picture,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setImage(int)));
   connect(shader,SIGNAL(currentIndexChanged(int))  , ogl,SLOT(setShader(int)));
   connect(frac,SIGNAL(valueChanged(int))           , ogl,SLOT(setFrac(int)));
   connect(shader,SIGNAL(currentIndexChanged(int))  , this,SLOT(setShader(int)));
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
