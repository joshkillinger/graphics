Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 10 (Resubmission)

I used HW9 and Ex22 as a starting point for this assignment. My intent was to create a compute shader that would calculate Conway's Game of Life. The board would then be rendered to the screen for viewing pleasure. I had also seen some examples where users could "paint" cells onto the board with the mouse. I had planned to implement this as well.
I ran into several major snags, however. First, I discovered that Qt does not support SSBOs. This is problematic, but not a show-stopper for a compute shader. The second issue was that I could not get a very basic compute shader to compile. I have no idea why this was. I put a total of about 5 hours into this approach.
I was then advised to try a CUDA project, so I abandoned the compute shader and set about installing CUDA. This turned out to be quite frustrating as well, but I was eventually able to get Ex23 into Visual Studio 2010 and converted to C++. Just getting this functional took about 7 hours. I likely would have had more success in that span of time with installing and setting up a GLUT environment and continuing with my Conway's approach.

RESUBMISSION CHANGES:
I converted the code to perform matrix times a vector multiplication. I actually had a terrible speedup, likely because the operations per float are very low. I was able to see a speedup of 1.348 on a matrix of 10240x10240, and if I went with much larger of a matrix my graphics card ran out of memory.

I spent roughly 15 hours on this assignment.

This project was tested on Windows 10 x64 on a GTX 470 using Visual Studio 2010 and CUDA 7.5.