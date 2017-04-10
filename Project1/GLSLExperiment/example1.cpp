// Starter program for HW 1. 
// Program draws a triangle. Study the program first
// The function generateGeometry can be modified to draw many 2D drawings (e.g. 2D Sierpinski Gasket)
// Generated using randomly selected vertices and bisection

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

typedef vec2 point2;
GLuint program;
GLuint ProjLoc;
GLuint ColorLoc;

point2 points[10][200][3000];
point2 drawPoints[200][100];
point2 gingerPoints[750000], fernPoints[750000];
float extents[10][4]; // left, right, bottom, top
int polylineCounts[10], pointCounts[10][200];
int windowHeight, windowWidth, originX, originY, seed;
int drawLineCount = 0, drawPointCount[200] = { 0 };
char currentState = 'p';
char* currentColor = "red";
int selectedFile = 9;
float r = 0;
bool firstPoint = true;
int gingerCount = 0, fernCount = 0;
float xMin = 100, xMax = -100, yMin = 100, yMax = -100;

char* files[10] = {
"data/dino.dat",
"data/birdhead.dat",
"data/dragon.dat",
"data/dragon.dat",
"data/house.dat",
"data/knight.dat",
"data/rex.dat",
"data/scene.dat",
"data/usa.dat",
"data/vinci.dat"
};

// remember to prototype
void drawThumbnails(void);
void drawPolylineFile(int fileIndex, int width, int height);
void drawPolylines(void);
void readPolylineFiles(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void reshape(int windowWidth, int windowHeight);
void checkRatio(int width, int height);
void drawGingerBreadMan(void);
void drawFern(void);

int main(int argc, char ** argv);


void drawThumbnails(void) {
	for (int i = 0; i < 10; i++) {
		originX = ((windowWidth)/ 10) * i;
		originY = windowHeight - windowWidth/10;
		drawPolylineFile(i, (windowWidth)/11, windowWidth/11);
	}
	originX = originY = 0;
}

void readPolylineFiles(void) {
	for (int i = 0; i < 10; i++) {
		// read all points from file
		ifstream infile;
		infile.open(files[i]);
		string comment, star;
		float x, y;
		getline(infile, comment);
		getline(infile, star);
		infile >> extents[i][0] >> extents[i][3] >> extents[i][1] >> extents[i][2];
		infile >> polylineCounts[i];
		for (int j = 0; j < polylineCounts[i]; j++) {
			infile >> pointCounts[i][j];
			for (int k = 0; k < pointCounts[i][j]; k++) {
				infile >> x >> y;
				points[i][j][k] = point2(x, y);
			}
		}
		infile.close();
	}
}


void drawPolylineFile(int fileIndex, int width, int height)
{
	// define world window
	mat4 ortho = Ortho2D(extents[fileIndex][0], extents[fileIndex][1], extents[fileIndex][2]-0.02, extents[fileIndex][3]);
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	// define viewport
	r = (extents[fileIndex][1] - extents[fileIndex][0]) / (extents[fileIndex][3] - extents[fileIndex][2]+0.02);
	checkRatio(width, height);

	for (int i = 0; i < polylineCounts[fileIndex]; i++) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(points[fileIndex][i]), points[fileIndex][i], GL_STATIC_DRAW);
		glDrawArrays(GL_LINE_STRIP, 0, pointCounts[fileIndex][i]);    // draw the points
	}
}


void drawPolylines(void) {

	// define world window
	mat4 ortho = Ortho2D(0.0, windowWidth, 0.0, windowHeight-windowWidth/10);
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	// define viewport
	r = (float)windowWidth / (windowHeight-windowWidth/10);
	checkRatio(windowWidth, windowHeight-windowWidth/10);
	
	for (int i = 0; i < drawLineCount; i++) {
		if (i == drawLineCount - 1 && firstPoint == true) {
			glBufferData(GL_ARRAY_BUFFER, sizeof(drawPoints[i]), drawPoints[i], GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);
			firstPoint = false;
		}
		else {
			glBufferData(GL_ARRAY_BUFFER, sizeof(drawPoints[i]), drawPoints[i], GL_STATIC_DRAW);
			glDrawArrays(GL_LINE_STRIP, 0, drawPointCount[i]);
		}
	}
}


void drawGingerBreadMan(void) {
	if (gingerCount == 0) { // generates points first
		int pX = 115, pY = 121, qX, qY, M = 40, L = 3;
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 50000; j++) {
				qX = M * (1 + 2 * L) - pY + abs(pX - L*M);
				qY = pX;
				gingerPoints[gingerCount] = point2(pX, pY);
				gingerCount++;
				pX = qX;
				pY = qY;
			}
		}
	}
	
	// define world window
	mat4 ortho = Ortho2D(0.0, 640, 0.0, 440.0);
	glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

	// define viewport
	r = (float)640.0 / 440.0;
	checkRatio(windowWidth, windowHeight - windowWidth/9);

	glBufferData(GL_ARRAY_BUFFER, sizeof(gingerPoints), gingerPoints, GL_STATIC_DRAW);
	glDrawArrays(GL_POINTS, 0, gingerCount);
}


