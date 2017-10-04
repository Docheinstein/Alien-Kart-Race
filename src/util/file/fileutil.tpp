#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "matrixutil.h"
#include "logger.h"

template<typename T>
void FileUtil::getMatrixSize(const char *fileName, int &rowCount, int &colCount) {
	std::ifstream inFile(fileName);
	if (!inFile)
		return;

	int rows = -1;
	int maximumColCount = 0;
	std::string str;

	do {
		// Read the next line
		getline(inFile, str);
		rows++;

		// Check for occurrences of the specified type in this row
		std::stringstream strStream(str);
		int occurrences = -1;
		T anObject;
		do {
	  		strStream >> anObject;
			occurrences++;
		}
		while (strStream);

		if (occurrences > maximumColCount) {
			maximumColCount = occurrences;
		}
		else if (occurrences < maximumColCount) {
			warn("{FileUtil} ", "Found a row with less occurrences than the others. (found: ",
			  occurrences, ", expected: ", maximumColCount, ")");
		}
	} while (inFile);

	rowCount = rows;
	colCount = maximumColCount;
}

template<typename FileT, typename FuncOwner>
void FileUtil::loadStructureFromFileKnowningSize(
									const char *fileName,
									int knownRowCount,
									int knownColCount,
									FuncOwner *instance,
									void (FuncOwner::*fillFunc)(FileT readValue, int row, int col),
									void * structure,
									void (*defaultFillFunc)
										(FileT readValue, void *structure, int row, int col)) {

	std::ifstream inFile(fileName);
	if (!inFile)
		return;

	int r = -1;
	int c = knownColCount;

	FileT anObject;

	do {
		// End of line reached
		if (c == knownColCount) {
			c = 0;
			r++;

		}
		// There is still row to read
		if (r < knownRowCount) {
			// Read the next object
			inFile >> anObject;

			// Call default fill function if provided
			if (defaultFillFunc != NULL)
				defaultFillFunc(anObject, structure, r, c);
			else // Call the given callback otherwise
				(instance->*fillFunc)(anObject, r, c);

			c++;
		}
	} while (inFile && r < knownRowCount);
}

// Matrix helpers
template<typename T>
void FileUtil::defaultMatrixFillFunction(T readValue, void* matrix, int row, int col) {
	static_cast<T**>(matrix)[row][col] = readValue;
}

template<typename T>
void FileUtil::loadMatrixFromFileKnowningSize(
									const char *filename, T **matrix,
									int knownRowCount, int knownColCount) {
	loadStructureFromFileKnowningSize<T, FileUtil>(
		filename,
		knownRowCount, knownColCount,
		NULL, NULL,
		matrix, defaultMatrixFillFunction<T>
	);
}

template<typename T>
void FileUtil::loadMatrix(const char *fileName, T **&matrix, int &rowCount, int &colCount) {
	getMatrixSize<T>(fileName, rowCount, colCount);
	MatrixUtil::initMatrix<T>(matrix, rowCount, colCount);
	loadMatrixFromFileKnowningSize<T>(fileName, matrix, rowCount, colCount);
}


// Array helpers
template<typename T>
void FileUtil::defaultArrayFillFunction(T readValue, void* array, int row, int col) {
	static_cast<T*>(array)[col] = readValue;
}

template<typename T>
void FileUtil::loadArrayFromFileKnowningSize(
									const char *filename, T *array,
									int knownRowCount, int knownColCount) {
	loadStructureFromFileKnowningSize<T, FileUtil>(
		filename,
		knownRowCount, knownColCount,
		NULL, NULL,
		array, defaultArrayFillFunction<T>
	);
}

template<typename T>
void FileUtil::loadArray(const char *fileName, T *&array, int &count) {
	int rowCount, colCount;
	getMatrixSize<T>(fileName, rowCount, colCount);
	count = colCount * rowCount;
	array = new T[count];
	loadArrayFromFileKnowningSize<T>(fileName, array, rowCount, colCount);
}