#version 430 core

uniform vec4 LightDiffuse;
uniform vec4 LightSpecular;
uniform sampler2D Texture;
uniform float shininess;

//  Input from previous shader
in vec3 View;
in vec3 Light;
in vec3 Normal;
in vec4 Ambient;

in vec2 vs_tex_coord;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

vec4 phong()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(Light);

   //  Emission and ambient color
   vec4 color = vec4(0);//Ambient;

   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   //if (Id>0.0)
   {
      //  Add diffuse
      color += Id*LightDiffuse;
      //  R is the reflected light vector R = 2(L.N)N - L
      //vec3 R = reflect(-L,N);
      //  V is the view vector (eye vector)
      //vec3 V = normalize(View);
      //  Specular is cosine of reflected and view vectors
      //float Is = dot(R,V);
      //if (Is>0.0) color += pow(Is,shininess)*LightSpecular;
   }

   //  Return sum of color components
   return color;
}

void main()
{
   //Fragcolor = vec4(FrontColor,1.0) * texture(tex, vs_tex_coord);
   //Fragcolor = texture(tex, vs_tex_coord);
   Fragcolor = phong();
   //Fragcolor = phong() * texture(Texture, vs_tex_coord);
}
