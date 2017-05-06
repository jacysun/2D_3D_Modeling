#include "Angel.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include <string>
#include "bmpread.h"
#include <complex>

typedef std::complex<float> complex;
//----------------------------------------------------------------------------
int width = 0;
int height = 0;
float yMove = 0.0, yRotate[7] = { 0.0 }, moveAmount = 0.001f;
int moveCount = 0;
bool drawBox = false;
bool sinusoid = false;
bool up = true, down = false;
bool drawShadow = false, drawTexture = false, drawReflection = false, drawRefraction = false;
bool wall = false;
float light[3];
mat4 m;


// remember to prototype
void myInit(void);
void readPlyFile(void);
void drawPlyFile(int index);
void extentBox(int index);
void line(void);
void drawWalls(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void idle(void);

point4 points[7][50000];
vec3 pointNormals[7][50000];
int pointIndex[7][50000];
point4 boxPoints[7][16];
point4 linePoints[20];
point4 tex_points[3][6];
vec2 tex_coords[3][6];
vec3 tex_normals[3][6];

// handle to program
GLuint program;

using namespace std;

bmpread_t wall_map, floor_map, env_map[6];
char* env_files[6] = { "data/nvposx.bmp","data/nvnegx.bmp", "data/nvposy.bmp", "data/nvnegy.bmp", "data/nvposz.bmp", "data/nvnegz.bmp"};
GLuint textures[2];
GLuint cubetex[1];

char *files[7] = {
	"data/airplane.ply",
	"data/kerolamp.ply",
	"data/apple.ply",
	"data/big_spider.ply",
	"data/mug.ply",
	"data/trashcan.ply",
	"data/urn2.ply"
};

Ply ply[7];

color4 light_ambient, light_diffuse, light_specular;
point4 light_position, light_direction;
color4 reflect_ambient, reflect_diffuse, reflect_specular;
color4 ambientProduct, diffuseProduct, specularProduct;

void extentBox(int index) {
		boxPoints[index][0] = ply[index].extentBox[0];
		boxPoints[index][1] = ply[index].extentBox[1];
		boxPoints[index][2] = ply[index].extentBox[2];
		boxPoints[index][3] = ply[index].extentBox[3];

		boxPoints[index][4] = ply[index].extentBox[0];
		boxPoints[index][5] = ply[index].extentBox[4];
		boxPoints[index][6] = ply[index].extentBox[5];
		boxPoints[index][7] = ply[index].extentBox[1];

		boxPoints[index][8] = ply[index].extentBox[2];
		boxPoints[index][9] = ply[index].extentBox[6];
		boxPoints[index][10] = ply[index].extentBox[5];
		boxPoints[index][11] = ply[index].extentBox[6];

		boxPoints[index][12] = ply[index].extentBox[7];
		boxPoints[index][13] = ply[index].extentBox[4];
		boxPoints[index][14] = ply[index].extentBox[7];
		boxPoints[index][15] = ply[index].extentBox[3];
}

void line(void) {
	linePoints[0] = point4((ply[0].xmin + ply[0].xmax) / 2.0f, ply[0].ymax * 0.9f, (ply[0].zmin + ply[0].zmax) / 2.0f, 1.0);
	linePoints[1] = point4((ply[0].xmin + ply[0].xmax) / 2.0f, ply[0].ymax + (ply[0].ymin + ply[0].ymax) / 2.0f, (ply[0].zmin + ply[0].zmax) / 2.0f, 1.0);

	linePoints[2] = point4((ply[0].xmin + ply[0].xmax) / 2.0f, ply[0].ymin, (ply[0].zmin + ply[0].zmax) / 2.0f, 1.0);
	linePoints[3] = point4((ply[0].xmin + ply[0].xmax) / 2.0f, ply[0].ymin - (ply[0].ymin + ply[0].ymax) / 2.0f, (ply[0].zmin + ply[0].zmax) / 2.0f, 1.0);
	linePoints[4] = point4(ply[0].xmin - 1800.0f, ply[0].ymin - (ply[0].ymin + ply[0].ymax) * 0.9f, (ply[0].zmin + ply[0].zmax) /2.0f, 1.0);// kerolamp 1
	linePoints[5] = point4(ply[0].xmin - 1800.0f, ply[0].ymin - (ply[0].ymin + ply[0].ymax) / 2.0f, (ply[0].zmin + ply[0].zmax) /2.0f, 1.0);
	linePoints[6] = point4(ply[0].xmax + 1800.0f, ply[0].ymin - (ply[0].ymin + ply[0].ymax) / 2.0f, (ply[0].zmin + ply[0].zmax) / 2.0f, 1.0);//apple 2
	linePoints[7] = point4(ply[0].xmax + 1800.0f, ply[0].ymin - (ply[0].ymin + ply[0].ymax) * 1.3f, (ply[0].zmin + ply[0].zmax) / 2.0f, 1.0);

	linePoints[8] = point4((ply[1].xmin + ply[1].xmax) / 2.0f, ply[1].ymin, (ply[1].zmin + ply[1].zmax) / 2.0f, 1.0);
	linePoints[9] = point4((ply[1].xmin + ply[1].xmax) / 2.0f, ply[1].ymin + (ply[1].ymin + ply[1].ymax) / 1.0f, (ply[1].zmin + ply[1].zmax) / 2.0f, 1.0);
	linePoints[10] = point4(ply[1].xmin - 600.0f, ply[1].ymin + (ply[1].ymin + ply[1].ymax) * 2.3f, (ply[1].zmin + ply[1].zmax) / 2.0f, 1.0);// spider 3
	linePoints[11] = point4(ply[1].xmin - 600.0f, ply[1].ymin + (ply[1].ymin + ply[1].ymax) / 1.0f, (ply[1].zmin + ply[1].zmax) / 2.0f, 1.0);
	linePoints[12] = point4(ply[1].xmax + 600.0f, ply[1].ymin + (ply[1].ymin + ply[1].ymax) / 1.0f, (ply[1].zmin + ply[1].zmax) / 2.0f, 1.0);//mug 4
	linePoints[13] = point4(ply[1].xmax + 600.0f, ply[1].ymin + (ply[1].ymin + ply[1].ymax) * 2.5f, (ply[1].zmin + ply[1].zmax) / 2.0f, 1.0);

	linePoints[14] = point4((ply[4].xmin + ply[4].xmax) / 2.0f, ply[4].ymin, (ply[4].zmin + ply[4].zmax) / 2.0f, 1.0);
	linePoints[15] = point4((ply[4].xmin + ply[4].xmax) / 2.0f, ply[4].ymin - (ply[4].ymin + ply[4].ymax) / 1.0f, (ply[4].zmin + ply[4].zmax) / 2.0f, 1.0);
	linePoints[16] = point4(ply[4].xmin - 0.18f, ply[4].ymin - (ply[4].ymin + ply[4].ymax) * 2.3f, (ply[4].zmin + ply[4].zmax) / 2.0f, 1.0);// 5
	linePoints[17] = point4(ply[4].xmin - 0.18f, ply[4].ymin - (ply[4].ymin + ply[4].ymax) / 1.0f, (ply[4].zmin + ply[4].zmax) / 2.0f, 1.0);
	linePoints[18] = point4(ply[4].xmax + 0.18f, ply[4].ymin - (ply[4].ymin + ply[4].ymax) / 1.0f, (ply[4].zmin + ply[4].zmax) / 2.0f, 1.0);//6
	linePoints[19] = point4(ply[4].xmax + 0.18f, ply[4].ymin - (ply[4].ymin + ply[4].ymax) * 2.3f, (ply[4].zmin + ply[4].zmax) / 2.0f, 1.0);

	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(boxPoints), sizeof(linePoints), linePoints);
}

