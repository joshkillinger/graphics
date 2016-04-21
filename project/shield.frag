#version 430 core

uniform vec4 Tint;
uniform float Time;

//  Input from previous shader
in vec3 Hit;

//  Fragment color
layout (location=0) out vec4 Fragcolor;


void main()
{
    float distanceFromHit = distance(Hit, gl_FragCoord.xyz);

    float intensity = clamp(Time - distanceFromHit,0.0,1.0);

    Fragcolor = vec4(Tint.rgb,intensity);
}
