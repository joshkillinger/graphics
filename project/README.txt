Josh Killinger
joshua.killinger@colorado.edu
864-978-4794
CPSC 5239
Final Project



This is a Qt 5.5, OpenGL 4 project, built on MSVC 2010 32-bit. Tested on Windows 10 x64, with a GTX 470 graphics card. (I suspect I will have to demostrate this to you in office hours, more on why I used 5.5 below)



Usage:
Right-click-and-drag: change view direction
Scroll-wheel: zoom
Left-click: FIRE ZE LAZORS!
Maximize and enjoy the show!



I have accomplished all my goals in my original proposal, plus a few other things.
Originally proposed functionality:
1. Shield is procedurally generated when hit, adjusting the alpha with the following formula:
	float intensity = clamp((10*Time[i]) - distanceFromHit, 0, M_PI);
	totalIntensity += sin(intensity) / (1 + distanceFromHit * 2);
  This intensity is summed over an array of 10 possible hits and saved as the alpha value for the fragment. The effect is that the point closest to the hit is brightest, and the effect spreads out over time, reducing in intensity with distance.
  One issue I ran into with the shield was that in order to properly display hits on a moving target, I had to convert the world coordinates of the hit point into the model space of the shield for storage and use in the shader. After some investigation and experimentation, I used the inverse of the model matrix of the shield.
  The blending function for the shield is:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  This has the effect of obscuring the object behind the shield.

2. Particle Systems. The size and position of the particles that make up a plasma shot are calculated based on time in a vertex shader, then billboarded in a geometry shader. Of note is the blending function used, which is:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  This has the effect of brightening the colors of the plasma to white in the center, while leaving a glow of color around the edges.
  Wolfram Alpha code for plasma particle size calculation visualization:
    Piecewise[{{sin(x),x<(pi/2)}, {1-((x-(pi/2))/(3.5*pi)), x>=(pi/2)}}] from 0 to (4*pi)
  Ideally I would have used a spline function to calculate the size of the particles, but I had to make sacrifices somewhere in the interest of time, and the particles look cool enough.
  
3. Ray-tracing. In order to detect when plasma hits a shield, I had to perform ray-tracing. This was done by each plasma shot, tracing a ray in front of it. If it hits an object within the distance that it moves, then it registers a hit with the shield that was hit. I also enabled plasma to be shot from screen clicks, using a ray traced through the near and far planes of the projection matrix to determine position and direction of the plasma, respectively.
  This required the use of quaternions. More on this later.
  
  

Additional functionality added:
1. I created a rudimentary game engine for this project, which included material, transform, hitbox, and behavior components for each object. Materials handle rendering, transforms handle transformations (and can have parent transformations), hitboxes received traced rays, and behaviors control things such as movement, ray-tracing, and firing. There was also a game loop, involving updating, opaque rendering, then transparent rendering.

2. Object pooling. I created an object pool for unused particles. Whenever I create a new plasma shot, I check for an unused one in my pool. If it exists, I reuse it, otherwise I create a new one.

3. OpenGL 4. I find that I prefer the more explicit nature of OpenGL 4, the same way I prefer more explicit, statically typed languages such as C++ or C# over implicit, dynamically typed languages such as Python. This required a little more work on the front end, but it ended up saving me time in the end, both through debugging and not having to refactor to it later.

4. Obj parsing. I converted your WaveOBJ parser to use OpenGL 4, since I could no longer rely on call lists. There are a few restrictions: it requires triangle faces (no quads or n-gons), and it ignores materials. I don't remember if it handles negative index references.

5. Qt 5.5. This might be an issue for you, since you mentioned you only had 5.2. I actually ran into issues with Qt 5.2's QQuaternion, where there was some missing functionality. I needed to be able to convert a direction vector into a quaternion, and it required me to either write a significant (and likely buggy, since I wasn't immediately familiar with the math required) extension to the existing library, or upgrade. I opted for the already tested and known working Qt 5.5 version. Qt 5.5 actually removed support for OpenGL 1, since they directly targeted OpenGL ES 2.0+. For this reason, I was very glad to be already using OpenGL 4. I just had to refactor a few of my Qt references and OpenGL calls, and clean out a few unneccesary OpenGL 1 things like axis drawing to get it working. 5.5 was a little weird to get used to, but I find it cleaner and easier to work with than 5.2.

6. Illumination mapping. I tacked this on at the very end. I use luminance maps, calculated in the fragment shader, to light up things like engines.



Things I would have liked to implement if I had more time, in order of importance:
1. Shadows.
2. Anti-Aliasing.
3. Smoother destruction of plasma particles. They currently all disappear when hitting a shield. Not hugely noticeable but it bugs me.
4. Skybox. Just to make it a little bit more interesting.
5. Specular mapping. One of the models I downloaded has one. Probably not any more difficult than the luminance map.
6. Bump/Normal maps. Very standard to have this feature.