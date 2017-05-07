In this project you will add more realism to your hierarchy from project 3 by adding texturing and shadows.

The specific features you will implement in this homework are:
Texture the floor plane with grass and walls with stone: Texture map some grass onto the floor of your scene to make it look like grass is growing. Use the following grass texture [ grass.bmp ] Make it look like the walls were made of stone by texturing both walls using a stone texture [ stones.bmp] Use the libbmpread library above to read in your bmp texture files. Do not try to stretch these small textures over a large floor (or walls). Map each texture to the bottom left corner of the floor (or walls) undistorted and then use the appropriate parameters to repeat the texture to cover the entire floor. The example program in Appendix A.8 on page 638 of your text gives the complete working code of a program that does texturing. 

Shadows: Section 4.10 of your textbook describes a simple technique to render shadows using projection. Implement this shadow algorithm such that the shadows of the hierarchy/spinning cow are projected onto the floor plane and walls, and updated continuously as the hierarchy/cow spins. 

Environment Mapping: Add in reflective and refractive environment mapping using cubemaps to your scene. The example in section 7.9 (page 393) of your text presents a complete working example of a reflection map. The class slides present how to implement refraction. Use the following cube map with its [ cube map sides in 6 images ] as your environment map. For reference purposes, the complete environment map looks like this:

Summary of Your program behavior

Control your scene using the following keystrokes.
Key A: Toggle shadows ON/OFF. When ON, the shadows show up and when OFF the shadows do not show up. 

Key B Toggle ON/OFF between a scene with grass texture on floor and stone texture on the walls and a plain wall (no texturing). When ON, the floor is textured with grass and the walls are textured with stone. when OFF the floor and walls have no texturing, displaying a gray floor and blue walls. 

Key C: Toggle reflection ON/OFF. When ON, the hierarchy/cow PLY is drawn with reflection. When OFF, the hierarchy/cow PLY objects is drawn with no reflection (rendered as a solid model(s) with per-fragment lighting). Select an appropriate shininess (reflectivity) for the cow model.

Key D: Toggle refraction ON/OFF. When ON, the hierarchy/cow PLY is drawn with refraction. When OFF, the hierarchy/cow PLY file is drawn with no refraction ( rendered as a solid model(s) with per-fragment lighting). Select an appropriate refractive index for the hierarchy/cow model. 

Notes: No OpenGL fixed function commands (glBegin, glVertex, etc) or immediate mode drawing commands should be used in your program. All drawing should be done using shaders, retained mode, Vertex Buffer Objects, and glDrawArrays similar to the code in your textbook (and in your previous projects) 

Please read:

opengl-quick-reference-card.pdf
glsl_quickref.pdf
The OpenGL Shading Language.pdf

before starting.


OGL manpages:
http://www.opengl.org/sdk/docs/manglsl/
