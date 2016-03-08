//  Marble fragment shader

uniform sampler3D Noise3D;
const float Scale = 1.2;

const vec4 Vein   = vec4(0.0,0.0,0.5,1.0);
const vec4 Marble = vec4(0.8,0.8,0.8,1.0);

void main(void)
{
   vec4 nv = texture3D(Noise3D,Scale*gl_TexCoord[0].xyz);
   float n = abs(nv[0] - 0.25)
           + abs(nv[1] - 0.125)
           + abs(nv[2] - 0.0625)
           + abs(nv[3] - 0.03125);
   n = 0.5*sin(12.0*n)+0.5;

   //  Apply sky color
   gl_FragColor = length(gl_Color.xyz) * mix(Vein , Marble , n);
}
