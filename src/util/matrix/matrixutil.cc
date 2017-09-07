#include <iostream>
#include <fstream>
#include "matrixutil.h"

using namespace std;

void MatrixUtil::printMatrix(int **matrix, int rowCount, int colCount) {
	for (int r = 0; r < rowCount; r++) {
		for (int c = 0; c < colCount; c++) {
			cout << matrix[r][c] << " ";
		}
		cout << endl;
	}
}

void MatrixUtil::deleteMatrix(int **&matrix, int rowCount) {
	if (matrix != NULL) {
		for (int r = 0; r < rowCount; r++)
			delete [] matrix[r];
		delete [] matrix;
		matrix = NULL;
	}
}
