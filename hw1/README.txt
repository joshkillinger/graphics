Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 1

I used Ex01 as the starting point for this assignment. I added an additional option in the GUI under the shaders dropdown to select the NDC to RGB shader.
I also added sliders for the X, Y, and Z coordinates, allowing the model to be slid around between -2 and +2 in each dimension.
I additionally modified several of the classes to allow for reading of object coordinates and informing the viewer of the correct slider positions, which is performed when models are swapped.

My shader files are named hw1.vert and hw1.frag.

I think I spent roughly 8 hours on this assignment, but I am including installation and setup time. I'll be keeping better track of my time next week.

As an additional note, the Windows installation of Qt installs MingW as an optional addon.

This was unfortunately only tested on Windows 7 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL. I was unable to acquire a working linux box capable of testing this in time.