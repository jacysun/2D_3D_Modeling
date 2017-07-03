// Draws colored cube  

#include "Angel.h"
#include <fstream>
#include <iostream>
#include <string>


//----------------------------------------------------------------------------
int width = 0;
int height = 0;
int currentPly = 11;
char currentState = 'W';
float xMove = 0, yMove = 0, zMove = 0, xRotate = 0, pulse = 0, xShear = 0;
bool breathOut = true;
double pulseConstant = 0.1;
int pulseCount = 0;
bool drawNormal = false;
bool isPulsing = false;

// remember to prototype
void myInit(void);
void readPlyFile(void);
void drawPlyFile(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void idle(void);
mat4 ShearX(float xShear);

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

point4 points[80000];
color4 colors[80000];
point4 normals[50000];
color4 normalColors[50000];


// handle to program
GLuint program;

using namespace std;

char *files[43] = {
	"data/airplane.ply",
	"data/ant.ply",
	"data/apple.ply",
	"data/balance.ply",
	"data/beethoven.ply",
	"data/big_atc.ply",
	"data/big_dodge.ply",
	"data/big_porsche.ply",
	"data/big_spider.ply",
	"data/canstick.ply",
	"data/chopper.ply",
	"data/cow.ply",
	"data/dolphins.ply",
	"data/egret.ply",
	"data/f16.ply",
	"data/footbones.ply",
	"data/fracttree.ply",
	"data/galleon.ply",
	"data/hammerhead.ply",
	"data/helix.ply",
	"data/hind.ply",
	"data/kerolamp.ply",
	"data/ketchup.ply",
	"data/mug.ply",
	"data/part.ply",
	"data/pickup_big.ply",
	"data/pump.ply",
	"data/pumpa_tb.ply",
	"data/sandal.ply",
	"data/saratoga.ply",
	"data/scissors.ply",
	"data/shark.ply",
	"data/steeringweel.ply",
	"data/stratocaster.ply",
	"data/street_lamp.ply",
	"data/teapot.ply",
	"data/tennis_shoe.ply",
	"data/tommygun.ply",
	"data/trashcan.ply",
	"data/turbine.ply",
	"data/urn2.ply",
	"data/walkman.ply",
	"data/weathervane.ply"
};

typedef struct Face {
	int vertex1;
	int vertex2;
	int vertex3;
	point4 normal;
} Face;

typedef struct Ply {
	int vertexNum;
	int faceNum;
	point4 *vertices;
	Face *faces;
	float plyWidth;
	float plyHeight;
	float plyDepth;
	float scaleFactor;
	float xmin;
	float xmax;
	float ymin;
	float ymax;
	float zmin;
	float zmax;
} Ply;

Ply ply[43];

mat4 ShearX(float xShear) {
	mat4 c;
	c[0][1] = xShear;
	return c;
}

void readPlyFile(void) {
	for (int i = 0; i < 43; i++) {
		ifstream infile;
		infile.open(files[i]);
		string skip;
		getline(infile, skip);
		getline(infile, skip);
		infile >> skip >> skip >> ply[i].vertexNum; // get vertexCount
		getline(infile, skip);
		getline(infile, skip);
		getline(infile, skip);
		getline(infile, skip);
		infile >> skip >> skip >> ply[i].faceNum; // get faceCount
		getline(infile, skip);
		getline(infile, skip);
		getline(infile, skip);

		// vertex list and bounds
		ply[i].vertices = (point4 *)calloc(1, sizeof(point4)*ply[i].vertexNum);
		ply[i].xmin = ply[i].xmax = ply[i].vertices[0].x;
		ply[i].ymin = ply[i].ymax = ply[i].vertices[0].y;
		ply[i].zmin = ply[i].zmax = ply[i].vertices[0].z;
		for (int j = 0; j < ply[i].vertexNum; j++) {
			infile >> ply[i].vertices[j].x >> ply[i].vertices[j].y >> ply[i].vertices[j].z;
			ply[i].vertices[j].w = 1.0f;
			if (ply[i].vertices[j].x < ply[i].xmin) ply[i].xmin = ply[i].vertices[j].x;
			if (ply[i].vertices[j].x > ply[i].xmax) ply[i].xmax = ply[i].vertices[j].x;
			if (ply[i].vertices[j].y < ply[i].ymin) ply[i].ymin = ply[i].vertices[j].y;
			if (ply[i].vertices[j].y > ply[i].ymax) ply[i].ymax = ply[i].vertices[j].y;
			if (ply[i].vertices[j].z < ply[i].zmin) ply[i].zmin = ply[i].vertices[j].z;
			if (ply[i].vertices[j].z > ply[i].zmax) ply[i].zmax = ply[i].vertices[j].z;
		}
		ply[i].plyWidth = ply[i].xmax - ply[i].xmin;
		ply[i].plyHeight = ply[i].ymax - ply[i].ymin;
		ply[i].plyDepth = ply[i].zmax - ply[i].zmin;
		if (ply[i].plyWidth > ply[i].plyHeight) {
			ply[i].scaleFactor = 3.0f / ply[i].plyWidth;
		}
		else {
			ply[i].scaleFactor = 3.0f / ply[i].plyHeight;
		}

		// face list
		ply[i].faces = (Face *)calloc(1, sizeof(Face)*ply[i].faceNum);
		for (int k = 0; k < ply[i].faceNum; k++) {
			infile >> skip >> ply[i].faces[k].vertex1 >> ply[i].faces[k].vertex2 >> ply[i].faces[k].vertex3;
		}
		infile.close();
	}
}

void myInit(void)
{
	// Read all 43 ply files and create 43 Ply objects
	readPlyFile();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(2, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
	//glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	//glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );


	// Load shaders and use the resulting shader program
	program = InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(normals)));

	// sets the default color to clear screen
	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

void drawPlyFile(void)
{
	// change to GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//points = (point4 *)malloc(sizeof(point4)*ply[index].faceNum * 3);
	//colors = (color4 *)malloc(sizeof(color4)*ply[index].faceNum * 3);
	//point4 points[NumVertices];
	//color4 colors[NumVertices];

	for (int i = 0, j = 0, k= 0; j < ply[currentPly].faceNum; i = i + 3, j++, k = k+2) {
		points[i] = ply[currentPly].vertices[ply[currentPly].faces[j].vertex1];
		points[i + 1] = ply[currentPly].vertices[ply[currentPly].faces[j].vertex2];
		points[i + 2] = ply[currentPly].vertices[ply[currentPly].faces[j].vertex3];
		colors[i] = colors[i + 1] = colors[i + 2] = color4(1.0, 0.0, 0.0, 1.0); //set color to red
		// calculate normal for each face
		ply[currentPly].faces[j].normal.x = (points[i].y - points[i + 1].y)*(points[i].z + points[i + 1].z) + (points[i + 1].y - points[i + 2].y)*(points[i + 1].z + points[i + 2].z) + (points[i + 2].y - points[i].y)*(points[i + 2].z + points[i].z);
		ply[currentPly].faces[j].normal.y = (points[i].z - points[i + 1].z)*(points[i].x + points[i + 1].x) + (points[i + 1].z - points[i + 2].z)*(points[i + 1].x + points[i + 2].x) + (points[i + 2].z - points[i].z)*(points[i + 2].x + points[i].x);
		ply[currentPly].faces[j].normal.z = (points[i].x - points[i + 1].x)*(points[i].y + points[i + 1].y) + (points[i + 1].x - points[i + 2].x)*(points[i + 1].y + points[i + 2].y) + (points[i + 2].x - points[i].x)*(points[i + 2].y + points[i].y);
		ply[currentPly].faces[j].normal.w = 0.0;
		ply[currentPly].faces[j].normal = (0.1/ply[currentPly].scaleFactor) *  Angel::normalize(ply[currentPly].faces[j].normal); 
		normals[k] = point4((points[i].x + points[i + 1].x + points[i + 2].x) / 3.0f, (points[i].y + points[i + 1].y + points[i + 2].y) / 3.0f, (points[i].z + points[i + 1].z + points[i + 2].z) / 3.0f, 1.0);
		normals[k + 1] = point4(normals[k].x + ply[currentPly].faces[j].normal.x, normals[k].y + ply[currentPly].faces[j].normal.y, normals[k].z + ply[currentPly].faces[j].normal.z, 1.0);
		normalColors[k] = normalColors[k + 1] = color4(0.0, 1.0, 0.0, 1.0);
	}

	if (isPulsing == true) {
		for (int i = 0, j = 0; j < ply[currentPly].faceNum; i = i + 3, j++) {
			if (breathOut == true) {
				points[i] += pulseConstant * ply[currentPly].faces[j].normal;
				points[i + 1] += pulseConstant * ply[currentPly].faces[j].normal;
				points[i + 2] += pulseConstant * ply[currentPly].faces[j].normal;
			}
			else {
				points[i] -= pulseConstant * ply[currentPly].faces[j].normal;
				points[i + 1] -= pulseConstant * ply[currentPly].faces[j].normal;
				points[i + 2] -= pulseConstant * ply[currentPly].faces[j].normal;
			}
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(colors) + sizeof(normalColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(normals), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(colors), sizeof(normalColors), normalColors);

	// draw functions should enable then disable the features 
	// that are specifit the themselves
	// the depth is disabled after the draw 
	// in case you need to draw overlays
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, ply[currentPly].faceNum * 3);
	glDisable(GL_DEPTH_TEST);

	if (drawNormal == true) {
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINES, sizeof(points) / sizeof(point4), ply[currentPly].faceNum * 2);
		glDisable(GL_DEPTH_TEST);
	}
}

//----------------------------------------------------------------------------
// this is where the drawing should happen
void display(void)
{
	// SOME RANDOM TIPS
	//========================================================================
	// remember to enable depth buffering when drawing in 3d

	// avoid using glTranslatex, glRotatex, push and pop
	// pass your own view matrix to the shader directly
	// refer to the latest OpenGL documentation for implementation details

	// Do not set the near and far plane too far appart!
	// depth buffers do not have unlimited resolution
	// surfaces will start to fight as they come nearer to each other
	// if the planes are too far appart (quantization errors :(   )

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the window

	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)60.0, (GLfloat)width / (GLfloat)height, (GLfloat)0.1, (GLfloat) 10000.0);

	float viewMatrixf[16];
	viewMatrixf[0] = perspectiveMat[0][0]; viewMatrixf[4] = perspectiveMat[0][1];
	viewMatrixf[1] = perspectiveMat[1][0]; viewMatrixf[5] = perspectiveMat[1][1];
	viewMatrixf[2] = perspectiveMat[2][0]; viewMatrixf[6] = perspectiveMat[2][1];
	viewMatrixf[3] = perspectiveMat[3][0]; viewMatrixf[7] = perspectiveMat[3][1];

	viewMatrixf[8] = perspectiveMat[0][2]; viewMatrixf[12] = perspectiveMat[0][3];
	viewMatrixf[9] = perspectiveMat[1][2]; viewMatrixf[13] = perspectiveMat[1][3];
	viewMatrixf[10] = perspectiveMat[2][2]; viewMatrixf[14] = perspectiveMat[2][3];
	viewMatrixf[11] = perspectiveMat[3][2]; viewMatrixf[15] = perspectiveMat[3][3];

	Angel::mat4 modelMat = Angel::identity();
	modelMat = modelMat * Angel::Scale(ply[currentPly].scaleFactor, ply[currentPly].scaleFactor, ply[currentPly].scaleFactor) * Angel::Translate(-(ply[currentPly].xmin + ply[currentPly].xmax) / 2.0f + xMove, -(ply[currentPly].ymin + ply[currentPly].ymax) / 2.0f + yMove, -sqrt(pow(ply[currentPly].xmax - ply[currentPly].xmin, 2) + pow(ply[currentPly].ymax - ply[currentPly].ymin, 2) + pow(ply[currentPly].zmax - ply[currentPly].zmin, 2)) + zMove) * Angel::RotateY(0.0f) * Angel::RotateX(xRotate) * ShearX(xShear);
	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0]; modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0]; modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0]; modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0]; modelMatrixf[7] = modelMat[3][1];

	modelMatrixf[8] = modelMat[0][2]; modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2]; modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2]; modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2]; modelMatrixf[15] = modelMat[3][3];

	// set up projection matricies
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, modelMatrixf);
	GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewMatrixf);

	drawPlyFile();


	glFlush(); // force output to graphics hardware

    // use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'W':
		currentState = 'W';
		currentPly = 0;
		xShear = 0;
		pulseCount = 0;
		xRotate = 0;
		breathOut = true;
		xMove = yMove = zMove = xShear = 0;
		display();
		break;
	case 'N':
		if (currentPly == 42) {
			currentPly = 0;
		}
		else {
			currentPly++;
		}
		currentState = 'N';
		xShear = 0;
		pulseCount = 0;
		xRotate = 0;
		xMove = yMove = zMove = xShear = 0;
		breathOut = true;
		display();
		break;
	case 'P':
		if (currentPly == 0) {
			currentPly = 42;
		}
		else {
			currentPly--;
		}
		currentState = 'P';
		xShear = 0;
		pulseCount = 0;
		xRotate = 0;
		xMove = yMove = zMove = xShear = 0;
		breathOut = true;
		display();
		break;
	case 'X':
		if (currentState == 'X') {
			currentState = 'N';
		}
		else {
			currentState = 'X';
		}
		break;
	case 'x':
		if (currentState == 'x') {
			currentState = 'N';
		}
		else {
			currentState = 'x';
		}
		break;
	case 'Y':
		if (currentState == 'Y') {
			currentState = 'N';
		}
		else {
			currentState = 'Y';
		}
		break;
	case 'y':
		if (currentState == 'y') {
			currentState = 'N';
		}
		else {
			currentState = 'y';
		}
		break;
	case 'Z':
		if (currentState == 'Z') {
			currentState = 'N';
		}
		else {
			currentState = 'Z';
		}
		break;
	case 'z':
		if (currentState == 'z') {
			currentState = 'N';
		}
		else {
			currentState = 'z';
		}
		break;
	case 'R':
		if (currentState == 'R') {
			currentState = 'N';
		}
		else {
			currentState = 'R';
		}
		break;
	case 'B':
		if (isPulsing == true) {
			isPulsing = false;
		}
		else {
			isPulsing = true;
		}
		break;
	case 'm':
		if (drawNormal == true) {
			drawNormal = false;
		}
		else {
			drawNormal = true;
		}
		display();
		break;
	case 'h':
		xShear += 0.1f;
		display();
		break;
	case 'H':
		xShear -= 0.1f;
		display();
		break;
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