void readPlyFile(void) {
	for (int i = 0; i < 7; i++) {
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

		// vertex list and extents
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
			ply[i].scaleFactor = 0.2f / ply[i].plyWidth;
		}
		else {
			ply[i].scaleFactor = 0.2f / ply[i].plyHeight;
		}

		// face list
		ply[i].faces = (Face *)calloc(1, sizeof(Face)*ply[i].faceNum);
		for (int j = 0, k = 0; k < ply[i].faceNum; j = j + 3, k++) { //i is the image index, j is point index for this image, k is face index for this image
			infile >> skip >> ply[i].faces[k].vertex1 >> ply[i].faces[k].vertex2 >> ply[i].faces[k].vertex3;
			points[i][j] = ply[i].vertices[ply[i].faces[k].vertex1];
			points[i][j + 1] = ply[i].vertices[ply[i].faces[k].vertex2];
			points[i][j + 2] = ply[i].vertices[ply[i].faces[k].vertex3];

			pointIndex[i][j] = ply[i].faces[k].vertex1;
			pointIndex[i][j + 1] = ply[i].faces[k].vertex2;
			pointIndex[i][j + 2] = ply[i].faces[k].vertex3;

			// calculate normal for each face
			ply[i].faces[k].normal.x = (points[i][j].y - points[i][j + 1].y)*(points[i][j].z + points[i][j + 1].z) + (points[i][j + 1].y - points[i][j + 2].y)*(points[i][j + 1].z + points[i][j + 2].z) + (points[i][j + 2].y - points[i][j].y)*(points[i][j + 2].z + points[i][j].z);
			ply[i].faces[k].normal.y = (points[i][j].z - points[i][j + 1].z)*(points[i][j].x + points[i][j + 1].x) + (points[i][j + 1].z - points[i][j + 2].z)*(points[i][j + 1].x + points[i][j + 2].x) + (points[i][j + 2].z - points[i][j].z)*(points[i][j + 2].x + points[i][j].x);
			ply[i].faces[k].normal.z = (points[i][j].x - points[i][j + 1].x)*(points[i][j].y + points[i][j + 1].y) + (points[i][j + 1].x - points[i][j + 2].x)*(points[i][j + 1].y + points[i][j + 2].y) + (points[i][j + 2].x - points[i][j].x)*(points[i][j + 2].y + points[i][j].y);
			ply[i].faces[k].normal = Angel::normalize(ply[i].faces[k].normal);
		}

		// average the normals of every face this vertex belongs to
		for (int p = 0; p < ply[i].faceNum * 3; p++) {
			pointNormals[i][p] = vec3(0.0, 0.0, 0.0);
			for (int q = 0; q < ply[i].faceNum; q++) {
				if (ply[i].faces[q].vertex1 == pointIndex[i][p] || ply[i].faces[q].vertex2 == pointIndex[i][p] || ply[i].faces[q].vertex3 == pointIndex[i][p]) {
					pointNormals[i][p] += ply[i].faces[q].normal;
				}
			}
			pointNormals[i][p] = Angel::normalize(pointNormals[i][p]);
		}

		//extent box
		ply[i].extentBox[0] = point4(ply[i].xmin, ply[i].ymax, ply[i].zmax, 1.0);
		ply[i].extentBox[1] = point4(ply[i].xmax, ply[i].ymax, ply[i].zmax, 1.0);
		ply[i].extentBox[2] = point4(ply[i].xmax, ply[i].ymax, ply[i].zmin, 1.0);
		ply[i].extentBox[3] = point4(ply[i].xmin, ply[i].ymax, ply[i].zmin, 1.0);
		ply[i].extentBox[4] = point4(ply[i].xmin, ply[i].ymin, ply[i].zmax, 1.0);
		ply[i].extentBox[5] = point4(ply[i].xmax, ply[i].ymin, ply[i].zmax, 1.0);
		ply[i].extentBox[6] = point4(ply[i].xmax, ply[i].ymin, ply[i].zmin, 1.0);
		ply[i].extentBox[7] = point4(ply[i].xmin, ply[i].ymin, ply[i].zmin, 1.0);

		extentBox(i);
		infile.close();
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_points) + sizeof(points) + sizeof(boxPoints) + sizeof(linePoints) + sizeof(tex_normals) + sizeof(pointNormals) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tex_points), sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tex_points) + sizeof(points), sizeof(boxPoints), boxPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(boxPoints) + sizeof(linePoints) + sizeof(tex_points) + sizeof(tex_normals), sizeof(pointNormals), pointNormals);
}

