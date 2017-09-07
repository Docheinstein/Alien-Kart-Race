#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

class MatrixUtil {
public:
	struct MatrixCoordinate {
		int col;
		int row;
	};
	static void printMatrix(int **matrix, int rowCount, int colCount);
	static void deleteMatrix(int **&matrix, int rowCount);
	static void findValuesCoordinates(
		int valuesToFind[], // [in]
		MatrixCoordinate valuesCoordinates[] // [out]
	);
};

#endif // MATRIX_UTIL_H
