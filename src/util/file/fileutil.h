#ifndef FILE_UTIL_H
#define FILE_UTIL_H

using namespace std;

/**
 * Provides utility functions for obtain information
 * or data from a a raw txt file
 */
class FileUtil {
public:
	/**
	 * Returns the size of the matrix loaded at the given filename.
	 * For the considered file, the space are considered as columns
	 * seperator and the newline is considered as rows seperator.
	 * @param fileName 		the filename of the file.
	 * @param[out] rowCount the row count of the file's matrix.
	 * @param[out] colCount the column count of the file's matrix.
	 */
	template<typename T>
	static void getMatrixSize(const char *fileName, int &rowCount, int &colCount);

	/**
	 * Loads any type of structure from a matrix in a file with the given col and row count.
	 * This methods offers two types of used:
	 * The first is pass a member function pointer and an instance; in this case the function will
	 * be called for each value loaded from the matrix and the function is responsible for
	 * fill the structure by itself. In that case there is no need to pass the structure to this
	 * function since the function owner is responsible for fill the structure.
	 * The second is pass a structure and eventually a function pointer to the fill function:
	 * in that case the structure is filled by the function which will receive the structure
	 * to fill as parameter.
	 *
	 * @param fileName      	the file name of the file to load.
	 * @param knownRowCount 	the row count of the matrix to load (must be valid).
	 * @param knownColCount 	the column count of the matrix to load (must be valid).
	 * @param instance      	the instance that owns the fill function pointer.
	 * @param fillFunc      	the function that is called for each read value.
	 * @param[out] structure    the structure that will be filled.
	 * @param defaultFillFunc   default fill function for the given structure.
	 */
	template<typename FileT, typename FuncOwner>
	static void loadStructureFromFileKnowningSize(
		const char *fileName,
		int knownRowCount,
		int knownColCount,
		FuncOwner *instance,
		void (FuncOwner::*fillFunc)(FileT readValue, int row, int col),
		void * structure = NULL,
		void (*defaultFillFunc)
			(FileT readValue, void *structure, int row, int col) = NULL);

	// --------------
	// MATRIX HELPERS
	// --------------
	//
	/**
	 * Sets the read value as matrix's value for the given row and column.
	 * @param readValue the value read from the file.
	 * @param matrix    the matrix that will be filled.
	 * @param row       the row of the read value.
	 * @param col       the column of the read value.
	 */
	template<typename T>
	static void defaultMatrixFillFunction(T readValue, void* matrix, int row, int col);

	/**
	 * Loads the given matrix from a file using the default fill function.
	 * Each file's value will be put at the respective row and col index in the matrix.
	 * @param filename      the filename of the file to load.
	 * @param matrix        the matrix that will be filled.
	 * @param knownRowCount the row count of both the file's matrix and the matrix structure.
	 * @param knownColCount the column count of both the file's matrix and the matrix structure.
	 */
	template<typename T>
	static void loadMatrixFromFileKnowningSize(
										const char *filename, T **matrix,
										int knownRowCount, int knownColCount);

	/**
	 * Loads the given matrix from a file using the default fill function.
	 * Each file's value will be put at the respective row and col index in the matrix.
	 * @param filename      the filename of the file to load.
	 * @param matrix        the matrix that will be filled.
	 * @param[out] rowCount the row count of both the file's matrix and the matrix structure.
	 * @param[out] colCount the column count of both the file's matrix and the matrix structure.
	 */
	template<typename T>
	static void loadMatrix(const char *fileName, T **&matrix, int &rowCount, int &colCount);

	// -------------
	// ARRAY HELPERS
	// -------------
	//
	/**
	 * Sets the read value as array's value for the given row and column.
	 * The default implementation just ignore the row count and write to
	 * the array index equals to the file's column.
	 * @param readValue the value read from the file.
	 * @param array    the array that will be filled.
	 * @param row       the row of the read value.
	 * @param col       the column of the read value.
	 */
	template<typename T>
	static void defaultArrayFillFunction(T readValue, void* array, int row, int col);

	/**
	 * Loads the given array from a file using the default fill function.
	 * Each file's value will be put at the array index equals to the file's column,
	 * overwriting any value previosuly written in that position (so in case a the file
	 * contains more row, thhe given array will be filled with the last row).
	 * @see #loadMatrixFromFileKnowningSize for load multidimensional files.
	 * @param filename      the filename of the file to load.
	 * @param array        the matrix that will be filled.
	 * @param knownRowCount the row count of both the file's matrix and the matrix structure.
	 * @param knownColCount the column count of both the file's matrix and the matrix structure.
	 */
	template<typename T>
	static void loadArrayFromFileKnowningSize(
										const char *filename, T *array,
										int knownRowCount, int knownColCount);

	/**
	 * Loads the given array from a file using the default fill function.
	 * Each file's value will be put at the array index equals to the file's column.
	 * @see #loadMatrix for load multidimensional files.
	 * @param filename    the filename of the file to load.
	 * @param array       the matrix that will be filled.
	 * @param[ouŧ] count  the column count of both the file's matrix and the matrix structure.
	 */
	template<typename T>
	static void loadArray(const char *fileName, T *&array, int &count);
};

#include "fileutil.tpp"

#endif // FILE_UTIL_H