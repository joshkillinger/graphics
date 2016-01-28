//  Ring fragment shader
//  Derived from Brick Fragment Shader

//  Hardwire ring color
const vec3 ColorA = vec3(1.0,0.0,0.0);
const vec3 ColorB = vec3(0.0,0.0,1.0);

//  Model coordinates and light from vertex shader
varying float LightIntensity;
varying vec2  ModelPos;
uniform float time;

void main()
{
    //square distance means the rings get smaller as we get farther from center, multiply by 8 to make it a bit more interesting to look at
    float sqrdist = ((ModelPos.x*ModelPos.x) + (ModelPos.y*ModelPos.y))*8.0;
    //add time to sqrdist to get an animated wave function, shifted into the 0-1 range
    float intensity = (sin(sqrdist + time) + 1.0) / 2.0;
    //  Interpolate colors
    vec3 color  = mix(ColorA, ColorB, intensity);
    //  Adjust color intensity for lighting (interpolated from vertex shader values)
    color *= LightIntensity;
    //  Pad color with alpha
    gl_FragColor = vec4(color,1.0);
}
