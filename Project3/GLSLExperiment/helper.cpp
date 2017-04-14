#include "helper.h"
#include <iostream>
#include <string>

CTMStack *top = NULL;

void initStack(mat4 matrix) {
	CTMStack *temp = (CTMStack*)malloc(sizeof(CTMStack));
	temp->matrix = matrix;
	temp->next = top;
	top = temp;
}

void pushMatrix() { // copy top matrix and add it above previous top
	CTMStack *temp = (CTMStack*)malloc(sizeof(CTMStack));
	temp->matrix = top->matrix;
	temp->next = top;
	top = temp;
}

void pushMatrix(mat4 matrix) { // push matrix on top of stack
	CTMStack *temp = (CTMStack*)malloc(sizeof(CTMStack));
	temp->matrix = matrix;
	temp->next = top;
	top = temp;
}

void popMatrix() { // pop the current top
	CTMStack *temp;
	if (top == NULL) return;
	temp = top;
	top = top->next;
	free(temp);
}

mat4 topMatrix() { // get the matrix at the top of stack
	if (top == NULL) return NULL;
	return top->matrix;
}

void clearStack() {
	while (top != NULL) {
		popMatrix();
	}
}

void updateModelMatrix(int modelMatrix, mat4 modelMat) {
	//top->matrix = modelMat;

	float modelMatrixf[16];
	modelMatrixf[0] = modelMat[0][0]; modelMatrixf[4] = modelMat[0][1];
	modelMatrixf[1] = modelMat[1][0]; modelMatrixf[5] = modelMat[1][1];
	modelMatrixf[2] = modelMat[2][0]; modelMatrixf[6] = modelMat[2][1];
	modelMatrixf[3] = modelMat[3][0]; modelMatrixf[7] = modelMat[3][1];

	modelMatrixf[8] = modelMat[0][2]; modelMatrixf[12] = modelMat[0][3];
	modelMatrixf[9] = modelMat[1][2]; modelMatrixf[13] = modelMat[1][3];
	modelMatrixf[10] = modelMat[2][2]; modelMatrixf[14] = modelMat[2][3];
	modelMatrixf[11] = modelMat[3][2]; modelMatrixf[15] = modelMat[3][3];

	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, modelMatrixf);
}