#version 410 core

// Large point texture

uniform sampler2D Texture;

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
   //  Sample texture
   vec4 color = texture2D(Texture,TexCoord);
   //  Set alpha as 1/3 the color
   //color.a = length(color.rgb);
   //  Blend point color with texture
  // gl_FragColor = gl_Color*color;
   FragColor = Color;
   //FragColor = vec4(1,1,1,1);
}
