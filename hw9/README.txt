Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 9

I used ex19 as a starting point for this assignment, but also drew from ex20.
My first task was to experiment with the fire. I changed the vertex shader to fade in and out (adjusted alpha) based on how long it had been alive.
This resulted in a slightly more realistic looking fire.
I then loaded a second texture, and created a similar vertex shader, but altered the movement behavior using trig functions, the parameters of which
were sent in as attributes. I got this working nicely, making pleasant looking snowflakes wafting back and forth as they fall.
I then converted the snowflake shader to use the geometry shader from ex20. This is where I ran into numerous issues. For a while I had nothing
showing up, and Stas helped me figure out that I needed to rename the gl_PointCoord variable in the frag shader to gl_TexCoord[0]. I then had
it rendering, but there was an issue with z-sorting not working properly, and as a result, blending wasn't working correctly. I ended up fixing this
by disabling depth testing. I had looked into doing it various other ways, and some of the methods I came up with through my readings were Order
Independent Transparency, Transform Feedback, or changing the z buffer value (I saw something about this but now can't find it, its a big book). I
decided that for now, disabling depth testing would be good enough.
Finally, I implemented a rotation element to the snowflakes. This was done in the geometry shader.

I spent roughly 10 hours on this assignment.

This was tested on Windows 7 x64 and 8.1 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL.