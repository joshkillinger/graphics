//
//  Hw10viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "hw10viewer.h"

//
//  Constructor
//
Hw10viewer::Hw10viewer()
{
   //  Set window title
   setWindowTitle(tr("Josh Killinger: Hw10"));

   //  Create new OpenGL widget
   ogl = new Hw10opengl;

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,5,1);
   layout->addWidget(rst,4,1);
   layout->addWidget(quit,4,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(3,100);
   setLayout(layout);

   //  Connect reset() and lmove() signals
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   //  Connect quit() signal to qApp::quit()
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}
