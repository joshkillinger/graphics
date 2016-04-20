//
//  Wave OBJ class
//  The constructor places the object at the origin
//  All parameters are assigned default values
//
#ifndef WAVEOBJ_H
#define WAVEOBJ_H

#include "Object.h"
#include "Type.h"
#include <QString>
#include <QHash>

//  Material structure
typedef struct
{
   Color Ke,Ka,Kd,Ks; //  Colors
   float Ns;          //  Shininess
   float d;           //  Transparency
   unsigned int map;  //  Texture
} Mat;

class WaveOBJ: public Object
{
private:
   float sr;              // Scale
   //int   list;            // Display list
   QHash<QString,Mat> mat;
   void SetMaterial(const QString& name);
   void LoadMaterial(const QString& name,const QString& path="");
public:
   WaveOBJ(SciShieldOpengl *context, const char* file,const QString& path);     //  Constructor
};

#endif
