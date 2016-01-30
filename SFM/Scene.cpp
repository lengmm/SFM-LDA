#include "Scene.h"
#include "Config.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>
extern Config cf;

Scene::Scene()
{
}

Scene::Scene(string path) :dir(path)//path是这个场景中所有帧的目录
{
	vector<string>  imgStr;
	//std::cout << path << std::endl;
	getPathStrings(path, imgStr);//这里我们是要得到txt文件的路径
	frameCount = imgStr.end() - imgStr.begin();
	clipCount = frameCount / cf.cellbins;
	for (int i = 0; i < frameCount / cf.cellbins; i++)
	{
		//读10张图片
		vector<string>  imgPaths;
		for (int j = cf.cellbins * i; j < cf.cellbins * i + cf.cellbins; j++)
			imgPaths.push_back(*(imgStr.cbegin() + j));
		//用这10张图片的路径，clip的序号，图片的高宽，数目生成Clip
		clips.push_back(Clip(i,imgPaths,cf.height, cf.width, cf.cellbins));
	}
	
}
//get the sequence file name in folder;
void getPathStrings(string foldName, vector<string>& imgStrs)
{
	DIR* dir = 0;
	struct dirent* s_dir = 0;
	struct stat st;
	char path[500] = "";

	// printf("folder: %s\n", foldName.c_str());
	dir = opendir(foldName.c_str());
	if (dir == NULL)
	{
		printf("directory not exist!\n");
		return;
	}

	vector<string> substrs;
	while (s_dir = readdir(dir))
	{
		if (strcmp(s_dir->d_name, ".") == 0 || strcmp(s_dir->d_name, "..") == 0)
			continue;
		if (s_dir->d_name[0] == '.')
			continue;
		stat(s_dir->d_name, &st);
		substrs.push_back(s_dir->d_name);
	}
	if (dir)
		closedir(dir);
	sort(substrs.begin(), substrs.end());
	vector<string>::iterator it = substrs.begin();
	while (it != substrs.end())
	{
		sprintf(path, "%s/%s", foldName.c_str(), (*it).c_str());
		imgStrs.push_back(path);
		// printf("%s\n", path);
		++it;
	}
}

Scene::~Scene()
{
}