void drawFern(void) {
	if (fernCount == 0) { // generate points first
		float a[4] = { 0.0, 0.2, -0.15, 0.85 };
		float b[4] = { 0.0, 0.23, 0.26, -0.04 };
		float c[4] = { 0.0, -0.26, 0.28, 0.04 };
		float d[4] = { 0.16, 0.22, 0.24, 0.85 };
		float tx[4] = { 0.0, 0.0, 0.0, 0.0 };
		float ty[4] = { 0.0, 1.6, 0.44, 1.6 };

		float oldX = 0.0, oldY = 0.0, newX, newY, prob;
		int index;

		for (int i = 0; i < 75000; i++) {
			prob = (float)(rand() % 100 + 1) / 100;
			if (prob <= 0.01) { index = 0; }
			else if (prob <= 0.08) { index = 1; }
			else if (prob <= 0.15) { index = 2; }
			else { index = 3; }

			newX = a[index] * oldX + c[index] * oldY + tx[index];
			newY = b[index] * oldX + d[index] * oldY + ty[index];
			fernPoints[fernCount] = point2(newX, newY);
			fernCount++;
			if (newX < xMin) { xMin = newX; }
			if (newX > xMax) { xMax = newX; }
			if (newY < yMin) { yMin = newY; }
			if (newY > yMax) { yMax = newY; }
			oldX = newX;
			oldY = newY;
		}
	}

		// define world window
		mat4 ortho = Ortho2D(xMin-5, xMax, yMin, yMax);
		glUniformMatrix4fv(ProjLoc, 1, GL_TRUE, ortho);

		// define viewport
		r = (float)(xMax+5 - xMin) / (yMax - yMin);
		checkRatio(windowWidth, windowHeight - windowWidth / 9);

		glBufferData(GL_ARRAY_BUFFER, sizeof(fernPoints), fernPoints, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, fernCount);
}


void initGPUBuffers( void )
{
	// Create a vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
}


void shaderSetup( void )
{
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

	ProjLoc = glGetUniformLocation(program, "Proj");
	ColorLoc = glGetUniformLocation(program, "myColor");

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 );        // sets white as color used to clear screen
}


void display( void )
{
	// All drawing happens in display function
    glClear( GL_COLOR_BUFFER_BIT );    // clear window

	switch (currentState) {
	case 'p':
		drawThumbnails();
		drawPolylineFile(selectedFile, windowWidth, windowHeight - windowWidth / 9);
		break;
	case 't':
		drawThumbnails();
		srand(seed);
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				originX = (windowWidth / 5) * i;
				originY = ((windowHeight - windowWidth / 10) / 5) * j;
				drawPolylineFile(rand() % 10, windowWidth / 6, (windowHeight - windowWidth / 10) / 6);
			}
		}
		originX = originY = 0;
		break;
	case 'e':
		drawThumbnails();
		if (drawLineCount > 0) {
			drawPolylines();
		}
		break;
	case 'g':
		drawThumbnails();
		drawGingerBreadMan();
		break;
	case 'f':
		drawThumbnails();
		drawFern();
		break;
	default:
		break;
	}
	
	glFlush(); // force output to graphics hardware
}


void keyboard( unsigned char key, int x, int y )
{
	// keyboard handler

    switch ( key ) {
    case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;

	case 'p':
		currentState = 'p';
		display();
		break;
	case 't':
		currentState = 't';
		seed = rand();
		display();
		break;
	case 'e':
		currentState = 'e';
		drawLineCount = 0;
		memset(drawPointCount, 0, sizeof(drawPointCount));
		firstPoint = true;
		display();
		break;
	case 'b':
		if (currentState == 'e') {
			firstPoint = true;
		}
		break;
	case 'g':
		currentState = 'g';
		display();
		break;
	case 'f':
		currentState = 'f';
		display();
		break;
	case 'c':
		if (strcmp(currentColor, "red") == 0) {
			glUniform4f(ColorLoc, 0.0, 1.0, 0.0, 1.0);
			currentColor = "green";
		} else if (strcmp(currentColor, "green") == 0) {
			glUniform4f(ColorLoc, 0.0, 0.0, 1.0, 1.0);
			currentColor = "blue";
		} else {
			glUniform4f(ColorLoc, 1.0, 0.0, 0.0, 1.0);
			currentColor = "red";
		}
		display();
		break;
	default:
		break;
    }
}


void mouse(int button, int state, int x, int y) {

	// mouse handler
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		switch (currentState) {
		case 'p':
			if (y < windowWidth / 10) {
				selectedFile = x * 10 / windowWidth;
			}
			break;
		case 'e':
			if (y > windowWidth / 10) {
				if (firstPoint) {
					drawLineCount++;
				}
				drawPoints[drawLineCount-1][drawPointCount[drawLineCount-1]] = point2((float)x, (float)(windowHeight - y));
				drawPointCount[drawLineCount-1]++;
			}
			break;
		default:
			break;
		}
	}
}


void reshape(int w, int h) {

	windowWidth = w - 8 ;
	windowHeight = h - 8;
	if (r != 0) {
		checkRatio(windowWidth, windowHeight);
	}
}


void checkRatio(int width, int height) {
	if (r > (float)width / height) {
		glViewport(originX, originY, width, width / r);
	}
	else if (r < (float)width / height) {
		glViewport(originX, originY, height*r, height);
	}
	else {
		glViewport(originX, originY, width, height);
	}
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       // intialize GLUT  
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ); // single framebuffer, colors in RGB
    glutInitWindowSize( 640, 480 );                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100,150);               // Top left corner at (100, 150)
    glutCreateWindow( "PoliBook" );            // Create Window

    glewInit();		                             // init glew
	readPolylineFiles();
    initGPUBuffers( );							   // Create GPU buffers
    shaderSetup( );                             // Connect this .cpp file to shader file
	
	glPointSize(2);                   // set point size 5 pixels     
	glUniform4f(ColorLoc, 1.0, 0.0, 0.0, 1.0);     //set default color in red

    glutDisplayFunc( display );                    // Register display callback function
    glutKeyboardFunc( keyboard );                  // Register keyboard callback function
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);

	// Can add minimalist menus here
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
    glutMainLoop();
    return 0;
}
