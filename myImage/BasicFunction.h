#pragma once
#ifndef BasicFuntion_H
#define BasicFuntion_H

#include "GlobalVar.h"

int FindL(int type, int line, int basic_line, int maxvalue);
int trend(int *array, int n);
int IsCross3(int type, int line);
int IsEage(int type, int row, int col, int n_white, int n_black);
int IsEageCol(int row, int col, int n_white, int n_black);
Line LeastSquare(int *array, int line, int end_line);
int ArrayMax(int *array, int n);
int ArrayMin(int *array, int n);
int JumpType(int row, int col);
int CutLine(int *array, int line);
int CutLineDown(int *array, int line);
void show(int a);
int Connect(int x1, int y1, int x2, int y2);
int AutoValue(int line);

#endif // !BasicFuntion_H
