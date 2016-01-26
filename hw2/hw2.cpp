//
//  Ex 03:  Procedural Textures
//  Willem A. (Vlakkies) Schreuder
//  CSCI 4239/5239 Spring 2016
//
//  Modified by Josh Killinger for HW2

#include <QApplication>
#include "hw2viewer.h"

//
//  Main function
//
int main(int argc, char *argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show view widget
   Hw2viewer view;
   view.show();
   //  Main loop for application
   return app.exec();
}