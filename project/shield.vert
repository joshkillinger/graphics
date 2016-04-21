#version 430 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

//  Vertex attributes (input)
layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 VNormal;
layout(location = 2) in vec3 HitPoint;

//  Output to next shader
out vec3 Hit;

void main()
{
   vec4 vpos = vec4(Vertex,1);

   Hit = HitPoint;
   
   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * vpos;
}
