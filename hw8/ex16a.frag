//  Cloud fragment shader

uniform sampler3D Noise3D;

const vec4 Sky   = vec4(0.0,0.0,0.8,1.0);
const vec4 Cloud = vec4(0.8,0.8,0.8,1.0);

void main(void)
{
   vec4 nv = texture3D(Noise3D,gl_TexCoord[0].xyz);
   float n = 1.5*(nv[0]+nv[1]+nv[2]+nv[3]+0.03135);

   //  Apply sky color
   gl_FragColor = length(gl_Color.xyz) * mix(Sky , Cloud , n);
}

