#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <string>
#include "Clip.h"

using std::vector;
using std::string;
class Scene
{
public:
	Scene();
	Scene(string );
	~Scene();
	Clip& getClip(int clipnm){ return clips[clipnm]; }//根据clip的序号得到Clip
	int getframeCount(){ return frameCount; }//返回场景中帧的数量
	int getClipCount(){ return clipCount; }
private:
	vector<Clip> clips;
	int frameCount;//场景中包含的帧的数量
	int clipCount;//场景中包含的Clip的数量，最后的零头先不考虑
	string dir;//这一场景中帧所在的目录
};
void getPathStrings(string foldName, vector<string>& imgStrs);
#endif