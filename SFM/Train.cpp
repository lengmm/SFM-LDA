#include "Train.h"

#include<highgui/highgui.hpp>
#include<iostream>
#include<ctime>

#include "Config.h"
#include "OpticalFlow.h"
#include "SocialForceMap.h"
#include "VisualWord.h"
#include "Scene.h"
#include "WordList.h"
#include "lda.h"
#include "MatrixIO.h"
extern Config cf;
Train::Train()
{
}

void Train::train(){
	//将所有的训练图片都加到imgStr,每个场景用空字符串隔开
	//std::vector<std::string> imgStr;
	//for (auto it = cf.trainFiles.begin(); it != cf.trainFiles.end(); it++){
	//	getImageStrings(*it, imgStr);
	//	imgStr.push_back("");
	//}
	//cv::Mat img, preImg;
	//std::vector<std::string>::iterator it = imgStr.begin();
	//img = cv::imread(*it);
	//it++;
	//preImg = img;
	//std::vector<OpticalFlow>  flows;
	//cv::Mat v;
	//std::cout << "开始计算光流向量" << std::endl;
	////imgStr保存了所有场景的帧的路径信息，这里先只考虑一个场景，计算第二帧到最后一帧中网格处的光流向量，并保存在flows中
	//int t = 0;
	//while (it != imgStr.cend() && (*it != "")){
	//	img = cv::imread(*it);
	//	OpticalFlow flow(preImg, img);
	//	std::cout << "opticalFlow"<<t<< std::endl;
	//	flows.push_back(flow);
	//	preImg = img;
	//	it++;
	//	t++;
	//}
	//std::cout << "开始计算社会力" << std::endl;
	////利用光流向量将该场景下的所有帧的社会力图计算出来
	//socialForceMap sfm(flows);
	//std::cout << "开始构建场景" << std::endl;
	//用sfm来建立场景，clip,word等进行分块
	Scene scene(cf.trainR);

	std::cout << "开始选词" << std::endl;
	std::vector<VisualWord>  selectwords;//场景中每个Clip随机挑出32个VisualWord用来单词表
	int clipAmount = 0;
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
	std::cout << "开始构建单词表" << std::endl;
	//用选定的单词来构造单词表,selectwords中没32个VisualWord是来自一篇文档的
	//std::cout << selectwords.end() - selectwords.begin();
	WordList vacablulary(selectwords);

	std::cout << "开始统计词频" << std::endl;
	//根据单词表统计各个document(clip)中单词出现的频率
	vector<vector<int> > frequences;//保存一个scene中所有Clip的词频统计数据
	/*cv::Mat frequence(scene.getClipCount(), cf.centerCount, CV_8UC1);*/
	for (int i = 0; i < scene.getClipCount(); i++)
	{
		vector<int> freq;
		for (int i = 0; i < cf.centerCount; i++)
			freq.push_back(0);
		wordCount4List(scene.getClip(i), freq, vacablulary);
		std::cout << "wordCount4List: Clip "<<i<< std::endl;
		frequences.push_back(freq);
	/*	int* data = frequence.ptr<int>(i);
		for (int j = 0; j < frequence.cols; j++)
			data[j]= freq[j];*/
	}
	/*saveMatrixData(frequence, "frequence.txt");*/
	//用统计词频得到的数据去训练LDA,得到预测模型的参数
	lda ldatr;
	ldatr.init_est(0.5, 0.1, 30, 100, frequences);
	ldatr.estimate();
}
Train::~Train()
{
}
