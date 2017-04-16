In this project, we will explore hierarchical modeling of 3D meshes. 

Per-Pixel Lighting (smooth shading) Draw all your meshes in your hierarchy as solid meshes (not wireframe). Implement shaders for per-pixel lighting and flat shading. You will switch between shading types using keystrokes below. Apply some interesting colors and material properties to your 3D meshes. Don't forget to enable depth testing (Hidden surface removal)

Implement a spotlight (described on pages 263 and 264 of your text) The spotlight is like a cone and light only reaches objects inside the cone. Objects outside a certain angle, called the cutoff angle, phi, gets no light (only ambient component). Make the cutoff angle a variable that can be controlled with a keystroke

Matrix stack You will need to implement a matrix stack, including PushMatrix( ) and PopMatrix( ) routines. You may use a simple linked list to store the stack or use a more sophisticated tree structure.

Animation Define a suitable frame rate such as 30 FPS and animate your model at that speed. Every 1/30 of a second, the animator will call the display callback function to re-draw everything. By displaying objects at slightly different locations (corresponding to camera or object movements) every frame, you can simulate an animation. Be mindful of performance issues while you implement this homework. As your scene size grows, avoid unnecessary re-initialization, buffer recopying, and file reopening that could slow down your program considerably. 

Requirements: Your sculpture must consist of at least 6 unique meshes with interesting colors. Each mesh should have a different color. Your sculpture must contain at least 3 levels of hierarchy. See the second image above. The animation should move all parts of the sculpture. It should be physically plausible and make sense. --- that is, there are no magical, free-flying shapes or disjointed movements. Objects should remain connected in whatever way you connected them as you animate them. 

All mesh models should rotate counter-clockwise about their own Y axis. The arm of Level 1 of the hierarchy should rotate clockwise, the arm of level 2 of the hierarchy should rotate counter-clockwise, the arm of level 3 should rotate clockwise, and so on. Basically, the arms of consecutive levels of the hierarchy should alternate between rotating clockwise and counter-clockwise. 

Additional Keystrokes

Also implement the following additional keystrokes.
(User hits 'e'): Draw a box around each mesh showing the extents of each mesh. This keystroke should be a toggle button. If the extents box is not drawn, hitting e draws the extents box. If the extents box is already drawn, hitting e erases the extents box. Even though the mesh rotates about the Y axis, the extents box should not. It should be stationary around the mesh. If the mesh's width and length are different, you can just use the greater of the two dimensions as for your extents.

(User hits 's'): The arms of the hierarchy follow a gentle sinusoid. i.e. in addition to going round clockwise or counterclockwise, the hierarchy arms (and objects) also go slightly up and down like on a merry go round. The sinusoidal movements of all levels of the hierarchy should have the same amplitude (i.e. move up and down gently by the same amount). The s key should be a toggle key. If the sinusoidal movement is currently off, hitting the s key initiates the sinusoidal movement. If the sinusoidal movement is currently on, hitting the s key turns it off so that objects just move in a simple circle. Note that whether the sinusoidal movement is on or off, consecutive levels of the hierarchy should alternate between rotating clockwise and counter-clockwise.

(User hits 'p'): Increase spotlight cut off angle (increase cone angle) by a small amount
(User hits 'P'): Decrease spotlight cut off angle (decrease cone angle) by a small amount 

(User hits 'm'): The scene is shaded using per-pixel lighting (smooth shading)
(User hits 'M'): The scene is shaded using flat shading 


Please read:

opengl-quick-reference-card.pdf
glsl_quickref.pdf
The OpenGL Shading Language.pdf

before starting.


OGL manpages:
http://www.opengl.org/sdk/docs/manglsl/