void drawWalls(void) {
	// floor
	tex_points[0][0] = point4(0.0, 0.0, 0.0, 1.0);
	tex_points[0][1] = point4(0.0, 0.0, 1.0, 1.0);
	tex_points[0][2] = point4(1.0, 0.0, 1.0, 1.0);
	tex_points[0][3] = point4(1.0, 0.0, 1.0, 1.0);
	tex_points[0][4] = point4(1.0, 0.0, 0.0, 1.0);
	tex_points[0][5] = point4(0.0, 0.0, 0.0, 1.0);

	// left wall
	tex_points[1][0] = point4(0.0, 0.0, 1.0, 1.0);
	tex_points[1][1] = point4(0.0, 1.0, 1.0, 1.0);
	tex_points[1][2] = point4(0.0, 0.0, 0.0, 1.0);
	tex_points[1][3] = point4(0.0, 0.0, 0.0, 1.0);
	tex_points[1][4] = point4(0.0, 1.0, 1.0, 1.0);
	tex_points[1][5] = point4(0.0, 1.0, 0.0, 1.0);

	// right wall
	tex_points[2][0] = point4(0.0, 0.0, 0.0, 1.0);
	tex_points[2][1] = point4(0.0, 1.0, 0.0, 1.0);
	tex_points[2][2] = point4(1.0, 0.0, 0.0, 1.0);
	tex_points[2][3] = point4(1.0, 0.0, 0.0, 1.0);
	tex_points[2][4] = point4(0.0, 1.0, 0.0, 1.0);
	tex_points[2][5] = point4(1.0, 1.0, 0.0, 1.0);

	for (int i = 0; i < 3; i++) {
		tex_coords[i][0] = vec2(0.0, 0.0);
		tex_coords[i][1] = vec2(0.0, 1.0);
		tex_coords[i][2] = vec2(1.0, 0.0);
		tex_coords[i][3] = vec2(1.0, 0.0);
		tex_coords[i][4] = vec2(0.0, 1.0);
		tex_coords[i][5] = vec2(1.0, 1.0);
	}

	for (int j = 0; j < 6; j++) {
		tex_normals[0][j] = vec3(0.0, 1.0, 0.0);
		tex_normals[1][j] = vec3(1.0, 0.0, 0.0);
		tex_normals[2][j] = vec3(0.0, 0.0, 1.0);
	}

	glUniform1f(glGetUniformLocation(program, "mode"), 1.0);
	int temp1 = sizeof(tex_points) + sizeof(points) + sizeof(boxPoints) + sizeof(linePoints);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tex_points), tex_points);
	glBufferSubData(GL_ARRAY_BUFFER, temp1, sizeof(tex_normals), tex_normals);
	glBufferSubData(GL_ARRAY_BUFFER, temp1 + sizeof(pointNormals) + sizeof(tex_normals), sizeof(tex_coords), tex_coords);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	if (drawTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniform1f(glGetUniformLocation(program, "drawTex"), 1.0);
	} else {
		glUniform1f(glGetUniformLocation(program, "drawTex"), 0.0);
	}
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_DEPTH_TEST);
	
	if (drawTexture) {
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1f(glGetUniformLocation(program, "drawTex"), 1.0);
	}
	else {
		glUniform1f(glGetUniformLocation(program, "drawTex"), 0.0);
	}
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 6, 12);
	glDisable(GL_DEPTH_TEST);

	glUniform1f(glGetUniformLocation(program, "drawTex"), 0.0);
	glUniform1f(glGetUniformLocation(program, "mode"), 1.0);
	
}

