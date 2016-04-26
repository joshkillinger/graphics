#version 410 core

const float M_PI = 3.14159265;

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

uniform float Time;      //  Time
uniform float Lifetime;
uniform vec4 Tint;

//  Vertex attributes (input)
layout(location = 0) in vec3 Vertex;
layout(location = 1) in float Start;

layout (location = 0) out vec4 VertPosition;
layout (location = 1) out vec4 VertColor;
layout (location = 2) out float Size;

void main(void)
{
   float t = mod(Time - Start, Lifetime);

   //  Initial position
   vec4 vert = vec4(Vertex, 1);

   //move back at 10 units per second (half speed of bullet)
   vert.z += t * 5;

   //  Contract with distance
   //vert.x *= 1.0-0.1*vert.z;
   //vert.y *= 1.0-0.1*vert.z;
   vert.x = 0;
   vert.y = 0;

   float theta = 8 * M_PI * t;
   Size = (theta > (2 * M_PI)) ? (2 * M_PI) / (theta) : (1-cos(theta)) * .5;

   VertColor = Tint;
   VertPosition = vert;
}
