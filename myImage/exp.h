#ifndef exp_H
#define exp_H

void Mid_Filtering(int *data, int n);		//�����˲�
int Curvature(int *array, int max);
int LeastError(int *array, int max);
//��Ȩƽ��
int WeightAve(int Distant, int Max, int *Array);
//�ٶȿ���
int SpeedC_2(int *LL, int *RL);
//�ٶȿ���
int SpeedC_3(int *LL, int *RL);
//����ƽ��
int WeightAveAll(int *Array, int Max);
//��Ч��ƽ��
int WeightAveSome(int *Array, int Pro, int Max);

void LinearSmooth(signed int in[], signed int out[], signed int N);


#endif // !exp

#pragma once