void myInit(void)
{
	printf("Loading images");

	if (!bmpread("data/stones.bmp", 0, &wall_map)) {
		fprintf(stderr, "%s:error loading stone file\n", "stone.bmp");
		exit(1);
	}

	if (!bmpread("data/grass.bmp", 0, &floor_map)) {
		fprintf(stderr, "%s:error loading grass file\n", "grass.bmp");
		exit(1);
	}

	for (int i = 0; i < 6; i++) {
		if (!bmpread(env_files[i], 0, &env_map[i])) {
			fprintf(stderr, "%s:error loading env file\n", env_files[i]);
			exit(1);
		}
	}

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_map.width, wall_map.height, 0, GL_RGB, GL_UNSIGNED_BYTE, wall_map.rgb_data);
	bmpread_free(&wall_map);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floor_map.width, floor_map.height, 0, GL_RGB, GL_UNSIGNED_BYTE, floor_map.rgb_data);
	bmpread_free(&floor_map);

	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, cubetex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubetex[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, env_map[0].width, env_map[0].height, 0, GL_RGB, GL_UNSIGNED_BYTE, env_map[0].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, env_map[1].width, env_map[1].height, 0, GL_RGB, GL_UNSIGNED_BYTE, env_map[1].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, env_map[2].width, env_map[2].height, 0, GL_RGB, GL_UNSIGNED_BYTE, env_map[2].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, env_map[3].width, env_map[3].height, 0, GL_RGB, GL_UNSIGNED_BYTE, env_map[3].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, env_map[4].width, env_map[4].height, 0, GL_RGB, GL_UNSIGNED_BYTE, env_map[4].rgb_data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, env_map[5].width, env_map[5].height, 0, GL_RGB, GL_UNSIGNED_BYTE, env_map[5].rgb_data);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	bmpread_free(env_map);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	// setup vertex arrays
	int offset = 0;
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
	offset += sizeof(tex_points) + sizeof(points) + sizeof(boxPoints) + sizeof(linePoints);

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
	offset += sizeof(tex_normals) + sizeof(pointNormals);

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));

	glUniform1i(glGetUniformLocation(program, "texture"), 0);
	glUniform1i(glGetUniformLocation(program, "texMap"), 1);

	light_position = point4(50.0, 5000.0, 50.0, 0.0);
	light[0] = light_position.x;
	light[1] = light_position.y;
	light[2] = light_position.z;

	m = Angel::identity();
	m[3][1] = -1 / light[1];
	m[3][3] = 0.0;

	light_ambient = color4(0.2, 0.2, 0.2, 1.0);
	light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
	light_specular = color4(1.0, 0.0, 0.0, 1.0);

    reflect_ambient = color4(0.2, 0.2, 0.2, 1.0);
	reflect_diffuse = color4(0.0, 0.0, 0.0, 1.0);  // default black
	reflect_specular = color4(1.0, 1.0, 1.0, 1.0);

	GLfloat shine = 100.0;

	ambientProduct = light_ambient * reflect_ambient;
	diffuseProduct = light_diffuse * reflect_diffuse;
	specularProduct = light_specular * reflect_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specularProduct);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), shine);

	glUniform1f(glGetUniformLocation(program, "shadow"), 0.0);
	glUniform1f(glGetUniformLocation(program, "drawTex"), 0.0);
	glUniform1f(glGetUniformLocation(program, "drawEnv"), 0.0);

	// sets the default color to clear screen
	glClearColor(1.0, 1.0, 1.0, 1.0); // white background

	// Read all 7 ply files, create 7 Ply objects, send points and pointNormals data to GPU
	readPlyFile();
	line();
}

