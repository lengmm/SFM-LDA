//һ֡�������һ����Ԫ
#ifndef CELL_H
#define CELL_H

struct Cell
{
public:
	Cell();
	Cell(int rows, int cols);
	int getSize();
	int getRows();
	int getCols();
	void init(int rows, int cols);
private:
	int rows;
	int cols;
};


#endif