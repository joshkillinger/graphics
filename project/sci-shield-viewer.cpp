//
//  SciShieldViewer Widget
//

#include <QtWidgets\QComboBox>
#include <QtWidgets\QLabel>
#include <QtWidgets\QGridLayout>
#include "sci-shield-viewer.h"
#include <iostream>

using namespace std;

//
//  Constructor
//
SciShieldViewer::SciShieldViewer()
{
   //  Set window title
   setWindowTitle(tr("Josh Killinger: Project"));

   //  Create new Triangle widget
   ogl = new SciShieldOpengl;
   cout << "OpenGL Widget instantiated" << endl;


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
   QPushButton* hit = new QPushButton("Hit");

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   cout << "Laying out grid" << endl;

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,7,1);
   layout->addWidget(new QLabel("Light Position"),0,1);
   layout->addWidget(Lpos,0,2);
   layout->addWidget(new QLabel("Light"),1,1);
   layout->addWidget(light,1,2);
   layout->addWidget(new QLabel("Light Elevation"),2,1);
   layout->addWidget(Zpos,2,2);
   layout->addWidget(new QLabel("Angles"),3,1);
   layout->addWidget(angles,3,2);
   layout->addWidget(hit,4,2);
   layout->addWidget(rst,6,1);
   layout->addWidget(quit,6,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(5,100);
   setLayout(layout);

   cout << "grid laid out" << endl;


   //  Connect valueChanged() signals to ogl
   connect(Lpos,SIGNAL(valueChanged(int)) , ogl,SLOT(setPos(int)));
   connect(Zpos,SIGNAL(valueChanged(int)) , ogl,SLOT(setElev(int)));
   //  Connect angles() and zoom() signal to labels
   connect(ogl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   connect(ogl,SIGNAL(light(int))      , Lpos,SLOT(setValue(int)));
   //  Connect reset() and lmove() signals
   connect(hit  ,SIGNAL(pressed()),ogl,SLOT(hit()));
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   connect(light,SIGNAL(pressed()),this,SLOT(lmove()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));

   cout << "connections made" << endl;

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
