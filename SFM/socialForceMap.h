#include<core/core.hpp>
#include "OpticalFlow.h"
#include <string>
#include <vector>
//���ù��������ɵ�һϵ�е�socialForceMap
class socialForceMap
{
public:
	socialForceMap()=default;
	socialForceMap(std::vector<OpticalFlow>& );
	~socialForceMap();
private:
	std::vector<cv::Mat> socialForceMaps;
};