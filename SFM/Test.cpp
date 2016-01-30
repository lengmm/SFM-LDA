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
	//�����е�ѵ��ͼƬ���ӵ�imgStr,ÿ�������ÿ��ַ�������
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

	//����ڶ�֡�����һ֡�����񴦵Ĺ�����������������flows��
	while (it != imgStr.cend() && (*it != "")){
		img = cv::imread(*it);
		OpticalFlow flow(preImg, img);
		flows.push_back(flow);
		preImg = img;
		it++;
	}

	//���ù����������ó����µ�����֡�������ͼ�������
	socialForceMap sfm(flows);

	//��sfm������������clip,word�Ƚ��зֿ�
	Scene scene(cf.trainR);


	std::vector<VisualWord>  selectwords;//������ÿ��Clip�������32��VisualWord�������ʱ�
	int clipAmount = 0;
	bool wordValidity = true;
	while (clipAmount<scene.getClipCount()){
		int wordAmount = 0;//�������浱ǰ֡���Լ�ѡ�еĴ���
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
	//��ѡ���ĵ��������쵥�ʱ�,selectwords��û32��VisualWord������һƪ�ĵ���
	std::cout << selectwords.end() - selectwords.begin();
	WordList vacablulary(selectwords);

	//���ݵ��ʱ�ͳ�Ƹ���document�е��ʳ��ֵ�Ƶ��
	vector<vector<int> > frequences;//����һ��scene������Clip�Ĵ�Ƶͳ������
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

	//����LDAȥԤ��
	//һƪ�ĵ��Ŀ�������ÿ�������������ԣ������Ѿ���õ�����ģ�Ϳ��Եõ�����log���ۼӣ�Ȼ���һ����ֵ���бȽϣ����������ֵ�����쳣
	//��ÿƪ�ĵ�����������ƶ���������������Ǹ�����ֵ���бȽ�

}
Test::~Test()
{
}
