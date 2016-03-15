#version 120

//  Lame fire


uniform   float time;      //  Time
attribute float Start;     //  Start time
attribute vec4  Vel;       //  Initial velocity, w = spin
attribute vec4  AmpFreq;   //  amplitude x/z = x/y, frequency x/z = z/w
attribute float Lifetime;

void main(void)
{
   //  Particle life is mod Start time
   float t = mod(time - Start,Lifetime);
   //  fade in and out
   float alpha = min(clamp((Lifetime - t), 0, 1), t * 4);

   //  Initial position
   vec4 vert = gl_Vertex;
   
   //  wavy movement
   vert += vec4(AmpFreq.x * sin(AmpFreq.z * t), Vel.y * t, AmpFreq.y * sin(AmpFreq.w * t), 0);
   //vert += vec4(sin(AmpFreq.z * t), Vel.y * t, sin(AmpFreq.w * t), 0);
   
   //  blue
   gl_FrontColor = vec4(0.5,0.5,1,alpha);
   //gl_FrontColor = abs(AmpFreq+1.0);
   //  Transform particle location
   gl_Position = gl_ModelViewProjectionMatrix*vert;
}
