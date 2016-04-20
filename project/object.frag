#version 430 core

struct light
{
	vec4 Position;
    vec4 Color;
};

uniform light Light;

uniform vec4 Tint;
uniform float Diffuse;
uniform float Specular;
uniform float Shininess;

uniform sampler2D Texture;

//  Input from previous shader
in vec3 View;
in vec3 DirToLight;
in vec3 Normal;
in vec4 Color;

in vec2 vs_tex_coord;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

vec4 phong()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(DirToLight);

   vec4 color = vec4(0);
   
   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*Diffuse;
      //  R is the reflected light vector R = 2(L.N)N - L
      vec3 R = reflect(-L,N);
      //  V is the view vector (eye vector)
      vec3 V = normalize(View);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(R,V);
      if (Is>0.0) color += pow(Is,Shininess)*Specular;
   }

   //  Return sum of color components
   return color;
}

void main()
{

    Fragcolor = Color + (phong() * Light.Color);
    Fragcolor = Fragcolor * texture(Texture, vs_tex_coord) * Tint;
    //Fragcolor = (Color + phong());
    //Fragcolor = vec4(normalize(Normal), 1);
    //Fragcolor = Color;
}
