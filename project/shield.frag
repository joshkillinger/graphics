#version 430 core

const float M_PI = 3.14159265;

uniform vec4 Tint;
uniform float Time;

in vec3 Vert;
in vec3 Hit;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

void main()
{
    float distanceFromHit = distance(Hit, Vert);

    float intensity = clamp((10*Time) - distanceFromHit,0,M_PI);

    intensity = sin(intensity) / (1 + distanceFromHit * 2);


    //intensity = clamp(Time,0,1);
    Fragcolor = vec4(Tint.rgb,intensity);
    //Fragcolor = vec4(Vert,1);
}
