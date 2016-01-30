#include<core/core.hpp>
#include "OpticalFlow.h"
#include <string>
#include <vector>
//利用光流法生成的一系列的socialForceMap
class socialForceMap
{
public:
	socialForceMap()=default;
	socialForceMap(std::vector<OpticalFlow>& );
	~socialForceMap();
private:
	std::vector<cv::Mat> socialForceMaps;
};