#ifndef CONFIG_H
#define CONFIG_H
#include<vector>
#include<string>
using std::vector;
using std::string;
//用来配置基本的信息，如训练、测试数据集的文件位置，计算出来的社会力特征保存的位置，程序中用到的一些参数
class Config{
public:
	vector<string> testFiles;//测试数据所在的目录，还要扫描目录得到
	vector<string>  trainFiles;//训练数据所在的目录
	string testLabels;//用来保存测试数据集的标记结果
	string testR;//用来保存测试文件的社会力图
	string trainR;//用来保存训练文件的社会力图
	int cellrows, cellcols, cellbins;//网格的大小
	int width, height;//图像的大小
	int centerCount;//生成的单词表中单词数
	int selected;//每一帧提出的用来生成单词表的单词数
	Config() = default;
	Config(const char* );
private:
	void readFile(char* buff, vector<string>& outString);
};
void getImageStrings(string foldName, vector<string>& imgStrs);
bool isImageFormat(string s);
#endif