void drawPlyFile(int index)
{
	// draw solid mesh
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// draw functions should enable then disable the features 
	// that are specifit the themselves
	// the depth is disabled after the draw 
	// in case you need to draw overlays
	glEnable(GL_DEPTH_TEST);
	int offset = sizeof(tex_points)/sizeof(point4);
	for (int i = 0; i < index; i++) {
		offset += sizeof(points[i]) / sizeof(point4);
	}
	glDrawArrays(GL_TRIANGLES, offset, ply[index].faceNum * 3);
    glDisable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------
// this is where the drawing should happen
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the window

	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)60.0, (GLfloat)width / (GLfloat)height, (GLfloat)0.0001, (GLfloat) 100000.0);
	float viewMatrixf[16];
	viewMatrixf[0] = perspectiveMat[0][0]; viewMatrixf[4] = perspectiveMat[0][1];
	viewMatrixf[1] = perspectiveMat[1][0]; viewMatrixf[5] = perspectiveMat[1][1];
	viewMatrixf[2] = perspectiveMat[2][0]; viewMatrixf[6] = perspectiveMat[2][1];
	viewMatrixf[3] = perspectiveMat[3][0]; viewMatrixf[7] = perspectiveMat[3][1];

	viewMatrixf[8] = perspectiveMat[0][2]; viewMatrixf[12] = perspectiveMat[0][3];
	viewMatrixf[9] = perspectiveMat[1][2]; viewMatrixf[13] = perspectiveMat[1][3];
	viewMatrixf[10] = perspectiveMat[2][2]; viewMatrixf[14] = perspectiveMat[2][3];
	viewMatrixf[11] = perspectiveMat[3][2]; viewMatrixf[15] = perspectiveMat[3][3];

	mat4 shadowMat;
	// set up projection matricies
	GLuint viewMatrix = glGetUniformLocationARB(program, "projection_matrix");
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewMatrixf);
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");

	Angel::mat4 modelMat = Angel::identity();
	modelMat = modelMat * Angel::Translate(-10.0f, -200.0f, -1000.0f) * Angel::Scale(500.0f) * Angel::RotateY(-45.0f);
	updateModelMatrix(modelMatrix, modelMat);
	drawWalls();

	if (drawReflection || drawRefraction) {
		glEnable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubetex[0]);
		if (drawReflection) {
			glUniform1f(glGetUniformLocation(program, "drawEnv"), 1.0);
		}
		else {
			glUniform1f(glGetUniformLocation(program, "drawEnv"), 2.0);
		}
	}
	else {
		glUniform1f(glGetUniformLocation(program, "drawEnv"), 0.0);
	}

	glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	// begin hierachy transformations and drawings
	// airplane 0
	reflect_diffuse = color4(1.0, 0.0, 0.0, 1.0);  // red
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = Angel::identity();
	modelMat = modelMat * Angel::Translate(0.0, 0.6 + yMove, 0.0) * Angel::Scale(ply[0].scaleFactor) * Angel::Translate(-(ply[0].xmin + ply[0].xmax) / 2.0f, -(ply[0].ymin + ply[0].ymax) / 2.0f, -sqrt(pow(ply[0].xmax - ply[0].xmin, 2) + pow(ply[0].ymax - ply[0].ymin, 2) + pow(ply[0].zmax - ply[0].zmin, 2)));
	pushMatrix(modelMat);
	updateModelMatrix(modelMatrix, modelMat);

	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4), 16);
		glDisable(GL_DEPTH_TEST);
	}
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_LINES, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4), 2);
	glDisable(GL_DEPTH_TEST);
	modelMat = topMatrix() * Angel::Translate((ply[0].xmin + ply[0].xmax) / 2.0f, 0.0, (ply[0].zmin + ply[0].zmax) / 2.0f) * Angel::RotateY(yRotate[0]) * Angel::Translate(-(ply[0].xmin + ply[0].xmax) / 2.0f, 0.0, -(ply[0].zmin + ply[0].zmax) / 2.0f);
	updateModelMatrix(modelMatrix, modelMat);
	drawPlyFile(0);

	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = Angel::Translate(0.0f, -1.0f, 0.0f) * modelMat*  Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(0);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}
	
	modelMat = topMatrix() * Angel::Translate((ply[0].xmin + ply[0].xmax) / 2.0f, 0.0, (ply[0].zmin + ply[0].zmax) / 2.0f) * Angel::RotateY(yRotate[1]) * Angel::Translate(-(ply[0].xmin + ply[0].xmax) / 2.0f, 0.0, -(ply[0].zmin + ply[0].zmax) / 2.0f);
	updateModelMatrix(modelMatrix, modelMat);
	pushMatrix(modelMat);           // push ctm for arm1 on top
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_LINES, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4) + 2, 2);
	glDrawArrays(GL_LINE_STRIP, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4) + 4, 4);
	glDisable(GL_DEPTH_TEST);
	

	//apple 2
	reflect_diffuse = color4(0.0, 1.0, 0.0, 1.0);  //
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = topMatrix() * Angel::Translate(3500.0, -2400.0, 0.0) * Angel::Scale(10000.0f);
	updateModelMatrix(modelMatrix, modelMat);
	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4) + 16 * 2, 16);
		glDisable(GL_DEPTH_TEST);
	}
	modelMat = modelMat * Angel::Translate(0.0, 0.0, 0.0) * Angel::RotateY(yRotate[2]) * Angel::Translate(0.0, 0.0, 0.0);
	updateModelMatrix(modelMatrix, modelMat);
	drawPlyFile(2);

	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = Angel::Translate(0.0f, -0.8f, 0.0f) * modelMat* Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(2);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}

	//kerolamp 1
	reflect_diffuse = color4(0.0, 0.0, 1.0, 1.0);  // 
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = topMatrix() * Angel::Translate(-2000.0, -1400.0, 600.0) * Angel::Scale(3.0f);
	pushMatrix(modelMat);
	updateModelMatrix(modelMatrix, modelMat);
	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4) + 16, 16);
		glDisable(GL_DEPTH_TEST);
	}
	modelMat = modelMat * Angel::Translate((ply[1].xmin + ply[1].xmax) / 2.0f, 0.0, (ply[1].zmin + ply[1].zmax) / 2.0f) * Angel::RotateY(yRotate[2]) * Angel::Translate(-(ply[1].xmin + ply[1].xmax) / 2.0f, 0.0, -(ply[1].zmin + ply[1].zmax) / 2.0f);
	updateModelMatrix(modelMatrix, modelMat);
    drawPlyFile(1);

	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = Angel::Translate(0.0f, -0.8f, 0.0f) * modelMat * Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(1);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}
	
	modelMat = topMatrix() *Angel::Translate((ply[1].xmin + ply[1].xmax) / 2.0f, 0.0, (ply[1].zmin + ply[1].zmax) / 2.0f) * Angel::RotateY(yRotate[3]) * Angel::Translate(-(ply[1].xmin + ply[1].xmax) / 2.0f, 0.0, -(ply[1].zmin + ply[1].zmax) / 2.0f);
	updateModelMatrix(modelMatrix, modelMat);
	pushMatrix(modelMat);           // push ctm for arm2 on top
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_LINES, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4) + 8, 2);
	glDrawArrays(GL_LINE_STRIP, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4) + 10, 4);
	glDisable(GL_DEPTH_TEST);
	
	//big_spider 
	reflect_diffuse = color4(1.0, 1.0, 0.0, 1.0);  // 
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = topMatrix() * Angel::Translate(-1000.0, -1200.0, -200.0) * Angel::Scale(80.0f);
	updateModelMatrix(modelMatrix, modelMat);
	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4) + 16 * 3, 16);
		glDisable(GL_DEPTH_TEST);
	}
	modelMat = modelMat * Angel::Translate((ply[3].xmin + ply[3].xmax) / 2.0f, 0.0, (ply[3].zmin + ply[3].zmax) / 2.0f) * Angel::RotateY(yRotate[4]) * Angel::Translate(-(ply[3].xmin + ply[3].xmax) / 2.0f, 0.0, -(ply[3].zmin + ply[3].zmax) / 2.0f);
	updateModelMatrix(modelMatrix, modelMat);
	drawPlyFile(3);

	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = Angel::Translate(0.0f, -0.5f, 0.0f) * modelMat *  Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(3);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}
	
	 
	//mug 4
	reflect_diffuse = color4(0.0, 1.0, 1.0, 1.0);  // 
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = topMatrix() * Angel::Translate(800.0, -1100.0, -200.0) * Angel::Scale(3000.0f);
	pushMatrix(modelMat);
	updateModelMatrix(modelMatrix, modelMat);
	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4) + 16 * 4, 16);
		glDisable(GL_DEPTH_TEST);
	}
	modelMat = topMatrix() * Angel::Translate(0.0, 0.0, 0.0) * Angel::RotateY(yRotate[4]) * Angel::Translate(0.0, 0.0, 0.0);
	updateModelMatrix(modelMatrix, modelMat);
	drawPlyFile(4);	

	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = modelMat * Angel::Translate(0.0f, -0.5f, 0.0f) * Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(4);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}

	modelMat = topMatrix() * Angel::Translate(0.0, 0.0, 0.0) * Angel::RotateY(yRotate[5]) * Angel::Translate(0.0, 0.0, 0.0);
	updateModelMatrix(modelMatrix, modelMat);
	pushMatrix(modelMat);           // push ctm for arm3 on top
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_LINES, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4) + 14, 2);
	glDrawArrays(GL_LINE_STRIP, (sizeof(points) + sizeof(boxPoints)) / sizeof(point4) + 16, 4);
	glDisable(GL_DEPTH_TEST);

	//trashcan 5
	reflect_diffuse = color4(1.0, 0.0, 1.0, 1.0);  // 
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = topMatrix() * Angel::Translate(-0.25, -0.20, 0.0) * Angel::Scale(0.002f);
	updateModelMatrix(modelMatrix, modelMat);
	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4) + 16 * 5, 16);
		glDisable(GL_DEPTH_TEST);
	}
	modelMat = modelMat * Angel::Translate(0.0, 0.0, 0.0) * Angel::RotateY(yRotate[6]) * Angel::Translate(0.0, 0.0, 0.0);
	updateModelMatrix(modelMatrix, modelMat);
	drawPlyFile(5);
	
	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = Angel::Translate(0.0, -0.5f, 0.0) * modelMat *  Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(5);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}

	//urn2 6
	reflect_diffuse = color4(0.5, 0.8, 0.5, 1.0);  // 
	diffuseProduct = light_diffuse * reflect_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuseProduct);
	modelMat = topMatrix() * Angel::Translate(0.25, -0.20, 0.0) * Angel::Scale(0.05f);
	updateModelMatrix(modelMatrix, modelMat);
	if (drawBox) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, sizeof(points) / sizeof(point4) + 16 * 6, 16);
		glDisable(GL_DEPTH_TEST);
	}
	modelMat = modelMat * Angel::Translate(0.0, 0.0, 0.0) * Angel::RotateY(yRotate[6]) * Angel::Translate(0.0, 0.0, 0.0);
	updateModelMatrix(modelMatrix, modelMat);
	drawPlyFile(6);

	if (drawShadow) {
		glUniform1f(glGetUniformLocation(program, "mode"), 0.0);
		//glUniform1f(glGetUniformLocation(program, "shadow"), 1.0);
		ambientProduct = light_ambient * color4(0.0, 0.0, 0.0, 1.0);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambientProduct);
		shadowMat = Angel::Translate(0.0f, -0.5f, 0.0f) * modelMat * Angel::Translate(light[0], light[1], light[2]) * m * Angel::Translate(-light[0], -light[1], -light[2]);
		updateModelMatrix(modelMatrix, shadowMat);
		drawPlyFile(6);
		glUniform1f(glGetUniformLocation(program, "mode"), 2.0);
	}
	
	clearStack();

	glFlush(); // force output to graphics hardware
    glutSwapBuffers(); // use this call to double buffer
}

