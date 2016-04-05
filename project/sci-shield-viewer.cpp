//
//  SciShieldViewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "sci-shield-viewer.h"

//
//  Constructor
//
SciShieldViewer::SciShieldViewer()
{
   //  Set window title
   setWindowTitle(tr("Josh Killinger: Hw4"));

   //  Create new Triangle widget
   ogl = new SciShieldOpengl;

   //  Select projection
   QComboBox* projection = new QComboBox();
   projection->addItem("Orthogonal");
   projection->addItem("Perspective");
   projection->setCurrentIndex(1);

   //  Select object
   QComboBox* object = new QComboBox();
   object->addItem("Cube");

   //  Center position
   Lpos = new QSlider(Qt::Horizontal);
   Zpos = new QSlider(Qt::Horizontal);
   Lpos->setRange(0,360);
   Zpos->setRange(-100,100);

   //  View angle and zoom
   QLabel* angles = new QLabel();

   //  Pause/resume button
   light = new QPushButton("Pause");

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,8,1);
   layout->addWidget(new QLabel("Projection"),1,1);
   layout->addWidget(projection,1,2);
   layout->addWidget(new QLabel("Object"),2,1);
   layout->addWidget(object,2,2);
   layout->addWidget(new QLabel("Light Position"),3,1);
   layout->addWidget(Lpos,3,2);
   layout->addWidget(new QLabel("Light"),4,1);
   layout->addWidget(light,4,2);
   layout->addWidget(new QLabel("Light Elevation"),5,1);
   layout->addWidget(Zpos,5,2);
   layout->addWidget(new QLabel("Angles"),6,1);
   layout->addWidget(angles,6,2);
   layout->addWidget(rst,8,1);
   layout->addWidget(quit,8,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(7,100);
   setLayout(layout);

   //  Connect valueChanged() signals to ogl
   connect(object,SIGNAL(currentIndexChanged(int))     , ogl,SLOT(setObject(int)));
   connect(projection,SIGNAL(currentIndexChanged(int)) , ogl,SLOT(setPerspective(int)));
   connect(Lpos,SIGNAL(valueChanged(int)) , ogl,SLOT(setPos(int)));
   connect(Zpos,SIGNAL(valueChanged(int)) , ogl,SLOT(setElev(int)));
   //  Connect angles() and zoom() signal to labels
   connect(ogl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   connect(ogl,SIGNAL(light(int))      , Lpos,SLOT(setValue(int)));
   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   connect(light,SIGNAL(pressed()),this,SLOT(lmove()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}

//
//  Light pause/move
//
void SciShieldViewer::lmove()
{
   bool pause = (light->text()=="Pause");
   if (pause)
      light->setText("Animate");
   else
      light->setText("Pause");
   ogl->setLightMove(!pause);
}