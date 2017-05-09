In this project you shall implement some of the image manipulation operations we discussed in class. The following [ Visual Studio solution ] contains a working program that reads in and displays a bmp file usain_bolt.bmp. Your task will be to program various GLSL fragment shaders that implement various operations that we learned in class and control the effects interactively using keystrokes. To start off, download the Visual Studio Solution, compile it and run it. I have tested it and it works in the Zoolab. 

To read in the usain_bolt.bmp file, the program uses use the [ libbmpread library ] which is a tiny, fast bitmap (.bmp) image file loader. Specifically, libbmpread is implemented as two files bmpread.c and bmpread.h which have been included in the Visual Studio starter code. bmp file loading functions and data structures are then used in the starter program to load the bmp file. 

You should implement some of the image-manipulation operations previously discussed in class. Your program behavior should be as follows. 

User hits:
Key 'O': Display original picture (usain_bolt.bmp)
Key 'L': Display picture with Luminance effect (gray scale)
Key 'N': Display image negative
Key 'D': Display image with edge detection effect
Key 'E': Display image with embossing effect
Key 'T': Display image with Toon rendering
Key 'W': Display image with Twirl Transformation
Key 'P': Display image with Ripple Transformation
Key 'S': Display image with Spherical Transformation

Please read:

opengl-quick-reference-card.pdf
glsl_quickref.pdf
The OpenGL Shading Language.pdf

before starting.


OGL manpages:
http://www.opengl.org/sdk/docs/manglsl/
