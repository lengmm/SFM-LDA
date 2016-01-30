#include "matrixIO.h"
void saveMatrixData(const Mat& data, const char* dataFile)
{
	FILE* file = fopen(dataFile, "w");
	int row = data.rows;
	int col = data.cols;
	fprintf(file, "%d %d\n", row, col);
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
			fprintf(file, "%8f ", data.at<float>(i, j));
		fprintf(file, "\n");
	}
	fclose(file);
}


Mat loadMatrixData(const char* dataFile)
{
	FILE* file = fopen(dataFile, "r");
	if(!file)
	{
		printf("file \"%s\" not exist!!!\n", dataFile);
		exit(1);
	}
	int row, col;
	fscanf(file, "%d %d", &row, &col);
	Mat data(row, col, CV_32FC1);
	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++)
			fscanf(file, "%8f ", &data.at<float>(i, j));
	fclose(file);
	return data;
}

//Mat loadMatrixNoHead(const char* dataFile)
//{
//	FILE* file = fopen(dataFile, "r");
//	if(!file)
//	{
//		printf("file \"%s\" not exist!!!\n", dataFile);
//		exit(1);
//	}
//	int row, col;
//	row = countLines(dataFile);
//	col = countWords(dataFile);
//	Mat data(row, col, CV_32FC1);
//	for(int i = 0; i < row; i++)
//		for(int j = 0; j < col; j++)
//			fscanf(file, "%8f ", &data.at<float>(i, j));
//	fclose(file);
//	return data;
//}