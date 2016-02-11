Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 4

I used Ex06 as the starting point for this assignment. I extended the provided shaders, and added both lighting and texturing.
Once I achieved the minimum specification for the assignment, I started looking into structures as shader parameters.
It turns out that structures can be used fairly easily as uniforms, but can't be used as in or out parameters.
Even still, they do clean up the code and make it more maintainable and readable.

I spent roughly 10 hours on this assignment.

This was tested on Windows 7 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL.