//converts pixel colors to a discrete value

uniform float range;
uniform sampler2D img;

void main()
{
   gl_FragColor = ceil(texture2D(img,gl_TexCoord[0].st) * range) / range;
}
