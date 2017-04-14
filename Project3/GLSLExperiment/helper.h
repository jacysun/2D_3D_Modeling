#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

typedef struct Face {
	int vertex1;
	int vertex2;
	int vertex3;
	vec3 normal;
} Face;

typedef struct Ply {
	int vertexNum;
	int faceNum;
	color4 color;
	point4 *vertices;
	Face *faces;
	point4 extentBox[8];
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


typedef struct CTMStack {
	mat4 matrix;
	struct CTMStack *next;
} CTMStack;

void initStack(mat4 matrix);
void pushMatrix();
void pushMatrix(mat4 matrix);
void popMatrix();
mat4 topMatrix();
void clearStack();
void updateModelMatrix(int loc, mat4 matrix);