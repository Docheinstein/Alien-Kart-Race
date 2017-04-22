#ifndef FILE_UTIL_H
#define FILE_UTIL_H

class FileUtil {
public:
	static int getIntCountInFirstRow(const char *fileName);
	static int getConsecutiveRowCount(const char *fileName);
	static void loadIntMatrix(const char *fileName, int **&matrix,
							  int rowCount, int colCount);
};

#endif // FILE_UTIL_H
