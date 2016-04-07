Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Homework 10

I used HW9 and Ex22 as a starting point for this assignment. My intent was to create a compute shader that would calculate Conway's Game of Life. The board would then be rendered to the screen for viewing pleasure. I had also seen some examples where users could "paint" cells onto the board with the mouse. I had planned to implement this as well.
I ran into several major snags, however. First, I discovered that Qt does not support SSBOs. This is problematic, but not a show-stopper for a compute shader. The second issue was that I could not get a very basic compute shader to compile. I have no idea why this was. I put a total of about 5 hours into this approach.
I was then advised to try a CUDA project, so I abandoned the compute shader and set about installing CUDA. This turned out to be quite frustrating as well, but I was eventually able to get Ex23 into Visual Studio 2010 and converted to C++. Just getting this functional took about 7 hours. I likely would have had more success in that span of time with installing and setting up a GLUT environment and continuing with my Conway's approach.
The following is output from my version of Ex23. I doubt it is portable to Linux.

Josh Killinger: HW10
CUDA Device 0: GeForce GTX 470 Gflops 17.01 Processors 14 Threads/Block 1024
Fastest CUDA Device 0: GeForce GTX 470
Bw=32 Bn=32 n=1024
Host   Time = 9.793 s
Device Time = 0.1525 s
Speedup = 64.21
Difference = 8.732e-005

I spent roughly 12 hours on this assignment.

This project was tested on Windows 10 x64 on a GTX 470 using Visual Studio 2010 and CUDA 7.5.