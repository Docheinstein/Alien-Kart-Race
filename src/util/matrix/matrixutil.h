#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

class MatrixUtil {
public:
	struct MatrixCoordinate {
		int col;
		int row;
	};

	template<typename T>
	static void deleteMatrix(T **&matrix, int rowCount) {
		if (matrix != NULL) {
			for (int r = 0; r < rowCount; r++)
				delete [] matrix[r];
			delete [] matrix;
			matrix = NULL;
		}
	}

	template<typename T>
	static void initMatrix(	T **&matrix, int rowCount, int colCount) {
		matrix = new T*[rowCount];
		for (int r = 0; r < rowCount; r++)
			matrix[r] = new T[colCount];
	}

};

#endif // MATRIX_UTIL_H
