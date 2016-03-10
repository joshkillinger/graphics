#version 120

//  Per-pixel Phong lighting with normalmap
//  Vertex shader

varying vec3 View;
varying vec3 Light;
varying vec4 Ambient;

void main()
{
   //  tangent space calculations
   vec3 N = vec3(gl_ModelViewMatrix * vec4(gl_Normal,0.0));
   vec3 T = vec3(gl_ModelViewMatrix * vec4(1.0,0.0,0.0,0.0));
   vec3 B = vec3(gl_ModelViewMatrix * vec4(0.0,0.0,1.0,0.0));
   mat3 TBN = transpose(mat3(T, B, N));
   
   //  Lighting values needed by fragment shader
   //  Vertex location in modelview coordinates
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   
   //  Direction to Light in tangent space
   Light  = TBN * (vec3(gl_LightSource[0].position) - P);
   
   //  Eye position
   View  = TBN * (-P);
   
   //  Ambient color
   Ambient = gl_FrontMaterial.emission + gl_FrontLightProduct[0].ambient + gl_LightModel.ambient*gl_FrontMaterial.ambient;

   //  Texture coordinate for fragment shader
   gl_TexCoord[0] = gl_MultiTexCoord0;

   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
