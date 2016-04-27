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


   //  View angle and zoom
   QLabel* angles = new QLabel();

   //  Reset
   QPushButton* hit = new QPushButton("Hit");

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   cout << "Laying out grid" << endl;

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,4,1);
   layout->addWidget(new QLabel("Angles"),0,1);
   layout->addWidget(angles,0,2);
   layout->addWidget(hit,1,2);
   layout->addWidget(rst,3,1);
   layout->addWidget(quit,3,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(2,100);
   setLayout(layout);

   cout << "grid laid out" << endl;

   //  Connect angles() and zoom() signal to labels
   connect(ogl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   //  Connect reset() and lmove() signals
   connect(hit  ,SIGNAL(pressed()),ogl,SLOT(hit()));
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));

   cout << "connections made" << endl;

}
