//
//  Ex 01:  Basic Shader
//  Willem A. (Vlakkies) Schreuder
//  CSCI 4239/5239 Spring 2016
//  Program modified by Josh Killinger for Homework 1
//

#include <QApplication>
#include "hw1viewer.h"

//
//  Main function
//
int main(int argc, char *argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show view widget
   Hw1viewer view;
   view.show();
   //  Main loop for application
   return app.exec();
}
