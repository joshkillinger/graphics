#version 430 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

uniform vec4 LightPosition;
uniform vec4 LightAmbient;

//  Vertex attributes (input)
layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec3 VNormal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 TexCoord;

//  Output to next shader
out vec3 View;
out vec3 Light;
out vec3 Normal;
out vec4 Ambient;

out vec2 vs_tex_coord;

void main()
{	
   vec3 P = vec3(ModelViewMatrix * Vertex);
   //  Light position
   Light  = vec3(LightPosition) - P;
   //  Normal
   Normal = NormalMatrix * VNormal;
   //  Eye position
   View  = -P;
   
   
   //  Ambient color
   Ambient = LightAmbient*vec4(Color,1.0);

   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
   
   vs_tex_coord = TexCoord;
}
