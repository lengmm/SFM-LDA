#include<iostream>
#include "Train.h"
#include "Test.h"

int main(int argc,char** argv){
	Train tr;
	Test te;
	std::cout << argv[1] << std::endl;
	string s = argv[1];
	if (s == "train")
	    tr.train();
	else if (s == "test")
		te.test();
	system("pause");
}