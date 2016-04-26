#version 410 core

//  Transformation matrices
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

uniform float Time;      //  Time
uniform vec4 Tint;

//  Vertex attributes (input)
layout(location = 0) in vec3 Vertex;
layout(location = 1) in float Start;

layout (location = 0) out vec4 VertPosition;
layout (location = 1) out vec4 VertColor;

void main(void)
{
   //  Particle life is mod Start time
   float t = mod(Time,Start);
   float alpha = min(clamp((Start - t) * 2.0, 0, 1), t * 4);

   //  Initial position
   vec4 vert = vec4(Vertex, 1);

   //move back at 10 units per second (same speed as bullet)
   vert.z += t * 10;

   //  Contract with distance
   vert.x *= 1.0-0.1*vert.z;
   vert.y *= 1.0-0.1*vert.z;

   VertColor = Tint;
   VertColor.a *= alpha;
   VertPosition = vert;
   //VertPosition = vec4(Vertex,1);
}
