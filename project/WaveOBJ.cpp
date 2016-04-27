//
//  Wave OBJ class
//
//  The STL vector template class is used to manage arrays
//
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QPixmap>
#include <QGLWidget>
#include "WaveOBJ.h"
#include <iostream>

using namespace std;

//  Read float
static float Read1(QStringList s)
{
   float v;
   bool OK;
   v = s[1].toFloat(&OK);
   if (!OK) throw "Error reading float from "+s.join(" ");
   return v;
}

//  Read Vec2
static Vec2 Read2(QStringList s)
{
   Vec2 v;
   bool OKx,OKy;
   v.x = s[1].toFloat(&OKx);
   v.y = s[2].toFloat(&OKy);
   if (!OKx || !OKy) throw "Error reading Vec2 from "+s.join(" ");
   return v;
}

//  Read Vec3
static Vec3 Read3(QStringList s)
{
   Vec3 v;
   bool OKx,OKy,OKz;
   v.x = s[1].toFloat(&OKx);
   v.y = s[2].toFloat(&OKy);
   v.z = s[3].toFloat(&OKz);
   if (!OKx || !OKy || !OKz) throw "Error reading Vec3 from "+s.join(" ");
   return v;
}


//
//  Constructor
//
WaveOBJ::WaveOBJ(SciShieldOpengl *context, const char* file, const QString& path) : Object(context)
{
   //  Initialize scale
   sr = 1;

   //  Open file
   QFile f(path+file);
   if (!f.open(QFile::ReadOnly|QFile::Text)) throw QString("Cannot open file ")+file;
   QTextStream in(&f);

   //  Process file
   //mat.clear();
   QVector<Vec3> V,N;
   QVector<Vec2> T;
   QVector<QString> F;

   //  Read file line by line
   QString line;
   while (line = in.readLine(),!line.isNull())
   {
      //  Split line into words
      QStringList words = line.simplified().split(" ");
      //  Vertex coordinates (always 3)
      if (words.size()==4 && words[0]=="v")
         V.push_back(Read3(words));
      //  Normal coordinates (always 3)
      else if (words.size()==4 && words[0]=="vn")
         N.push_back(Read3(words));
      //  Texture coordinates (2 or 3)
      else if (words.size()>=3 && words[0]=="vt")
         T.push_back(Read2(words));
      //  Read and draw facets
      else if (words.size()>3 && words[0]=="f")
      {
          F.push_back(line);
      }
      //  Skip this line
    }
    f.close();

    vertexCount = F.count() * 3;
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.setUsagePattern(QGLBuffer::StaticDraw);
    vertexBuffer.allocate(sizeof(float) * vertexCount * 8);

    //cout << F.count() << " faces, " << vertexCount << " vertices, " << vertexBuffer.size() << " bytes." << endl;

    //  parse faces
    //  Read Vertex/Texture/Normal triplets
    for (int i = 0; i < F.count(); i++)
    {
        QStringList words = F[i].simplified().split(" ");
        for (int k=1;k<words.size();k++)
        {
            int Kv=0,Kt=0,Kn=0;
            QStringList parts = words[k].split('/');
            Kv = parts[0].toInt();
            Kt = parts[1].toInt();
            Kn = parts[2].toInt();
            //  Check index
            if (Kv<-(int)V.count() || Kv>(int)V.count()) throw "Vertex "+QString::number(Kv)+" out of range 1-"+QString::number(V.count());
            if (Kn<-(int)N.count() || Kn>(int)N.count()) throw "Normal "+QString::number(Kn)+" out of range 1-"+QString::number(N.count());
            if (Kt<-(int)T.count() || Kt>(int)T.count()) throw "Texture "+QString::number(Kt)+" out of range 1-"+QString::number(T.count());
            //  Adjust "from end" references
            if (Kv<0) Kv = V.count()+Kv+1;
            if (Kn<0) Kn = N.count()+Kn+1;
            if (Kt<0) Kt = T.count()+Kt+1;

            //  Draw vertex
            float vertex[8];

            vertex[0] = V[Kv-1].x;
            vertex[1] = V[Kv-1].y;
            vertex[2] = V[Kv-1].z;

            vertex[3] = N[Kn-1].x;
            vertex[4] = N[Kn-1].y;
            vertex[5] = N[Kn-1].z;

            vertex[6] = T[Kt-1].x;
            vertex[7] = T[Kt-1].y;

            int index = ((i*3)+(k-1))*8;
            vertexBuffer.write(sizeof(float)*index, vertex, sizeof(vertex));
        }
    }

    //  Unbind this buffer
    vertexBuffer.release();

}
