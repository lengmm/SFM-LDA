#include "Config.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>
Config::Config(const char * configfile){
	FILE* cfin = fopen(configfile, "r");
	if (cfin == NULL)
		perror("stdin redirect found error!\n");
	char buffer[256];
	fscanf(cfin, "trainFiles: %255s\n", buffer);
	readFile(buffer, trainFiles);

	fscanf(cfin, "testFiles: %255s\n", buffer);
	readFile(buffer, testFiles);

	fscanf(cfin, "testLabels: %255s\n",buffer );
	testLabels = buffer;

	fscanf(cfin, "trainR: %255s\n", buffer);
	trainR = buffer;

	fscanf(cfin, "testR: %255s\n", buffer);
	testR = buffer;

	fscanf(cfin, "cellrows: %2d, cellcols: %2d\n, cellbins: %2d\n", &cellrows, &cellcols, &cellbins);
	std::cout << cellrows <<" "<<cellcols<<" "<<cellbins<< std::endl;

	fscanf(cfin, "width: %4d, height: %4d\n",&width,&height);
	std::cout << width << " " << height << std::endl;
	fscanf(cfin, "centerCount: %2d\n", &centerCount);
	std::cout << centerCount << std::endl;
	fscanf(cfin, "selected: %2d", &selected);
	std::cout << selected << std::endl;
	fclose(cfin);

}
void Config::readFile(char* buff, vector<string>& outString)
{
	//std::cout << string(buff) << std::endl;
	FILE* in = fopen(buff, "r");
	if (!in)
	{
		printf("not found file %s.\n", buff);
		exit(1);
	}
	char ret[256];
	while (1)
	{
		fscanf(in, "%255s", ret);
		if (feof(in))
			break;
		outString.push_back(ret);
	}
	fclose(in);
}

//get the sequence image name in folder;
void getImageStrings(string foldName, vector<string>& imgStrs)
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
		if (!isImageFormat(s_dir->d_name))
			continue;
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


bool isImageFormat(string s)
{
	string subs;
	int findPos = s.find_last_of('.');
	if (findPos == string::npos)
		return false;
	subs = s.substr(findPos);
	if (strcmp(subs.c_str(), ".tif") == 0)
		return true;
	if (strcmp(subs.c_str(), ".TIF") == 0)
		return true;
	return false;
}

Config cf("config.txt");