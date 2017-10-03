template<typename T>
void MatrixUtil::deleteMatrix(T **&matrix, int rowCount) {
	if (matrix != NULL) {
		for (int r = 0; r < rowCount; r++)
			delete [] matrix[r];
		delete [] matrix;
		matrix = NULL;
	}
}

template<typename T>
void MatrixUtil::initMatrix(T **&matrix, int rowCount, int colCount) {
	matrix = new T*[rowCount];
	for (int r = 0; r < rowCount; r++)
		matrix[r] = new T[colCount];
}