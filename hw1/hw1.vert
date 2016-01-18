//  Basic vertex shader

varying vec4 color;

void main()
{
    //  Set vertex coordinates
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    color = .5 * (1 + (gl_Position / gl_Position[3]));
}
