#version 430 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ViewMatrix;

uniform vec3 HitPoint[10];

//  Vertex attributes (input)
layout(location = 0) in vec3 Vertex;

out vec3 Vert;
out vec3 Hit[10];

void main()
{
   vec4 vpos = vec4(Vertex,1);

   Vert = (ModelViewMatrix * vpos).xyz;
   for (int i = 0; i < 10; i++)
   {
      Hit[i] = (ModelViewMatrix * vec4(HitPoint[i],1)).xyz;
   }
   //Hit = HitPoint;

   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * vpos;
}
