#include "WordList.h"
#include "Config.h"
#include "matrixIO.h"
#include <iostream>
extern Config cf;
WordList::WordList()
{
}

//��k-means�㷨��ѡ���ĵ��ʽ��о��࣬�۳�10�࣬������ľ��ǵ��ʱ��е�һ������
WordList::WordList(std::vector<VisualWord>& selectedWords){
	int featureNum = selectedWords.end() - selectedWords.begin();
	int dims = cf.cellrows*cf.cellcols*cf.cellbins;

	cv::Mat samples(featureNum, dims, CV_32F,cv::Scalar(0));//sample��ÿһ����һ��VisualWord
	cv::Mat clusters(featureNum, 1, CV_8U, cv::Scalar(0));
	cv::Mat centers(cf.centerCount, dims, CV_32F, cv::Scalar(0));//ÿһ�о���һ��������

	for (int i = 0; i < featureNum; i++){
		vector<float> wordvec;
		selectedWords[i].toVector(wordvec);
		for (int j = 0; j < dims; j++)
		{
			samples.at<float>(i, j) = wordvec[j];
		}
	}

    kmeans(samples, cf.centerCount, clusters,cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0),3, cv::KMEANS_PP_CENTERS, centers);
	//��center��ÿһ�б����һ��VisualWord
	for (int i = 0; i < centers.rows; i++)
	{
		vector<float> wordVector;
		for (int j = 0; j < centers.cols; j++)
			wordVector.push_back(centers.at<float>(i, j));
		wordlist.push_back(VisualWord(wordVector));
	}

	//saveMatrixData(centers,"wordlist.txt");
}

//���㵥�ʱ��еĵ��������Clip�г��ֵ�Ƶ��
void wordCount4List(Clip& clip, vector<int>& wordfrequence, WordList wl){

	for (int n = 0; n < clip.getWordCount(); n++)
	{
		if (clip.getVisualWord(n).isValidity()){
			float mindist = 65535;
			int wordnm = 0;//������С�����Ӧ�ĵ��ʱ��ڵ��ʵ����
			for (int w = 1; w < cf.centerCount; w++)
			{
				float d = 0.0;
				if (d = similarity(wl.getWord(w), clip.getVisualWord(n)) < mindist)
				{
					mindist = d;
					wordnm = w;
				}
			}
			wordfrequence[wordnm]++;
			//std::cout << n << "��ѭ��" << wordnm << "�����ʳ��ִ����ۼ�һ��" << std::endl;
		}
		
	}
}


WordList::~WordList()
{
}
