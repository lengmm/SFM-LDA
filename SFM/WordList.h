#pragma once
#include <vector>
#include "VisualWord.h"
#include "Clip.h"
class WordList
{
public:
	WordList();
	WordList(std::vector<VisualWord>& );
	int getListSize(){ return wordlist.end() - wordlist.begin(); }
	VisualWord& getWord(int i){ return wordlist[i]; }
	~WordList();
private:
	std::vector<VisualWord> wordlist;
};


//根据单词表生成词频统计数据
void wordCount4List(Clip&, std::vector<int>&, WordList wl);

