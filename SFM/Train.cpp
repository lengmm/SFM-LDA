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
	//�����е�ѵ��ͼƬ���ӵ�imgStr,ÿ�������ÿ��ַ�������
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
	//std::cout << "��ʼ�����������" << std::endl;
	////imgStr���������г�����֡��·����Ϣ��������ֻ����һ������������ڶ�֡�����һ֡�����񴦵Ĺ�����������������flows��
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
	//std::cout << "��ʼ���������" << std::endl;
	////���ù����������ó����µ�����֡�������ͼ�������
	//socialForceMap sfm(flows);
	//std::cout << "��ʼ��������" << std::endl;
	//��sfm������������clip,word�Ƚ��зֿ�
	Scene scene(cf.trainR);

	std::cout << "��ʼѡ��" << std::endl;
	std::vector<VisualWord>  selectwords;//������ÿ��Clip�������32��VisualWord�������ʱ�
	int clipAmount = 0;
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
	std::cout << "��ʼ�������ʱ�" << std::endl;
	//��ѡ���ĵ��������쵥�ʱ�,selectwords��û32��VisualWord������һƪ�ĵ���
	//std::cout << selectwords.end() - selectwords.begin();
	WordList vacablulary(selectwords);

	std::cout << "��ʼͳ�ƴ�Ƶ" << std::endl;
	//���ݵ��ʱ�ͳ�Ƹ���document(clip)�е��ʳ��ֵ�Ƶ��
	vector<vector<int> > frequences;//����һ��scene������Clip�Ĵ�Ƶͳ������
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
	//��ͳ�ƴ�Ƶ�õ�������ȥѵ��LDA,�õ�Ԥ��ģ�͵Ĳ���
	lda ldatr;
	ldatr.init_est(0.5, 0.1, 30, 100, frequences);
	ldatr.estimate();
}
Train::~Train()
{
}
