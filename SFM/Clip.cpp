#include "Clip.h"
#include "Config.h"
#include "matrixIO.h"
#include<imgproc/imgproc.hpp>
#include<highgui/highgui.hpp>
#include<video/tracking.hpp>
extern Config cf;

Clip::Clip()
{
}


Clip::Clip(int nm, vector<string> paths, int r, int c, int b) : clipNm(nm),rows(r), cols(c), bins(b){
	vector<cv::Mat> frames;//frames�д����ʮ֡
	for (auto it = paths.begin(); it != paths.end(); it++){
		cv::Mat frame = loadMatrixData(it->c_str());//����Ҫ��txt�ļ��ж������ݷ���Mat��
		frames.push_back(frame);
	}
	wordCount = (cf.height / 5)*(cf.width / 5);
	for (int i = 0; i <(cf.height/5)*(cf.width /5); i++){
		//����VisualWordʱ�������ڵ�clip���Լ�VisualWord�����
		int x, y;
		x = i / (cf.width / cf.cellcols);//��
		y = i - x*(cf.width / cf.cellcols);//��
		words.push_back(VisualWord(cf.cellrows, cf.cellcols, cf.cellbins, x, y, frames));
	}
		
}
Clip::~Clip()
{
}