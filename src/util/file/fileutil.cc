#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "fileutil.h"
#include "log.h"

using namespace std;

void FileUtil::getIntMatrixSize(const char *fileName,
	 							int &rowCount, int &colCount) {
	ifstream inFile(fileName);
	if (!inFile)
		return;

	int rows = -1;
	int maximumColCount = 0;
	string str;

	do {
		getline(inFile, str);
		rows++;

		// Check for ints in this row
		std::stringstream strStream(str);
		int ints = -1;
		int anInt;
		do {
	  		strStream >> anInt;
			ints++;
		}
		while (strStream);

		if (ints > maximumColCount) {
			maximumColCount = ints;
		}
		else if (ints < maximumColCount) {
			w("Found a row with less integers than the others. Found: ", ints, ", expected: ", maximumColCount);
		}
	} while (inFile);

	rowCount = rows;
	colCount = maximumColCount;
}
