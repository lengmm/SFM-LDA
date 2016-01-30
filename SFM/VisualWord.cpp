#include "VisualWord.h"
#include "Config.h"
#include <vector>
#include<iostream>
#include<cmath>

extern Config cf;

VisualWord::VisualWord(int r, int c, int b) :rows(r), cols(c), bins(b)
{
}

VisualWord::VisualWord(int r, int c, int b, int x1, int y1,std::vector<cv::Mat>& frames) :rows(r), cols(c), bins(b), x(x1), y(y1)
{
	//std::cout << "x" << "= " << x << std::endl;
	//std::cout << "y" << "= " << y << std::endl;
	//根据Clip以及给出的word在其中的坐标位置，计算出该word所包含的Mat
	for (auto it = frames.begin(); it != frames.end(); it++){
		cv::Mat block(cf.cellrows, cf.cellcols,CV_32F);//word中在一帧中的Mat
		for (int i = 0; i < cf.cellrows; i++)
			for (int j = 0; j < cf.cellcols; j++)
				block.at<float>(i, j) = it->at<float>(x*cf.cellrows + i, y*cf.cellcols + j);
		word.push_back(block);
	}
	
}

//如果一个单词中有一小块全部为0.则这个word就是无效的
bool VisualWord::isValidity(){
	int zeroblock = 0;
	int zeroCount=0;
	for (auto it = word.begin(); it != word.end(); it++)
	{
		for (int row = 0; row < it->rows; row++){
			for (int col = 0; col < it->cols; col++)
			{
				if (it->at<float>(row, col) == 0)
					zeroCount++;
			}
		}
		if (zeroCount == cf.cellrows*cf.cellcols)
			zeroblock++;
	}
	if (zeroblock == 1)
	{
		return false;
		std::cout << "this visualword is not valid." << std::endl;
	}
	else 
		return true;
}


void  VisualWord::toVector(std::vector<float>& wordVector){
	for (auto it = word.begin(); it != word.end(); it++)
	{
		for (int row = 0; row < it->rows; row++){
			for (int col = 0; col < it->cols; col++)
			{
				wordVector.push_back(it->at<float>(row, col));
			}
		}
	}
}


VisualWord::VisualWord(vector<float>& w){
	for (int bin = 0; bin < cf.cellbins; bin++){
		cv::Mat block(cf.cellrows, cf.cellcols, CV_32F, cv::Scalar(0));
		for (int row = 0; row < cf.cellrows; row++){
			for (int col = 0; col < cf.cellcols; col++)
			{
				block.at<float>(row, col) = w[bin*cf.cellrows*cf.cellcols+row*cf.cellrows+col];
			}
		}
		word.push_back(block);
	}
}

float similarity(VisualWord& w1, VisualWord& w2){
	float dist = 0;
	cv::Mat tmp;
	for (int b = 0; b < cf.cellbins; b++){
		absdiff(w1.getWord()[b], w2.getWord()[b], tmp);
	}

	for (int r = 0; r < cf.cellrows;r++)
		for (int c = 0; c < cf.cellcols; c++){
				dist += tmp.at<float>(r, c);
			}
	return dist;
}

VisualWord::~VisualWord()
{
}

