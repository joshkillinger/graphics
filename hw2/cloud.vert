#version 130

// Vertex shader for brick and mandelbrot shaders
// Derived from Orange Book Chapter 6

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPos;

// Center and zoom (for Mandelbrot set)
uniform vec3 loc;

in vec4 MCVertex;
in vec3 MCNormal;

//  Light intensity and model position required by fragment shader
out float LightIntensity;
out vec3  MCposition;

//  Phong lighting intensity only
float phong()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(vec4(MVMatrix * MCVertex).xyz);
   //  N is the object normal at P
   vec3 N = normalize(NormalMatrix * MCNormal);
   //  Light Position for light 0
   //vec3 LightPos = vec3(gl_LightSource[0].position.xyz);
   //  L is the light vector
   vec3 L = normalize(LightPos - P);
   //  R is the reflected light vector R = 2(L.N)N - L
   vec3 R = reflect(-L, N);
   //  V is the view vector (eye at the origin)
   vec3 V = normalize(-P);

   //  Diffuse light intensity is cosine of light and normal vectors
   float Id = max(dot(L,N) , 0.0);
   //  Shininess intensity is cosine of light and reflection vectors to a power
   float Is = (Id>0.0) ? pow(max(dot(R,V) , 0.0) , gl_FrontMaterial.shininess) : 0.0;

   //  Vertex color (ignores emission and global ambient)
   vec3 color = gl_FrontLightProduct[0].ambient.rgb
           + Id*gl_FrontLightProduct[0].diffuse.rgb
           + Is*gl_FrontLightProduct[0].specular.rgb;

   //  Vertex intensity
   return length(color);
}

void main()
{
   //  Scalar light intensity (for fragment shader)
   LightIntensity = phong();
   //  Save model coordinates (for fragment shader)
   //MCposition = loc.z*MCVertex.xy - loc.xy;
   MCposition = vec3(MCVertex.xyz - loc) * loc.z;
   //  Return fixed transform coordinates for this vertex
   gl_Position = MVPMatrix * MCVertex;
}
