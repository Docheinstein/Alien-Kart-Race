#ifndef FILE_UTIL_H
#define FILE_UTIL_H

class FileUtil {
public:
	static void getIntMatrixSize(const char *fileName, int &rowCount, int &colCount);
	static void loadIntMatrix(const char *fileName, int **&matrix,
							  int rowCount, int colCount);
};

#endif // FILE_UTIL_H
