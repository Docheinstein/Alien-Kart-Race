#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H

/**
 * Provides utility method for handle the matrix, which are annoying to handle manually.
 */
class MatrixUtil {
public:

	/**
	 * Deallocates the given matrix.
	 * @param matrix   the matrix.
	 * @param rowCount the row count of the matrix.
	 */
	template<typename T>
	static void deleteMatrix(T **&matrix, int rowCount);

	/**
	 * Initializes a new matrix with the given dimensio.
	 * @param matrix   the matrix.
	 * @param rowCount the row count.
	 * @param colCount the column count.
	 */
	template<typename T>
	static void initMatrix(T **&matrix, int rowCount, int colCount);

};

#include "matrix.tpp"

#endif // MATRIX_UTIL_H
