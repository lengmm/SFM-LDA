#ifndef CLIP_H
#define CLIP_H
#include <vector>
#include <string>
#include<core/core.hpp>
#include "VisualWord.h"
class Clip
{
public:
	Clip();
	Clip(int ,std::vector<std::string>,int, int, int);
	~Clip();
	VisualWord getVisualWord(int wordNm){ return words[wordNm]; }
	int getWordCount(){ return wordCount; }
private:
	int rows;
	int cols;
	int bins;
	int clipNm;//��Clip�ڳ����е����
	int wordCount;
	std::vector<VisualWord> words;


};
#endif
