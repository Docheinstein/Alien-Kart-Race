#include <iostream>
#include <fstream>
#include <string>
#include "fileutil.h"

using namespace std;

int FileUtil::getIntCountInFirstRow(const char *fileName) {
	ifstream inFile(fileName);
	if (!inFile)
		return 0;

	string str;
	getline(inFile, str);

	int whitespaces = 0;
	int strLen = str.length();
	for (unsigned int i = 0; i < str.length(); i++)
		if (str[i] == ' ')
		whitespaces++;

	return strLen - whitespaces;
}

int FileUtil::getConsecutiveRowCount(const char *fileName) {
	ifstream inFile(fileName);
	if (!inFile)
		return 0;

	int rowCount = 0;
	string str;

	do {
		getline(inFile, str);
		rowCount++;
	} while (str.length() > 0);

	return rowCount - 1;
}

void FileUtil::loadIntMatrix(const char *fileName,
							 int **&matrix,
							 int rowCount,
							 int colCount) {
	ifstream inFile(fileName);
	if (!inFile)
		return;

	matrix = new int*[rowCount];

	int r = -1;
	int c = colCount;

	do {
		// End of line reached
		if (c == colCount) {
			c = 0;
			r++;
			if (r < rowCount)
				matrix[r] = new int[colCount];
		}

		inFile >> matrix[r][c];
		c++;

	} while (inFile && r < rowCount);
}
