#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "canny.h"



//================================================================//
//  @brief  :		判断是否为边缘
//  @param  :		void
//  @return :		1是 0不是
//  @note   :		void
//================================================================//
int IsEage(int row, int col)
{
	//限幅
	if (row > DOWN_EAGE) row = DOWN_EAGE;
	if (row < UP_EAGE) row = UP_EAGE;
	if (col <= LEFT_EAGE) col = LEFT_EAGE + 1;
	if (col > RIGHT_EAGE) col = RIGHT_EAGE;
	if (FindLineType)
	{
		unsigned char* p = ImageData[0] + IMG_COL * (row - 1) + col - 1;
		unsigned char ack = 0;
		ack += *p++ < LightThreshold;
		ack += *p++ < LightThreshold;
		p += (IMG_COL - 2);
		ack += *p++ < LightThreshold;
		ack += *p++ < LightThreshold;
		if (!ack) return 0;
	}
	if (HIGH_TH == ImageEage[row][col])
		return 1;
	else return 0;
}


//================================================================//
//  @brief  :		向左搜索一个边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int SearchLeftEage(int row, int col)
{
	for (; col > LEFT_EAGE; --col)
	{
		if (IsEage(row, col))
			return col;
	}
	return LEFT_EAGE;
}

//================================================================//
//  @brief  :		向右搜索一个边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int SearchRightEage(int row, int col)
{
	for (; col < RIGHT_EAGE; ++col)
	{
		if (IsEage(row, col))
			return col;
	}
	return RIGHT_EAGE;
}

//================================================================//
//  @brief  :		向上搜索一个边界点
//  @param  :		void
//  @return :		边界点所在行
//  @note   :		void
//================================================================//
int SearchUpEage(int row, int col)
{
	for (; row > UP_EAGE; --row)
	{
		if (IsEage(row, col))
			return row;
	}
	return UP_EAGE;
}

//================================================================//
//  @brief  :		向下搜索一个边界点
//  @param  :		void
//  @return :		边界点所在行
//  @note   :		void
//================================================================//
int SearchDownEage(int row, int col)
{
	for (; row < DOWN_EAGE; ++row)
	{
		if (IsEage(row, col))
			return row;
	}
	return DOWN_EAGE;
}

//================================================================//
//  @brief  :		向左搜索一个非边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int SearchLeftNoEage(int row, int col)
{
	for (; col > LEFT_EAGE; --col)
	{
		if (!IsEage(row, col))
			return col;
	}
	return LEFT_EAGE;
}

//================================================================//
//  @brief  :		向右搜索一个非边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int SearchRightNoEage(int row, int col)
{
	for (; col < RIGHT_EAGE; ++col)
	{
		if (!IsEage(row, col))
			return col;
	}
	return RIGHT_EAGE;
}


//================================================================//
//  @brief  :		向左上搜索一个边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
Point SearchLeftUpEage(int row, int col)
{
	int i = row, j = col;
	unsigned char k = 0;
	Point Up45;
	while (i > UP_EAGE && j > LEFT_EAGE)
	{
		if (IsEage(i, j))
		{
			Up45.Row = i;
			Up45.Col = j;
			return Up45;
		}
		if (0 == k)
		{
			k = 1;
			--i;
		}
		else
			k = 0;
		--j;
	}
	Up45.Row = i;
	Up45.Col = j;
	return Up45;
}

//================================================================//
//  @brief  :		向左上搜索一个边界点45
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
Point SearchLeftUpEage45(int row, int col)
{
	int i = row, j = col;
	unsigned char k = 0;
	Point Up45;
	while (i > UP_EAGE && j > LEFT_EAGE)
	{
		if (IsEage(i, j))
		{
			Up45.Row = i;
			Up45.Col = j;
			return Up45;
		}
		--i;
		--j;
	}
	Up45.Row = i;
	Up45.Col = j;
	return Up45;
}

