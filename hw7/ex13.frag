// Inter-image processing

uniform int       mode;
uniform float     frac;
uniform sampler2D img0;
uniform sampler2D img1;

void main()
{
   vec4 pix0 = texture2D(img0,gl_TexCoord[0].st);
   vec4 pix1 = texture2D(img1,gl_TexCoord[0].st);
   switch (mode)
   {
      //  First image
      case 0:
         gl_FragColor = pix0;
         break;
      //  Second image
      case 1:
         gl_FragColor = pix1;
         break;
      //  Transition
      case 2:
         gl_FragColor = mix(pix0,pix1,frac);
         break;
      //  Absolute difference
      case 3:
         gl_FragColor = abs(pix1-pix0);
         break;
      //  False color
      case 4:
         gl_FragColor = vec4(length(pix0.rgb),0,length(pix1.rgb),1);
         break;
      default:
         discard;
   }
}
