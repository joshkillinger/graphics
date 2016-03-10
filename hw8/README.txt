Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 8

I started by creating a quad object and applying a normal map to it. I simplified this task by requiring that the quad be axis aligned with the normal (0,1,0) and tangent (1,0,0).

My intent was then to create an animated normal map from a 3D noise texture. I ran into some issues when attempting this. Windows REALLY doesn't like 3D textures in OpenGL,
and Qt doesn't seem to support it either. I then scaled this back to creating a static normal map from a 2D noise texture. I then found a 2D perlin noise class online.
Once I had the perlin noise class, I planned to generate a height-map, and convert it to a normal map. I extended the provided Vector3 class in preparation for this. This turned
out to be a good refresher in operator overloading in C++. This is where I ended up running out of time. My next step was to build the height-map. Then I was going to convert it
to a normal map with the following pseudocode algorithm based on treating each height-map point as a vertex:

for each interior vertex V {
  Vector3 sum(0.0, 0.0, 0.0);
  for each of the six triangles T that share V {
    const Vector3 side1 = T.v1 - T.v0;
    const Vector3 side2 = T.v2 - T.v1;
    const Vector3 contribution = Cross(side1, side2);
    sum += contribution;
  }
  sum.Normalize();
  V.normal = sum;
}

I would then either create a new shader to treat this as the direct normal representation, or translate it to tangent space. As a workaround for the 3D texture issue I did have
the idea to generate N 2D noise textures, and swap 2 of them at a time, interpolating between them in the shader, similar to the globe example. I don't know if this would have worked.

I found a sample normal map (hexes) here: https://www.filterforge.com/filters/6624-normal.jpg
Perlin noise class is based on http://www.flipcode.com/archives/Perlin_Noise_Class.shtml

I spent roughly 9 hours on this assignment.

This was tested on Windows 7 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL.