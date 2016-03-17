#version 120

// Large point texture


uniform sampler2D img;

void main()
{
   //  Sample texture
   vec4 color = texture2D(img,gl_TexCoord[0].st);
   //  Set alpha as 1/3 the color
   //color.a = length(color.rgb);
   //  Blend point color with texture
   gl_FragColor = gl_Color*color;
}
