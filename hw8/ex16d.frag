//  Wood fragment shader

uniform sampler3D Noise3D;
const float Scale = 1.2;
const float Mag = 1.2;

const vec4  LightWood       = vec4(1.00,0.65,0.55,1.0);
const vec4  DarkWood        = vec4(0.55,0.27,0.07,1.0);
const float RingFreq        = 4.0;
const float LightGrain      = 1.0;
const float DarkGrain       = 0.1;
const float GrainThreshold  = 0.5;
const float GrainScale      = 27.0;

void main(void)
{
   vec4 nv = Mag*texture3D(Noise3D,Scale*gl_TexCoord[0].xyz);
   vec3 loc = gl_TexCoord[0].xyz + nv.xyz;

   float dist = RingFreq*sqrt(loc.x*loc.x + loc.z*loc.z);

   float r = 2.0 * fract(dist + nv[0] + nv[1] + nv[2]);
   if (r > 1.0) r = 2.0 - r;
 
   vec4 color = mix(LightWood , DarkWood , r);

   r = fract((gl_TexCoord[0].x + gl_TexCoord[0].z) * GrainScale + 0.5);
   nv[2] *= r;
   if (r < GrainThreshold)
      color += LightWood * LightGrain * nv[2];
   else
      color -= LightWood * DarkGrain * nv[2];

   gl_FragColor = length(gl_Color.xyz) * color;
}
