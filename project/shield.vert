#version 430 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

uniform vec3 HitPoint;

//  Vertex attributes (input)
layout(location = 0) in vec3 Vertex;

out vec3 Vert;
out vec3 Hit;

void main()
{
   vec4 vpos = vec4(Vertex,1);

   Vert = (ModelViewMatrix * vpos).xyz;
   Hit = (ModelViewMatrix * vec4(HitPoint,1)).xyz;

   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * vpos;
}