//================================================================//
//  @brief  :		向右上搜索一个边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
Point SearchRightUpEage(int row, int col)
{
	int i = row, j = col;
	unsigned char k = 0;
	Point Up45;
	while (i > UP_EAGE && j < RIGHT_EAGE)
	{
		if (IsEage(i, j))
		{
			Up45.Row = i;
			Up45.Col = j;
			return Up45;
		}
		if (0 == k)
		{
			k = 1;
			--i;
		}
		else k = 0;
		++j;
	}
	Up45.Row = i;
	Up45.Col = j;
	return Up45;
}

//================================================================//
//  @brief  :		向右上搜索一个边界点45
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
Point SearchRightUpEage45(int row, int col)
{
	int i = row, j = col;
	unsigned char k = 0;
	Point Up45;
	while (i > UP_EAGE && j < RIGHT_EAGE)
	{
		if (IsEage(i, j))
		{
			Up45.Row = i;
			Up45.Col = j;
			return Up45;
		}
		--i;
		++j;
	}
	Up45.Row = i;
	Up45.Col = j;
	return Up45;
}

//================================================================//
//  @brief  :		向左下搜索一个边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
Point SearchLeftDownEage(int row, int col)
{
	int i = row, j = col;
	Point Up45;
	while (i < DOWN_EAGE && j > LEFT_EAGE)
	{
		if (IsEage(i, j))
		{
			Up45.Row = i;
			Up45.Col = j;
			return Up45;
		}
		++i; --j;
	}
	Up45.Row = i;
	Up45.Col = j;
	return Up45;
}

//================================================================//
//  @brief  :		向右下搜索一个边界点
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
Point SearchRightDownEage(int row, int col)
{
	int i = row, j = col;
	Point Up45;
	while (i < DOWN_EAGE && j < RIGHT_EAGE)
	{
		if (IsEage(i, j))
		{
			Up45.Row = i;
			Up45.Col = j;
			return Up45;
		}
		++i; ++j;
	}
	Up45.Row = i;
	Up45.Col = j;
	return Up45;
}
//================================================================//
//  @brief  :		搜左边线
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int GetLL(int row, int col)
{
	int TmpCol;
	if (row == DOWN_EAGE - 1)
		col += 8;
	else
		col += IN_OFFSET;
	if (col > RIGHT_EAGE) col = RIGHT_EAGE;
	if (!IsEage(row, col))
		TmpCol = SearchLeftEage(row, col);
	else
		TmpCol = SearchRightNoEage(row, col) - 1;
	return TmpCol;
}

//================================================================//
//  @brief  :		搜右边线
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int GetRL(int row, int col)
{
	int TmpCol;
	if (row == DOWN_EAGE - 1)
		col -= 8;
	else
		col -= IN_OFFSET;
	if (col <= LEFT_EAGE) col = LEFT_EAGE + 1;
	if (!IsEage(row, col))
		TmpCol = SearchRightEage(row, col);
	else
		TmpCol = SearchLeftNoEage(row, col) + 1;
	return TmpCol;
}

//================================================================//
//  @brief  :		最小二乘法
//  @param  :		起始行（row1） 结束行（row2） 首地址（array）
//  @return :		k斜率
//  @note   :		void
//================================================================//
float LeastSquare(int* array, int row1, int row2)
{
	float sum_x2 = 0.0;
	float sum_y = 0.0;
	float sum_x = 0.0;
	float sum_xy = 0.0;
	float  c;

	for (int i = row1; i >= row2; --i)
	{
		sum_x2 += i * i;
		sum_y += *(array + i);
		sum_x += i;
		sum_xy += i * *(array + i);
	}

	c = (row1 - row2 + 1) * sum_x2 - sum_x * sum_x;
	if (0 == c)
		return 0;
	else
		return ((row1 - row2 + 1) * sum_xy - sum_x * sum_y) / c;
}

