#ifndef BasicFun_H
#define BasicFun_H

float LeastSquare(int *array, int row1, int row2);

int IsEage(int row, int col);
int SearchUpEage(int row, int col);
int SearchDownEage(int row, int col);
int SearchLeftEage(int row, int col);
int SearchRightEage(int row, int col);
int SearchLeftNoEage(int row, int col);
int SearchRightNoEage(int row, int col);
Point SearchLeftUpEage(int row, int col);
Point SearchRightUpEage(int row, int col);
Point SearchLeftDownEage(int row, int col);
Point SearchRightDownEage(int row, int col);
Point SearchLeftUpEage45(int row, int col);
Point SearchRightUpEage45(int row, int col);

int GetLL(int row, int col);
int GetRL(int row, int col);
int AveGray(void);
int RegionAveGray(int row, int left_col, int right_col);

int MaxArray(int* line, int n);
int MinArray(int* line, int n);
signed int TrendArray(int* line, int n);
void AveFilter(int* line, int n);
int IsBigWave(int* line, int n);

int JudgeCurveEage(int up_row, int down_row, int type);
#endif //!BasicFun_H
#pragma once
