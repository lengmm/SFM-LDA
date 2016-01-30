#include "Test.h"
#include<highgui/highgui.hpp>
#include<iostream>
#include<ctime>

#include "Config.h"
#include "OpticalFlow.h"
#include "SocialForceMap.h"
#include "VisualWord.h"
#include "WordList.h"
#include "MatrixIO.h"
extern Config cf;

Test::Test()
{
}

void Test::test(){
	//将所有的训练图片都加到imgStr,每个场景用空字符串隔开
	std::vector<std::string> imgStr;
	for (auto it = cf.testFiles.begin(); it != cf.testFiles.end(); it++){
		getImageStrings(*it, imgStr);
		imgStr.push_back("");
	}
	cv::Mat img, preImg;
	std::vector<std::string>::iterator it = imgStr.begin();
	img = cv::imread(*it);
	it++;
	preImg = img;
	std::vector<OpticalFlow>  flows;
	cv::Mat v;

	//计算第二帧到最后一帧中网格处的光流向量，并保存在flows中
	while (it != imgStr.cend() && (*it != "")){
		img = cv::imread(*it);
		OpticalFlow flow(preImg, img);
		flows.push_back(flow);
		preImg = img;
		it++;
	}

	//利用光流向量将该场景下的所有帧的社会力图计算出来
	socialForceMap sfm(flows);

	//用sfm来建立场景，clip,word等进行分块
	Scene scene(cf.trainR);


	std::vector<VisualWord>  selectwords;//场景中每个Clip随机挑出32个VisualWord用来单词表
	int clipAmount = 0;
	bool wordValidity = true;
	while (clipAmount<scene.getClipCount()){
		int wordAmount = 0;//用来保存当前帧中以及选中的词数
		Clip c = scene.getClip(clipAmount);
		while (wordAmount < cf.selected){
			int number;
			srand(unsigned(time(0)));
			number = rand() % (c.getWordCount());
			if (c.getVisualWord(number).isValidity())
			{
				selectwords.push_back(c.getVisualWord(number));
				wordAmount++;
			}
		}
		clipAmount++;
	}
	//用选定的单词来构造单词表,selectwords中没32个VisualWord是来自一篇文档的
	std::cout << selectwords.end() - selectwords.begin();
	WordList vacablulary(selectwords);

	//根据单词表统计各个document中单词出现的频率
	vector<vector<int> > frequences;//保存一个scene中所有Clip的词频统计数据
	cv::Mat frequence(scene.getClipCount(), scene.getClip(0).getWordCount(), CV_8U, cv::Scalar(0));
	for (int i = 0; i < scene.getClipCount(); i++)
	{
		vector<int> freq;
		wordCount4List(scene.getClip(i), freq, vacablulary);
		frequences.push_back(freq);
		for (auto it = freq.begin(); it != freq.end(); it++)
			frequence.at<int>(i, it - freq.begin()) = *it;
	}
	
	saveMatrixData(frequence,"frequence.txt");

	//调用LDA去预测
	//一篇文档的可能性是每个单词最大可能性（利用已经求得的主题模型可以得到）的log的累加，然后和一个阈值进行比较，低于这个阈值则是异常
	//把每篇文档和主题的相似度算出来，用最大的那个和阈值进行比较

}
Test::~Test()
{
}
