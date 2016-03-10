//
//  Cube class
//
#include "quad.h"

//
//  Constructor
//
Quad::Quad()
{
   sx = sy = sz = 1;
}

//
//  Display the cube
//
void Quad::display()
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   setTransform(sx,sy,sz);
   //  Texture on
   //EnableTex();
   //  Top
   setColor(Color(1,1,1));
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,0,+1);
   glTexCoord2f(1,0); glVertex3f(+1,0,+1);
   glTexCoord2f(1,1); glVertex3f(+1,0,-1);
   glTexCoord2f(0,1); glVertex3f(-1,0,-1);
   glEnd();
   //  Texture off
   //DisableTex();
   //  Undo transofrmations
   glPopMatrix();
}
