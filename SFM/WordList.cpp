#include "WordList.h"
#include "Config.h"
#include "matrixIO.h"
#include <iostream>
extern Config cf;
WordList::WordList()
{
}

//用k-means算法对选定的单词进行聚类，聚成10类，类的中心就是单词表中的一个单词
WordList::WordList(std::vector<VisualWord>& selectedWords){
	int featureNum = selectedWords.end() - selectedWords.begin();
	int dims = cf.cellrows*cf.cellcols*cf.cellbins;

	cv::Mat samples(featureNum, dims, CV_32F,cv::Scalar(0));//sample的每一行是一个VisualWord
	cv::Mat clusters(featureNum, 1, CV_8U, cv::Scalar(0));
	cv::Mat centers(cf.centerCount, dims, CV_32F, cv::Scalar(0));//每一行就是一个类中心

	for (int i = 0; i < featureNum; i++){
		vector<float> wordvec;
		selectedWords[i].toVector(wordvec);
		for (int j = 0; j < dims; j++)
		{
			samples.at<float>(i, j) = wordvec[j];
		}
	}

    kmeans(samples, cf.centerCount, clusters,cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0),3, cv::KMEANS_PP_CENTERS, centers);
	//将center的每一行保存成一个VisualWord
	for (int i = 0; i < centers.rows; i++)
	{
		vector<float> wordVector;
		for (int j = 0; j < centers.cols; j++)
			wordVector.push_back(centers.at<float>(i, j));
		wordlist.push_back(VisualWord(wordVector));
	}

	//saveMatrixData(centers,"wordlist.txt");
}

//计算单词表中的单词在这个Clip中出现的频率
void wordCount4List(Clip& clip, vector<int>& wordfrequence, WordList wl){

	for (int n = 0; n < clip.getWordCount(); n++)
	{
		if (clip.getVisualWord(n).isValidity()){
			float mindist = 65535;
			int wordnm = 0;//保存最小距离对应的单词表在单词的序号
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
			//std::cout << n << "次循环" << wordnm << "个单词出现次数累加一次" << std::endl;
		}
		
	}
}


WordList::~WordList()
{
}
