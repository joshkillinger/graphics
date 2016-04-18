//
//  Ex 03:  Procedural Textures
//  Willem A. (Vlakkies) Schreuder
//  CSCI 4239/5239 Spring 2016
//

#include <QApplication>
#include "sci-shield-viewer.h"
#include <iostream>

using namespace std;

//
//  Main function
//
int main(int argc, char *argv[])
{
    cout << "Starting up application" << endl;

    //  Create the application
    QApplication app(argc,argv);
    cout << "QApplication initialized" << endl;

    //  Create and show view widget
    SciShieldViewer view;
    cout << "Viewer initialized" << endl;

    view.show();
    cout << "Viewer displayed" << endl;

    //  Main loop for application
    return app.exec();
    cout << "Done with application" << endl;

}
