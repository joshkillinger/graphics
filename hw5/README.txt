Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 5

I used Ex08 as the starting point for this assignment.
I relied heavily on the tutorials at https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Tutorial/.
I first modified the shaders to accept lighting and texture information. The challenge here was with the view vector.
For some reason, I could not get the specular reflection to work properly using the math from the previous assignment.
I was able to get the specular reflection working if I simply set the view vector to (0,0,1). I assume this only works because
we were not using perspective projection. I was able to get the texture mapped, although at this point I realized that for
some reason the X direction is flipped in WebGL. I finalized my shaders by adding two additional lights, and there are now
two colored point lights on the sides of the scene(with intensity falloff) and a white directional light.

I then set out to get OBJ data loaded into the scene. This turned out to be a huge pain. I tried all number of methods,
but finally settled on just using a library to parse the data, which still provided me direct access to the buffers.
This meant I still had to make the WebGL calls myself. I could not figure out how to export UV data with Blender, though,
despite applying a material, texture in UV mapping mode, and exporting with UV data, the UV data still wouldn't show up in
the files. Because I couldn't get both texturing and obj importing to work at the same time, I made two different html files.

Things I learned about index buffers: All data buffers (vertex, normal, color, UV) need to have a one-to-one mapping by index.
You then only pass one index buffer, which is used to index all attribute buffers simultaneously.

Cube with raw vertex data as buffer and texture: hw5-tex.html
Cone as imported OBJ data as indices and no texture: hw5-obj.html

I spent roughly 15 hours on this assignment. Javascript was my biggest roadblock with this assignment.

This was tested on Windows 7 x64 in Firefox.