#version 430 core

const float M_PI = 3.14159265;

uniform vec4 Tint;
uniform float Time[10];

//uniform vec3 HitPoint;

in vec3 Vert;
in vec3 Hit[10];

//  Fragment color
layout (location=0) out vec4 Fragcolor;

void main()
{
    float totalIntensity = 0;

    for (int i = 0; i < 10; i++)
    {
        float distanceFromHit = distance(Hit[i], Vert);

        float intensity = clamp((10*Time[i]) - distanceFromHit, 0, M_PI);

        totalIntensity += sin(intensity) / (1 + distanceFromHit * 2);
    }

    Fragcolor = vec4(Tint.rgb, totalIntensity);
}
