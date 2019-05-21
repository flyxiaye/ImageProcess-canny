#ifndef exp_H
#define exp_H

void Mid_Filtering(int *data, int n);		//中线滤波
int Curvature(int *array, int max);
int LeastError(int *array, int max);
//加权平均
int WeightAve(int Distant, int Max, int *Array);
//速度控制
int SpeedC_2(int *LL, int *RL);
//速度控制
int SpeedC_3(int *LL, int *RL);
//整场平均
int WeightAveAll(int *Array, int Max);
//有效行平均
int WeightAveSome(int *Array, int Pro, int Max);

void LinearSmooth(signed int in[], signed int out[], signed int N);


#endif // !exp

#pragma once
