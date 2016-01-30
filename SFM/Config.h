#ifndef CONFIG_H
#define CONFIG_H
#include<vector>
#include<string>
using std::vector;
using std::string;
//�������û�������Ϣ����ѵ�����������ݼ����ļ�λ�ã������������������������λ�ã��������õ���һЩ����
class Config{
public:
	vector<string> testFiles;//�����������ڵ�Ŀ¼����Ҫɨ��Ŀ¼�õ�
	vector<string>  trainFiles;//ѵ���������ڵ�Ŀ¼
	string testLabels;//��������������ݼ��ı�ǽ��
	string testR;//������������ļ��������ͼ
	string trainR;//��������ѵ���ļ��������ͼ
	int cellrows, cellcols, cellbins;//����Ĵ�С
	int width, height;//ͼ��Ĵ�С
	int centerCount;//���ɵĵ��ʱ��е�����
	int selected;//ÿһ֡������������ɵ��ʱ�ĵ�����
	Config() = default;
	Config(const char* );
private:
	void readFile(char* buff, vector<string>& outString);
};
void getImageStrings(string foldName, vector<string>& imgStrs);
bool isImageFormat(string s);
#endif