//================================================================//
//  @brief  :		判断曲直
//  @param  :		type : 1判断LL，2判断RL
//  @return :		return : 1为曲线，0为直线
//  @note   :		void
//================================================================//
int JudgeCurveEage(int up_row, int down_row, int type)
{
	if (type == 1)
	{
		if ((LL[up_row] + LL[down_row]) / 2 - LL[(up_row + down_row) / 2] < 10
			&& LL[(up_row + down_row) / 2] - (LL[up_row] + LL[down_row]) / 2 < 10)
			return 0;
		else
			return 1;
	}
	else if (type == 2)
		if ((RL[up_row] + RL[down_row]) / 2 - RL[(up_row + down_row) / 2] < 10
			&& RL[(up_row + down_row) / 2] - (RL[up_row] + RL[down_row]) / 2 < 10)
			return 0;
		else
			return 1;
}


//================================================================//
//  @brief  :		计算灰度平均值
//  @param  :		数组首地址Array n个数
//  @return :		最值
//  @note   :		void
//================================================================//
int AveGray(void)
{
	unsigned char* p = ImageData[UP_EAGE];
	long int sum = 0;
	for (int i = 0; i < 188 * 51; i++)
	{
		sum += *p++;
	}
	return sum / 188 / 51;

}

//================================================================//
//  @brief  :		计算区间内灰度平均值
//  @param  :		数组首地址Array n个数
//  @return :		最值
//  @note   :		void
//================================================================//
int RegionAveGray(int row,int left_col,int right_col)
{
	long int sum = 0;
	int width = right_col - left_col;
	for (int j = left_col; j < right_col; j++)
	{
		sum += ImageData[row][j];
	}
	return sum / width;
}

//================================================================//
//  @brief  :		求数组最值
//  @param  :		数组首地址Array n个数
//  @return :		最值
//  @note   :		void
//================================================================//
int MaxArray(int* line, int n)
{
	int max = 0;
	for (int i = 0; i < n; i++)
	{
		if (*line > max) max = *line;
		line--;
	}
	return max;
}
int MinArray(int* line, int n)
{
	int min = 0;
	for (int i = 0; i < n; i++)
	{
		if (*line < min) min = *line;
		line--;
	}
	return min;
}

//================================================================//
//  @brief  :		求数组趋势
//  @param  :		数组首地址Array n个数
//  @return :		趋势 -1向左 1向右 0无朝向
//  @note   :		void
//================================================================//
signed int TrendArray(int* line, int n)
{
	int LeftNum = 0, RightNum = 0;
	for (int i = 0; i < n - 1; i++)
	{
		if (*line > * (line - 1))LeftNum++;
		else if (*line < *(line - 1))RightNum++;
		else;
		line--;
	}
	if (LeftNum > RightNum)return -1;
	else if (LeftNum < RightNum)return 1;
	else return 0;
}

//================================================================//
//  @brief  :		均值滤波
//  @param  :		数组首地址Array n个数
//  @return :		void
//  @note   :		void
//================================================================//
void AveFilter(int* line, int n)
{
	const int filter_threshold = 7;	//滤波阈值
	line--; n -= 2;
	while (n-- > 0)
	{
		if ((*line - *(line + 1) > filter_threshold || *(line + 1) - *line > filter_threshold)
			&& (*line - *(line - 1) > filter_threshold || *(line - 1) - *line > filter_threshold))
			* line = (*(line - 1) + *(line + 1)) >> 1;
		line--;
	}
}

//================================================================//
//  @brief  :		是否存在大波动
//  @param  :		数组首地址Array n个数
//  @return :		void
//  @note   :		void
//================================================================//
int IsBigWave(int* line, int n)
{
	AveFilter(line, n);		//滤波
	//for (int i = 0; i < n; i++)
	//{
	//	string.Format("\r\n LL = %d \r\n", *(line-i)); PrintDebug(string);
	//}
	int* line_init = line;
	while (--n > 0)
	{
		if (*line - *(line - 1) > FINDLINE_TH || *(line - 1) - *line > FINDLINE_TH)
			return line_init - line + 1;
		line--;
	}
	return 0;
}