#ifndef SOCIALFORCEMAP_H
#define SOCIALFORCEMAP_H


#define pi 3.1425926
#define conv2Ang(x)  x*180/pi

#include "socialForceMap.h"
#include "Config.h"
#include "matrixIO.h"
#include<core/core.hpp>
#include<video/tracking.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
extern  Config cf;

socialForceMap::~socialForceMap()
{
}

//�е�ǰ֡��ǰһ֡��������ǰ֡�������ϵ������
socialForceMap::socialForceMap(std::vector<OpticalFlow>& flows){
	//flow��һ��img.rows/2�У�img.cols/2�е�Mat����Ӧÿ֡�����񴦵Ĺ�������
	//��the average optical flow field Oave

	//�ռ��϶�flow�ø�˹����ƽ�����ô�С5*5�ĸ�˹��
	std::vector<cv::Mat> averageflows;
	for (auto it = flows.begin(); it != flows.end(); it++){
		cv::Mat averageflow;
		cv::GaussianBlur(it->getOpticalMap(), averageflow, cv::Size(5, 5), 0, 0);
		averageflows.push_back(averageflow);
	}

	//��ʱ����ֱ��������ƽ��,ÿ9֡��һ��ƽ������Ե�ϵĲ���
	for (auto iter = averageflows.begin() + 4; iter != averageflows.end() - 4; iter++)
	{
		for (int i = 1; i < 5; i++)
			*iter += *(iter + i) + *(iter-i);
		*iter /= 9;
	}

	//����flows����ٶ�
	std::vector<cv::Mat> accs;
	accs.push_back(cv::Mat::zeros(cf.height / 2, cf.width / 2, CV_32FC1));
	for (auto it = flows.begin() + 1; it != flows.end(); it++){
		cv::Mat acc;
		absdiff(it->getOpticalMap(), (it - 1)->getOpticalMap(), acc);//I=|I1-I2|;
		accs.push_back(acc);
	}
	//caculate social force
	std::vector<cv::Mat> socialforces;
	float t = 1.0;
	int i = 0;
	while (((flows.begin() + i) != flows.end()) && ((averageflows.begin() + i) != averageflows.end()) && ((accs.begin() + i) != accs.end()))
	{
		cv::Mat tmp;
		absdiff((flows.begin() + i)->getOpticalMap(), *(averageflows.begin() + i), tmp);
		cv::Mat socialforce;
		absdiff(tmp * 1 / t, *(accs.begin() + i), socialforce);
	/*	for (int i = 0; i < socialforce.rows; i++)
		for (int j = 0; j < socialforce.cols; j++)
		{
			if (socialforce.at<float>(i, j) < 0)
				std::cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
		}*/
		socialforces.push_back(socialforce);
		i++;
	}


	//map the magnitude of the interaction force vectors to the image plane
	//such that for every pixel in the frame there is a corresponding force vector
	for (int i = 0; i < socialforces.end() - socialforces.begin(); i++)
	{
		cv::Mat socialForceMap;
		cv::resize(*(socialforces.begin() + i), socialForceMap, cv::Size(), 2, 2, cv::INTER_LINEAR);
		socialForceMaps.push_back(socialForceMap);
		//����������������ͼ�񱣴�����
		std::string filename;
		std::stringstream ss;
		ss << i;
		std::string s1 = ss.str();
		filename = cf.trainR+"/"+s1 + ".txt";
		//std::cout << filename << std::endl;
	saveMatrixData(socialForceMap, filename.c_str());
	}
}

#endif