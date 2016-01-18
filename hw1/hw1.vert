//  Basic vertex shader

//uniform float time;
varying vec4 color;

void main()
{
    //  Set vertex coordinates
    //vec4 pos = gl_Vertex + vec4(0.5*sin(4.0*time),0.5*cos(4.0*time),0.5*sin(time),0.0);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    //  Use color unchanged
    //color = gl_Color;
    color = gl_Vertex;
    color[3] = 0.0;
    color = .5 * (1 + normalize(color));
    color[3] = 1.0;
}
