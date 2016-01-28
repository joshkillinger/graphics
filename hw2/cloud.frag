#version 130

uniform sampler3D Noise;
uniform vec3 SkyColor;
uniform vec3 CloudColor;

in float LightIntensity;
in vec3  MCposition;

out vec4 FragColor;

void main()
{
    vec4 noisevec = texture(Noise, MCposition);
    float intensity = (noisevec[0] + noisevec[1] + noisevec[2] + noisevec[3] + .03125) * 1.5;
    vec3 color  = mix(SkyColor, CloudColor, intensity) * LightIntensity;

    FragColor = vec4(color, 1.0);
}
