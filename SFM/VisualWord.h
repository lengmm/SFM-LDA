#ifndef VISUALWORD_H
#define VISUALWORD_H
#include<vector>
#include<core/core.hpp>
class VisualWord{
	friend float similarity(VisualWord&, VisualWord&);
public:
	VisualWord(int ,int ,int );
	VisualWord(int, int, int, int, int, std::vector<cv::Mat>&);
	~VisualWord();
	std::vector<cv::Mat> getWord(){ return word; }
	bool isValidity();
	void toVector(std::vector<float>&);
	VisualWord(std::vector<float>&);
private:
	int rows;
	int cols;
	int bins;
	int wordNm;//word的在这个Clip中的序号
	int x;//该word在Clip中的位置（0-cf.width/5）行
	int y;//该word在Clip中的位置（0-cf.height/5）列
	std::vector<cv::Mat> word;
};
float similarity(VisualWord&, VisualWord&);

#endif