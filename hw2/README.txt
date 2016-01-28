Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 2

I used Ex03 as the starting point for this assignment. In short, I aimed relatively high on this assignment, but fell flat on my face.
I originally set out to create a wood-grain texture based on Perlin Noise. After hours of tearing my hair out, and the deadline rapidly
approaching with inverse proportion to amount of remaining sleep available, I started over making a simple animated ring texture. The speed
of the animation is adjustable, and also adjusts the speed of the brick animation. The animation can run backwards with a negative value.

I simply reused ex03b.vert, but my shader is rings.frag.

I spent roughly 15 hours on this assignment. The animated ring shader was maybe 2 hours.

As an additional note, the Windows installation of Qt installs MingW as an optional addon.

This was unfortunately only tested on Windows 7 x64 using MingW 4.9.2 for CMake, and Qt 5.2.1 MSVC2010 32bit OpenGL.