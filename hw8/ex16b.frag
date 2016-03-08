//  Sun fragment shader

uniform sampler3D Noise3D;
const float Scale = 1.2;

const vec4 Sun1 = vec4(0.8,0.7,0.0,1.0);
const vec4 Sun2 = vec4(0.6,0.1,0.0,1.0);

void main(void)
{
   vec4 nv = texture3D(Noise3D,Scale*gl_TexCoord[0].xyz);
   float n = abs(nv[0] - 0.25)
           + abs(nv[1] - 0.125)
           + abs(nv[2] - 0.0625)
           + abs(nv[3] - 0.03125);
   n = clamp(6.0*n,0.0,1.0);

   //  Apply sky color
   gl_FragColor = length(gl_Color.xyz) * mix(Sun1 , Sun2 , n);
}

