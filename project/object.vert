#version 430 core

struct light
{
	vec4 Position;
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
};

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform light Light;

//  Vertex attributes (input)
layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 VNormal;
layout(location = 2) in vec2 TexCoord;

//  Output to next shader
out vec3 View;
out vec3 DirToLight;
out vec3 Normal;
out vec4 Color;

out vec2 vs_tex_coord;

void main()
{
   vec4 vpos = vec4(Vertex,0);
   vec3 P = vec3(ModelViewMatrix * vpos);
   //  Light position
   DirToLight  = vec3(Light.Position) - P;
   //  Normal
   Normal = NormalMatrix * VNormal;
   //  Eye position
   View  = -P;
   
   
   //  Ambient color
   Color = Light.Ambient;

   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * vpos;
   
   vs_tex_coord = TexCoord;
}
