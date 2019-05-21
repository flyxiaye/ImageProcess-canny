#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"
#include "Judge.h"

void Mid_Filtering(int *data, int n)		//中线滤波
{
	int i;

	//Count = 60 - Mid_Count;	//判断一共有多少多少可用中点
	for (i = 1; i < n; i++)	//从下向上滤波
	{
		data[i] = data[i - 1] * 0.3 + data[i] * 0.7;
	}
	for (i = n - 2; i >= 0; i--)	//从上向下互补滤波
	{
		data[i] = data[i + 1] * 0.3 + data[i] * 0.7;
	}
	//for (i = 59; i >= Count; i--)
	//{
	//	data[80 * i + Mid_Line[i]] = 0;	//将中线在图像上显示出来
	//}
}


int D2Pro[] = { 0, 11, 24, 36, 45, 53, 59, 65, 70, 74, 78, 81, 84, 87, 90, 92, 95, 96, 99 };
int Weight[] = { 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10 };
//y = 2.5e06*x^3 - 0.00012*x^2 + 0.011*x + 0.97
float Repare[] = { 1.000,1.052,1.190,1.336,1.482,1.614,1.716,1.895,2.012,
			2.173,2.233,2.362,2.508,2.672,2.810,2.911,3.019,3.135,3.327 };
int Curvature(int *array, int max) //计算曲率
{
	int L_AB, L_BC;
	int i;
	int curva;

	L_AB = MIN(50, max);
	L_BC = ML[0] - ML[L_AB];

	curva = 10000 * 2 * L_BC / (L_AB * L_AB + L_BC * L_BC) + 0.5;

	return curva;
	//向左偏移
	//向右偏移
		//处于赛道中心
}

int LeastError(int *array, int max)
{
	float sum_x2 = 0.0;
	float sum_y = 0.0;
	float sum_x = 0.0;
	float sum_xy = 0.0;
	float a, b, c;

	for (int i = 0; i <= max; ++i)
	{
		sum_x2 += i * i;
		sum_y += *(array + i);
		sum_x += i;
		sum_xy += i * *(array + i);
	}

	c = (1 + max) * sum_x2 - sum_x * sum_x;
	a = ((1 + max) * sum_xy - sum_x * sum_y) / c;
	return -(100 * a + 0.5);
	/*if (0 == c)
	{
		TmpLine.k = 0;
		TmpLine.b = 0;
		return TmpLine;
	}
	else
	{
		TmpLine.k = ((end_line - line) * sum_xy - sum_x * sum_y) / c;
		TmpLine.b = (sum_x2 * sum_y - sum_x * sum_xy) / c;
		return TmpLine;
	}*/
}

//int IsS(int *array, int prospect, int maxvalue)
//{
//	int MaxCol = *array, MinCol = *array;
//	int MaxRow = 0, MinRow = 0;
//	for (int i = 0; i < prospect; ++i)
//	{
//		if (*(array + i) > MaxCol)
//		{
//			MaxRow = i;
//			MaxCol = *(array + i);
//		}
//		if (*(array + i) < MinCol)
//		{
//			MinRow = i;
//			MinCol = *(array + i);
//		}
//	}
//
//	if (MinRow < prospect - 10 && MinRow > 10)  //左s弯
//	{
//		if (*array - MinCol > maxvalue && *(array + prospect) - MinCol > maxvalue)
//			return MinRow;
//		else return 0;
//	}
//	if ()
//}

//int D2Pro[] = { 0, 11, 24, 36, 45, 53, 59, 65, 70, 74, 78, 81, 84, 87, 90, 92, 95, 96, 99 };
//int Weight[] = { 100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10 };
////y = 2.5e06*x^3 - 0.00012*x^2 + 0.011*x + 0.97
//float Repare[] = { 1.000,1.052,1.190,1.336,1.482,1.614,1.716,1.895,2.012,
//			2.173,2.233,2.362,2.508,2.672,2.810,2.911,3.019,3.135,3.327 };

//加权平均
int WeightAve(int Distant, int Max, int *Array)
{
	int DisN = Distant / 5 - 2;
	if (D2Pro[DisN] <= Max);
	else
	{
		//          //补齐中线
		//          if (-1 == FillML(Array, Max, D2Pro[DisN]))
		//            ErrorFlag = 1;
		//          else ;
		//        }

		for (int i = 18; i >= 0; --i)
		{
			if (D2Pro[i] <= Max)
			{
				DisN = i;
				break;
			}
			else continue;
		}
	}

	if (1 == ErrorFlag)
		return 0;
	else
	{
		float SumUp = 0, SumDown = 0;

		for (int i = 0; i <= DisN; ++i)
		{
			SumUp += (160 - Array[D2Pro[i]]) * Repare[i];
			SumDown += Repare[i];

		}

		return (int)(0.5 + SumUp / SumDown);
	}

}


//速度控制
int SpeedC_2(int *LL, int *RL)
{
	//向上找到边界
	int max_row;
	int *pLL = LL, *pRL = RL;
	int tmp_ml = ML[0];
	static int LastSpeedType = 0;
	max_row = IMG_ROWS;
	for (int i = 0; i < IMG_ROWS - 1; ++i, ++pLL, ++pRL)
	{
		if ((*(pLL + 1) - tmp_ml) * (*pLL - tmp_ml) <= 0)
		{
			max_row = i;
			break;
		}
		if ((*(pRL + 1) - tmp_ml) * (*pRL - tmp_ml) <= 0)
		{
			max_row = i;
			break;
		}
	}
	return max_row;
}

