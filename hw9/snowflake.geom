#version 150 compatibility
#extension GL_EXT_geometry_shader4 : enable

//
//  snowflake geometry shader
//  Billboards point to rotated quad
//

//  Requires OpenGL 3.2 in compatibility mode
layout(points) in;
varying in float rotation[1];
layout(triangle_strip,max_vertices=4) out;
uniform float size;

void billboard(float x,float y,int s,int t)
{
   //  Copy Color
   gl_FrontColor  = gl_FrontColorIn[0];
   //  Set texture coordinates
   gl_TexCoord[0] = vec4(s,t,0,1);
   //  Determine position
   vec2 delta = vec2(x,y);
   vec4 p = gl_PositionIn[0];
   p.x += dot(delta,gl_ModelViewMatrix[0].xy);
   p.y += dot(delta,gl_ModelViewMatrix[1].xy);
   p.z += dot(delta,gl_ModelViewMatrix[2].xy);
   gl_Position = gl_ModelViewProjectionMatrix*p;
   //  Emit new vertex
   EmitVertex();
}

void main()
{
   float theta = rotation[0];
   float x0 = size * cos(theta);
   float y0 = size * sin(theta);
   
   theta += 1.57079632679489661923;
   float x1 = size * cos(theta);
   float y1 = size * sin(theta);

   billboard(x0,y0 , 0,0);
   billboard(x1,y1 , 1,0);
   billboard(-x1,-y1 , 0,1);
   billboard(-x0,-y0 , 1,1);
   EndPrimitive();
}
