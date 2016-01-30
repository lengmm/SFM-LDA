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
	vector<cv::Mat> frames;//frames中存放着十帧
	for (auto it = paths.begin(); it != paths.end(); it++){
		cv::Mat frame = loadMatrixData(it->c_str());//这里要从txt文件中读入内容放在Mat中
		frames.push_back(frame);
	}
	wordCount = (cf.height / 5)*(cf.width / 5);
	for (int i = 0; i <(cf.height/5)*(cf.width /5); i++){
		//构造VisualWord时给出所在的clip，以及VisualWord的序号
		int x, y;
		x = i / (cf.width / cf.cellcols);//行
		y = i - x*(cf.width / cf.cellcols);//列
		words.push_back(VisualWord(cf.cellrows, cf.cellcols, cf.cellbins, x, y, frames));
	}
		
}
Clip::~Clip()
{
}