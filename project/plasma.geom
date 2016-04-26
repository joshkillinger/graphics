#version 410 core

//  Billboards point to rotated quad

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

layout (location = 0) in vec4 VertPosition[];
layout (location = 1) in vec4 VertColor[];
layout (location = 2) in float Size[];

layout(triangle_strip,max_vertices=4) out;

//out vec4 Position;
out vec4 Color;
out vec2 TexCoord;

void billboard(float x,float y,int s,int t)
{
    //  Set color and texture coordinates
    Color = VertColor[0];
    TexCoord = vec2(s,t);

    //  Determine position
    vec2 delta = vec2(x,y);
    vec4 p = VertPosition[0];
    p.x += dot(delta,ModelViewMatrix[0].xy);
    p.y += dot(delta,ModelViewMatrix[1].xy);
    p.z += dot(delta,ModelViewMatrix[2].xy);
    gl_Position = ProjectionMatrix * ModelViewMatrix * p;

    //  Emit new vertex
    EmitVertex();
}

void main()
{
    float size = Size[0];
    billboard(-size,-size , 0,0);
    billboard(+size,-size , 1,0);
    billboard(-size,+size , 0,1);
    billboard(+size,+size , 1,1);
    EndPrimitive();
}