//----------------------------------------------------------------------------

// keyboard handler
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'A':
		drawShadow = !drawShadow;
		break;
	case 'B':
		drawTexture = !drawTexture;
		break;
	case 'C':
		drawReflection = !drawReflection;
		drawRefraction = false;
		break;
	case 'D':
		drawRefraction = !drawRefraction;
		drawReflection = false;
		break;
	case 'e':
		drawBox = !drawBox;
		display();
		break;
	case 's':
		sinusoid = !sinusoid;
		break;
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

void idle() {
	if ((glutGet(GLUT_ELAPSED_TIME) * 30) % 1 == 0) { // set frame rate 30 FPS
		yRotate[0] += 0.2f;
		yRotate[1] -= 0.4f;
		yRotate[2] += 1.2f;
		yRotate[3] += 0.8f;
		yRotate[4] += 1.2f;
		yRotate[5] -= 1.2f;
		yRotate[6] += 2.0f;
		if (sinusoid) {
			if (up == true && down == false) {
				yMove += moveAmount;
				moveCount++;
				if (moveCount == 100) {
					up = false;
				}
			}
			else if (up == false && down == false) {
				yMove -= moveAmount;
				moveCount--;
				if (moveCount == 0) {
					down = true;
				}
			}
			else if (up == false && down == true) {
				yMove -= moveAmount;
				moveCount++;
				if (moveCount == 100) {
					up = true;
				}
			}
			else {
				yMove += moveAmount;
				moveCount--;
				if (moveCount == 0) {
					down = false;
				}
			}
		}
	}
	
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// entry point
int main(int argc, char **argv)
{
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 800);
	width = 1000;
	height = 800;

	// create window
	// opengl can be incorperated into other packages like wxwidgets, fltoolkit, etc.
	glutCreateWindow("HW4");

	// init glew
	glewInit();

	// read files, set up shader and GPU buffer
	myInit();

	// assign handlers
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	// enter the drawing loop
	// frame rate can be controlled with 
	glutMainLoop();
	return 0;
}
