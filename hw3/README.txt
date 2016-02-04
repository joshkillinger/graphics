Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 3

I used Ex04 as the starting point for this assignment. When I started, I set out to create a convenient way to adjust the number of vertices
in the scene in a measurable way. I then fixed the Sphere object so that it properly created the vertices, and allowed the number of divisions
to be set through the GUI. I also wanted to ensure I was only measuring the time of the rendering, and not measuring extraneous operations.
In order to do that, I converted the Sphere object to store the vertexes in an OpenGL operation list, so that they weren't created each frame.
I expected to see a curve in the runtime, with the framerate inversely proportional to a function of the number of vertices (and the same for
number of pixels rendered), but was surprised to see that even with over 3.2M vertices and 1.6M pixels, on all shaders, there was no statistically
significant increase in render-time. I also expected to see a difference between the shaders, but on my desktop with a GTX 470, I saw roughly
5900 +- 100 FPS on all but the built-in shader, which ran roughly 100 FPS faster.

For the adjustments to the shaders, I tried several things, none of which produced statistically significant results. First, I inlined both the
Blinn (ex04bi.vert) and Phong (ex04ci.vert) shaders. Second, I wrote my own functions for dot products and normalize operations. (ex04bm.vert)
I used these in the Vertex Blinn, and again saw no statistically significant difference. Another oddity is that on my laptop (GT 745M) I saw
better performance when there were more vertices or more pixels, but saw no difference between the various shaders, save the default shader. I will
assume this has something to do with power saving techniques on an M variant graphics card, but can't confirm. I have confirmed my math is correct.
I am not as confident in the timing techniques, however, since there are known issues with Qt 5.2 and VSYNC.

Shader:				GTX 470 (all):		GT 745M (160k Px):	GT 745M (1.5M Px):
Default				6000+-100			2800+-100			3000+-100
Stored				5900+-100			2600+-100			3000+-100
V Blinn				5900+-100			2600+-100			3000+-100
V Blinn(Inline)		5900+-100			2600+-100			3000+-100
V Blinn(Manual)		5900+-100			2600+-100			3000+-100
V Phong				5900+-100			2600+-100			3000+-100
V Phong(Inline)		5900+-100			2600+-100			3000+-100
P Blinn				5900+-100			2600+-100			3000+-100
P Phong				5900+-100			2600+-100			3000+-100


I spent roughly 12 hours on this assignment.

This was tested on Windows 7 x64 and Windows 8.1 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL.