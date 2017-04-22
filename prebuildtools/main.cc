/*
  	TO COMPILE
 	g++ -c ../src/util/fileutil.h
 	g++ -c main.cc
	g++ main.o ../src/util/fileutil.o -o map_zoomer
 */
#include <iostream>
#include <fstream>
#include <string>
#include "../src/util/fileutil.h"

using namespace std;

const int MAX_COL = 1024;
const int MAX_ROW = 1024;

void printMatrix(ostream &out, int matrix[][MAX_COL],
				 int colCount, int rowCount) {
	for (int r = 0; r < rowCount; r++) {
		for (int c = 0; c < colCount; c++) {
			out << matrix[r][c] << " ";
		}
		out << endl;
	}
}

int main(int argc, char *argv[]) {

	char *inputFileName = argv[1];
	char *outputFileName = argv[2];
	int zoomFactor = atoi(argv[3]);

	int zoomedMatrixMap[MAX_COL][MAX_ROW];

	ifstream inFile(inputFileName);
	if (!inFile)
		return -1;


	cout << "Zooming map of file: : " << inputFileName << endl;
	int fileColCount = FileUtil::getIntCountInFirstRow(inputFileName);
	cout << "Column count: " << fileColCount << endl;

	int r = 0;
	int c = 0;
	int readValue;
	int validRowCount = 0;

	while (inFile) {

		inFile >> readValue;

		if (!inFile)
			break;

		validRowCount++;

		for (int cz = 0; cz < zoomFactor; cz++)
			zoomedMatrixMap[r][cz] = readValue;

		cout << zoomedMatrixMap[r][0] << " | ";

		for (c = 1; c < fileColCount; c++) {
			inFile >> readValue;
			for (int cz = 0; cz < zoomFactor; cz++)
				zoomedMatrixMap[r * zoomFactor][c * zoomFactor + cz]
					= readValue;
			cout << zoomedMatrixMap[r * zoomFactor][c * zoomFactor] << " | ";
		}

		// Replicate rows
		for (int rz = 1; rz < zoomFactor; rz++) {
			for (int cz = 0; cz < fileColCount * zoomFactor; cz++)
				zoomedMatrixMap[r * zoomFactor + rz][cz] =
					zoomedMatrixMap[r * zoomFactor][cz];
		}

		cout << endl;
		r++;
	}

	cout << "Zoomed matrix output" << endl;
	cout << "--------------------" << endl;
	printMatrix(cout, zoomedMatrixMap,
				fileColCount * zoomFactor,
				validRowCount * zoomFactor);

	ofstream outFile(outputFileName);
	if (outFile)
		printMatrix(outFile, zoomedMatrixMap,
					fileColCount * zoomFactor,
					validRowCount * zoomFactor);
}
