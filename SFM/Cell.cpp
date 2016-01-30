#include "Cell.h"
#include <stdio.h>


Cell::Cell()
{

}

Cell::Cell(int rows, int cols)
{
	init(rows, cols);
}

int Cell::getSize()
{
	return rows * cols;
}

int Cell::getRows()
{
	return rows;
}

int Cell::getCols()
{
	return cols;
}

void Cell::init(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;
}

