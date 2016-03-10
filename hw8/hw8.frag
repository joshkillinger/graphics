//  Per-pixel Phong lighting
//  Fragment shader

uniform sampler2D tex;

varying vec3 View;
varying vec3 Light;
varying vec4 Ambient;

vec4 phong(vec3 norm)
{
   //  N is the object normal
   vec3 N = normalize(norm);
   //  L is the light vector
   vec3 L = normalize(Light);

   //  Emission and ambient color
   vec4 color = Ambient;
   //vec4 color = vec4(0);

   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*gl_FrontLightProduct[0].diffuse;
      //  R is the reflected light vector R = 2(L.N)N - L
      vec3 R = reflect(-L,N);
      //  V is the view vector (eye vector)
      vec3 V = normalize(View);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(R,V);
      if (Is>0.0) color += pow(Is,gl_FrontMaterial.shininess)*gl_FrontLightProduct[0].specular;
   }

   //  Return sum of color components
   return color;
}

void main()
{
    vec3 norm = normalize(texture2D(tex,gl_TexCoord[0].xy).rgb - .5);
    gl_FragColor = phong(norm);// * texture2D(tex,gl_TexCoord[0].xy);
}