void idle() {
	switch (currentState) {
	case 'X':
		xMove += ply[currentPly].plyWidth / 1000;
		glutPostRedisplay();
		break;
	case 'x':
		xMove -= ply[currentPly].plyWidth / 1000;
		glutPostRedisplay();
		break;
	case 'Y':
		yMove += ply[currentPly].plyHeight / 1000;
		glutPostRedisplay();
		break;
	case 'y':
		yMove -= ply[currentPly].plyHeight / 1000;
		glutPostRedisplay();
		break;
	case 'Z':
		zMove += ply[currentPly].plyDepth / 1000;
		glutPostRedisplay();
		break;
	case 'z':
		zMove -= ply[currentPly].plyDepth / 1000;
		glutPostRedisplay();
		break;
	case 'R':
		if (xRotate == 360.0) {
			xRotate = 0;
		}
		else {
			xRotate += 1.0f;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
	if (isPulsing == true) {
		if (breathOut == true) {
			pulseCount++;
			if (pulseCount == 300) {
				breathOut = false;
			}
		}
		else {
			pulseCount--;
			if (pulseCount == 0) {
				breathOut = true;
			}
		}
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------
// entry point
int main(int argc, char **argv)
{
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	width = 512;
	height = 512;

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
	glutCreateWindow("HW2");

	// init glew
	glewInit();

	// read files, set up shader and GPU buffer
	myInit();

	// assign handlers
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
	// frame rate can be controlled with 
	glutMainLoop();
	return 0;
}
