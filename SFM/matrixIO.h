#ifndef __LIAN_MATRIXIO__
#define __LIAN_MATRIXIO__

#include<core/core.hpp>
using cv::Mat;

void saveMatrixData(const cv::Mat& data, const char* dataFile);
cv::Mat loadMatrixData(const char* dataFile);
cv::Mat loadMatrixNoHead(const char* dataFile);

#endif