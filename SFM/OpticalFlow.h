#pragma once
#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H
#include<core/core.hpp>

class OpticalFlow
{
public:
	OpticalFlow();
	OpticalFlow::OpticalFlow(cv::Mat , cv::Mat);
	~OpticalFlow();
	cv::Mat getOpticalMap(){ return opticalFlowMap; }
private:
	cv::Mat  opticalFlowMap;
};

void convPresentation(cv::Mat xy_coor, cv::Mat& velocity);
#endif