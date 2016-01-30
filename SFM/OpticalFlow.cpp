#include "OpticalFlow.h"

#include<video/tracking.hpp>
#include "Config.h"
#include <iostream>



#define pi 3.1425926
#define conv2Ang(x)  x*180/pi

extern Config cf;
OpticalFlow::OpticalFlow()
{
}


OpticalFlow::~OpticalFlow()
{
}


OpticalFlow::OpticalFlow(cv::Mat preFrame, cv::Mat curFrame){
	cv::Mat newPts, status, err, sampleP;
	std::vector<cv::Point2f> sampP;
	
	for (int i = 0; i < cf.height / 2; i++)
	for (int j = 0; j < cf.width / 2; j++)
		sampP.push_back(cv::Point2f(i, j));
	sampleP = cv::Mat(sampP);
	calcOpticalFlowPyrLK(preFrame, curFrame, sampleP, newPts, status, err, cv::Size(31, 31));
	cv::Mat velocity = newPts - sampleP;
	cv::Mat flow(cf.height / 2, cf.width / 2, CV_32FC1, cv::Scalar(0, 0));
	cv::Mat v;
	convPresentation(velocity, v);
	for (int i = 0; i < cf.height / 2; i++)
	for (int j = 0; j < cf.width / 2; j++){
		flow.at<float>(i, j) = v.at<cv::Vec2f>(i*cf.width / 2+j, 0)[0];//只保存社会的大小，先忽略社会力的方向
	}
	opticalFlowMap = flow;
}



//(x, y) -->(v, direction)
void convPresentation(cv::Mat xy_coor, cv::Mat& velocity)
{
	cv::Mat xy[2];
	split(xy_coor, xy);
	cv::Mat sepVel[2];
	sepVel[1] = xy[1] / xy[0];
	multiply(xy[0], xy[0], xy[0]);
	multiply(xy[1], xy[1], xy[1]);
	sqrt(xy[0] + xy[1], sepVel[0]);
	int rows = sepVel[1].rows;
	int cols = sepVel[1].cols;
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < cols; j++)
		sepVel[1].at<float>(i, j) = conv2Ang(atan(sepVel[1].at<float>(i, j)));
	merge(sepVel, 2, velocity);
}