//速度控制
int SpeedC_3(int *LL, int *RL)
{
	//向上找到边界
	int max_row[3] = { IMG_ROWS, IMG_ROWS, IMG_ROWS };
	int *pLL = LL, *pRL = RL;
	int tmp_ml[3] = { ML[0] - 20, ML[0], ML[0] + 20 };
	static int LastSpeedType = 0;
	for (int i = 0; i < IMG_ROWS - 1; ++i, ++pLL, ++pRL)
	{
		if ((*(pLL + 1) - tmp_ml[0]) * (*pLL - tmp_ml[0]) <= 0)
		{
			max_row[0] = i;
			break;
		}
		if ((*(pRL + 1) - tmp_ml[0]) * (*pRL - tmp_ml[0]) <= 0)
		{
			max_row[0] = i;
			break;
		}
	}
	pLL = LL; pRL = RL;
	for (int i = 0; i < IMG_ROWS - 1; ++i, ++pLL, ++pRL)
	{
		if ((*(pLL + 1) - tmp_ml[1]) * (*pLL - tmp_ml[1]) <= 0)
		{
			max_row[1] = i;
			break;
		}
		if ((*(pRL + 1) - tmp_ml[1]) * (*pRL - tmp_ml[1]) <= 0)
		{
			max_row[1] = i;
			break;
		}
	}
	pLL = LL; pRL = RL;
	for (int i = 0; i < IMG_ROWS - 1; ++i, ++pLL, ++pRL)
	{
		if ((*(pLL + 1) - tmp_ml[2]) * (*pLL - tmp_ml[2]) <= 0)
		{
			max_row[2] = i;
			break;
		}
		if ((*(pRL + 1) - tmp_ml[2]) * (*pRL - tmp_ml[2]) <= 0)
		{
			max_row[2] = i;
			break;
		}
	}
	str.Format("\r\n tmpml = %d \r\n", tmp_ml[0]); PrintDebug(str);
	str.Format("\r\n tmpml = %d \r\n", tmp_ml[1]); PrintDebug(str);
	str.Format("\r\n tmpml = %d \r\n", tmp_ml[2]); PrintDebug(str);
	str.Format("\r\n max_row = %d \r\n", max_row[0]); PrintDebug(str);
	str.Format("\r\n max_row = %d \r\n", max_row[1]); PrintDebug(str);
	str.Format("\r\n max_row = %d \r\n", max_row[2]); PrintDebug(str);
	int sum = 0, j = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (max_row[i] != IMG_ROWS)
		{
			sum += max_row[i];
			++j;
		}
	}
	if (0 == j)
		return 0;
	else
		return sum / j;
}

//整场平均
int WeightAveAll(int *Array, int Max)
{
	int SumUp = 0, SumDown = 0;
	for (int i = 0; i <= Max; ++i)
	{
		SumUp += 160 - *(Array + i);
	}
	return SumUp / ++Max;
}

//有效行平均
int WeightAveSome(int *Array, int Pro, int Max)
{
	int SumUp = 0, SumDown = 0;
	int MaxLine = MIN(Pro, Max);
	for (int i = 0; i <= MaxLine; ++i)
	{
		SumUp += 160 - *(Array + i);
	}
	return SumUp / ++MaxLine;
}

//================================================================//
//  @brief  :		一次平滑处理
//  @param  :		开始行startline, 开始列startcol
//  @return :		void
//  @note   :		void
//================================================================//
void LinearSmooth(signed int in[], signed int out[], signed int N)
{
	int i;
	if (N < 7)
	{
		for (i = 0; i < N; i++)
		{
			out[i] = in[i];
		}
	}
	else
	{
		out[0] = (13 * in[0] + 10 * in[1] + 7 * in[2] + (in[3] << 2) +
			in[4] - (in[5] << 1) - 5 * in[6]) / 28;

		out[1] = (5 * in[0] + (in[1] << 2) + 3 * in[2] + (in[3] << 1) +
			in[4] - in[6]) / 14;

		out[2] = (7 * in[0] + 6 * in[1] + 5 * in[2] + (in[3] << 2) +
			3 * in[4] + (in[5] << 1) + in[6]) / 28;
		for (i = 3; i < N - 3; i++)
		{
			out[i] = (in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3]) / 7;
		}
		out[N - 3] = (7 * in[N - 1] + 6 * in[N - 2] + 5 * in[N - 3] +
			(in[N - 4] << 2) + 3 * in[N - 5] + (in[N - 6] << 1) + in[N - 7]) / 28;

		out[N - 2] = (5 * in[N - 1] + (in[N - 2] << 2) + 3 * in[N - 3] +
			(in[N - 4] << 1) + in[N - 5] - in[N - 7]) / 14;

		out[N - 1] = (13 * in[N - 1] + 10 * in[N - 2] + 7 * in[N - 3] +
			(in[N - 4] << 2) + in[N - 5] - (in[N - 6] << 1) - 5 * in[N - 7]) / 28;
	}
}