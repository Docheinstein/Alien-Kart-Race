#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "matrixutil.h"
#include "log.h"

using namespace std;

class FileUtil {
public:
	template<typename T>
	static void getMatrixSize(const char *fileName, int &rowCount, int &colCount) {
		ifstream inFile(fileName);
		if (!inFile)
			return;

		int rows = -1;
		int maximumColCount = 0;
		string str;

		do {
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
				w("Found a row with less occurrences than the others. Found: ", occurrences, ", expected: ", maximumColCount);
			}
		} while (inFile);

		rowCount = rows;
		colCount = maximumColCount;
	}

	template<typename T>
	static void loadMatrixKnowningSize(const char *fileName, T **&matrix, int rowCount, int colCount) {
	    std::ifstream inFile(fileName);
		if (!inFile)
			return;
		int r = -1;
		int c = colCount;

		T anObject;

		do {
			// End of line reached
			if (c == colCount) {
				c = 0;
				r++;
			}
			if (r < rowCount) {
				inFile >> anObject;
				matrix[r][c] = anObject;
				c++;
			}
		} while (inFile && r < rowCount);
	}

	template<typename T>
	static void loadMatrix(const char *fileName, T **&matrix, int &rowCount, int &colCount) {
		getMatrixSize<T>(fileName, rowCount, colCount);
		MatrixUtil::initMatrix<T>(matrix, rowCount, colCount);
		loadMatrixKnowningSize(fileName, matrix, rowCount, colCount);
	}
};

#endif // FILE_UTIL_H
