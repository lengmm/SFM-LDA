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
	Clip& getClip(int clipnm){ return clips[clipnm]; }//����clip����ŵõ�Clip
	int getframeCount(){ return frameCount; }//���س�����֡������
	int getClipCount(){ return clipCount; }
private:
	vector<Clip> clips;
	int frameCount;//�����а�����֡������
	int clipCount;//�����а�����Clip��������������ͷ�Ȳ�����
	string dir;//��һ������֡���ڵ�Ŀ¼
};
void getPathStrings(string foldName, vector<string>& imgStrs);
#endif