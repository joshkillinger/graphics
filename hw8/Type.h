#ifndef TYPE_H
#define TYPE_H

#include <math.h>

//
//  Utitily Vec2, Vec3 and Color classes
//

//  Vec3
class Vec3
{
  public:
    float x,y,z;
    Vec3(void)
    {
        x = y = z = 0;
    }
    Vec3(float X,float Y,float Z)
    {
        x = X;
        y = Y;
        z = Z;
    }

    Vec3 operator += (const Vec3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    const Vec3 operator + (const Vec3 &rhs) const
    {
        Vec3 tmp = *this;
        tmp += rhs;
        return tmp;
    }

    Vec3 operator -= (const Vec3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    const Vec3 operator - (const Vec3 &rhs) const
    {
        Vec3 tmp = *this;
        tmp -= rhs;
        return tmp;
    }

    Vec3 operator *= (const float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vec3 operator /= (const float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    const Vec3 operator * (const float rhs) const
    {
        Vec3 tmp = *this;
        tmp *= rhs;
        return tmp;
    }

    const Vec3 operator / (const float rhs) const
    {
        Vec3 tmp = *this;
        tmp /= rhs;
        return tmp;
    }

    const float dot(const Vec3 &rhs)
    {
        float d = x * rhs.x;
        d += y * rhs.y;
        d += z * rhs.z;
        return d;
    }

    float length()
    {
        return sqrt(dot(*this));
    }

    void normalize()
    {
        *this /= length();
    }

    Vec3 cross(const Vec3 &rhs)
    {
        Vec3 tmp = Vec3();
        tmp.x = (y*rhs.z) - (z*rhs.y);
        tmp.y = (z*rhs.x) - (x*rhs.z);
        tmp.z = (x*rhs.y) - (y*rhs.x);
        return tmp;
    }
};

//  Vec2
class Vec2
{
   public:
      float x,y;
      Vec2(void)
      {
         x = y = 0;
      }
      Vec2(float X,float Y)
      {
         x = X;
         y = Y;
      }
};

//  RGB based Color class
class Color
{
   private:
      float v[4];
   public:
      float r,g,b,a;
      Color()
      {
         r = 0;
         g = 0;
         b = 0;
         a = 1;
      }
      Color(float R,float G,float B,float A=1)
      {
         r = R;
         g = G;
         b = B;
         a = A;
      }
      Color(const Vec3& V)
      {
         r = V.x;
         g = V.y;
         b = V.z;
         a = 1;
      }
      float* fv()
      {
         v[0] = r;
         v[1] = g;
         v[2] = b;
         v[3] = a;
         return v;
      }
};

#endif
