Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 7

I started by combining Ex11 and Ex13 for this assignment. I then added several images I found on the internet, cropped to 1024x1024.
I used four shaders for this assignment: Blit, Blur, Sharpen, and Flatten. The fifth filter, Posterize is a combination of multiple
passes of Blur, followed by a single pass of Flatten. Flatten only ever runs once pass. I had initially set out to create an 
"impressionist painting" filter, but this turned out to be simpler, and I didn't have time to implement it, so ended up with "posterize"
instead.

Some interesting setting combinations for Posterize:
Image 1, Passes 20, Colors 5
Image 2, Passes 30, Colors 7
Image 3, Passes 20+, Colors 10
Image 4, Passes 20-30, Colors 10

I spent roughly 15 hours on this assignment.

This was tested on Windows 7 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL.