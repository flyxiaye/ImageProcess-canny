#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"

//================================================================//
//  @brief  :		在指定行查找边界点
//  @param  :		类型（type) 指定行（line） 参考行（basic_line） 阈值（maxvalue）
//  @return :		0未找到边界点 1：已找到边界点 2: 图片边界 3:非边缘的图片边界
//  @note   :		void
//================================================================//
int FindL(int type, int line, int basic_line, int maxvalue)
{
	if (line <= 0 || line >= IMG_ROWS)
		return -1;
	if (type == FIND_LEFT)								//左线
	{
		int i;
		int LeftCol = LL[basic_line];
		int k1, k2;
		int TmpJudge, TmpJudge_2;
		for (i = 0; i <= maxvalue; ++i)
		{
			k1 = LeftCol - i; k2 = LeftCol + i;
			TmpJudge = IsEage(type, line, k1, 4, 1);
			TmpJudge_2 = IsEage(type, line, k2, 4, 1);
			if (TmpJudge)
			{
				LL[line] = k1;
				break;
			}
			if (TmpJudge_2)
			{
				LL[line] = k2;
				break;
			}
		}
		if (2 == TmpJudge || 2 == TmpJudge_2)
			return 2;
		else if (maxvalue == --i)
		{
			if (k1 <= LEFT_EAGE)
			{
				LL[line] = LEFT_EAGE;
				return 3;
			}
			else if (k2 >= RIGHT_EAGE)
			{
				LL[line] = RIGHT_EAGE;
				return 3;
			}
			else return 0;
		}
	}
	else                              //右线
	{
		int i;
		int RightCol = RL[basic_line];
		int k1, k2;
		int TmpJudge, TmpJudge_2;
		for (i = 0; i <= maxvalue; ++i)
		{
			k1 = RightCol - i, k2 = RightCol + i;
			TmpJudge = IsEage(type, line, k1, 4, 1);
			TmpJudge_2 = IsEage(type, line, k2, 4, 1);
			if (TmpJudge)
			{
				RL[line] = k1;
				break;
			}
			if (TmpJudge_2)
			{
				RL[line] = k2;
				break;
			}
		}
		if (2 == TmpJudge || 2 == TmpJudge_2)
			return 2;
		else if (maxvalue == --i)
		{
			if (k1 <= LEFT_EAGE)
			{
				RL[line] = LEFT_EAGE;
				return 3;
			}
			else if (k2 >= RIGHT_EAGE)
			{
				RL[line] = RIGHT_EAGE;
				return 3;
			}
			else return 0;
		}
	}

	return 1;
}

//================================================================//
//  @brief  :		趋势判断
//  @param  :		首元素地址（*array）  首元素起始后（n)个数据
//  @return :		1 右转	-1 左转		0 无转弯
//  @note   :		void
//================================================================//
int trend(int *array, int n)
{
	int *parray = array;
	int trend_r = 0, trend_l = 0;
	for (int i = 1; i < n; ++i)
	{
		if (*parray - *(parray + 1) > 0)
			trend_l++;
		else if (*(parray + 1) - *parray > 0)
			trend_r++;
		else;
		parray++;
	}
	if (trend_r > trend_l)
		return 1;
	else if (trend_r < trend_l)
		return -1;
	else return 0;
}

//================================================================//
//  @brief  :		判断是否有转弯
//  @param  :		类型（type)  判断行（line)
//  @return :		0无转弯 else 转弯点所在行
//  @note   :		void
//================================================================//
int IsCross3(int type, int line)
{

	static int LostLine = IMG_ROWS;
	int flag;
	int *linePoint;

	if (line == 0)
	{
		row_n = 0;
		g_trend_flag = 0;
		LostLine = IMG_ROWS;
	}
	else
	{
		if (type == FIND_LEFT)
			linePoint = LL;
		else linePoint = RL;

		if (linePoint[line] - linePoint[line - 1] > 0)
			flag = 1;
		else if (linePoint[line] - linePoint[line - 1] < 0)
			flag = -1;
		else flag = 0;

		if (g_trend_flag == 0)
			g_trend_flag = trend(linePoint + line - 5, 5);
		else if (0 == flag)
		{
			if (0 == row_n)
			{
				LostLine = line - 1;
			}
			else
			{
				++row_n;
				LostLine = MIN(LostLine, line - 1);
			}
		}
		else if (flag != g_trend_flag)
		{
			++row_n;
			LostLine = MIN(LostLine, line - 1);
		}
		else if (flag == g_trend_flag)
		{
			LostLine = line - 1;
			row_n = 0;
		}

		if (row_n == 5)
			return LostLine;
		else return 0;
	}
}

//================================================================//
//  @brief  :		边缘判断
//  @param  :		类型（type)  边缘点所在行（row）所在列（col） 边缘点旁白点个数（n_white）黑点个数（n_black）
//  @return :		0不是边缘 1是边缘	2图像边界
//  @note   :		void
//================================================================//
int IsEage(int type, int row, int col, int n_white, int n_black)
{
	if (col > RIGHT_EAGE || col < LEFT_EAGE)
		return 0;

	int tmp_row = (85 - row + FRONT_LINE) * 24;

	if (FIND_LEFT == type)
	{
		if ((ImgBuff[tmp_row + (col >> 3)] & Bit[col & 7]) == 0x00)
		{
			int n_white_max = MIN(col + n_white, RIGHT_EAGE);
			int n_black_max = MAX(col - n_black, LEFT_EAGE);
			for (int i = col + 1; i <= n_white_max; ++i)
			{
				if ((ImgBuff[tmp_row + (i >> 3)] & Bit[i & 7]) == 0x00)
					continue;
				else return 0;
			}
			for (int i = col - 1; i >= n_black_max; --i)
			{
				if ((ImgBuff[tmp_row + (i >> 3)] & Bit[i & 7]) == Bit[i & 7])
					continue;
				else return 0;
			}
			if (LEFT_EAGE == col || RIGHT_EAGE == col)
				return 2;
			else
				return 1;
		}
		else return 0;
	}
	else if (FIND_RIGHT == type)
	{
		if ((ImgBuff[tmp_row + (col >> 3)] & Bit[col & 7]) == 0x00)
		{
			int n_white_max = MAX(col - n_white, LEFT_EAGE);
			int n_black_max = MIN(col + n_black, RIGHT_EAGE);
			for (int i = col + 1; i <= n_black_max; ++i)
			{
				if ((ImgBuff[tmp_row + (i >> 3)] & Bit[i & 7]) == Bit[i & 7])
					continue;
				else return 0;
			}
			for (int i = col - 1; i >= n_white_max; --i)
			{
				if ((ImgBuff[tmp_row + (i >> 3)] & Bit[i & 7]) == 0x00)
					continue;
				else return 0;
			}
			if (LEFT_EAGE == col || RIGHT_EAGE == col)
				return 2;
			else
				return 1;
		}
		else return 0;
	}
	else return 0;
}

//================================================================//
//  @brief  :		边缘判断 列向
//  @param  :		边缘点所在行（row）所在列（col） 边缘点旁白点个数（n_white）黑点个数（n_black）
//  @return :		0不是边缘 1是边缘
//  @note   :		void
//================================================================//
int IsEageCol(int row, int col, int n_white, int n_black)
{
	if (col < LEFT_EAGE || col > RIGHT_EAGE)
		return -1;

	unsigned char *p = &ImgBuff[(85 - row + FRONT_LINE) * 24 + (col >> 3)];
	int TmpCol = col & 7;
	if ((*p & Bit[TmpCol]) == Bit[TmpCol])
	{
		int n_black_max = MIN(row + n_black, IMG_ROWS - 1);
		int n_white_max = MAX(row - n_white, 0);
		unsigned char *tmp_p = p;
		for (int i = row; i <= n_black_max; ++i)
		{
			tmp_p -= 40;
			if ((*tmp_p & Bit[TmpCol]) == Bit[TmpCol]);
			else return 0;
		}
		unsigned char *tmp_p2 = p;
		for (int i = row; i >= n_white_max; --i)
		{
			tmp_p2 += 40;
			if ((*tmp_p2 & Bit[TmpCol]) == 0x00);
			else return 0;
		}
	}
	else return 0;
	return 1;
}

//================================================================//
//  @brief  :		最小二乘法
//  @param  :		数组首地址（*array） 开始行（line） 结束行（end_line）无数据
//  @return :		斜率k 截距b
//  @note   :		{0， 0}代表出错
//================================================================//
Line LeastSquare(int *array, int line, int end_line)
{
	Line TmpLine;

	float sum_x2 = 0.0;
	float sum_y = 0.0;
	float sum_x = 0.0;
	float sum_xy = 0.0;
	float a, b, c;

	for (int i = line; i < end_line; ++i)
	{
		sum_x2 += i * i;
		sum_y += *(array + i);
		sum_x += i;
		sum_xy += i * *(array + i);
	}

	c = (end_line - line) * sum_x2 - sum_x * sum_x;
	if (0 == c)
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
	}
}

//================================================================//
//  @brief  :		求数组最值
//  @param  :		数组首地址（*array） 首地址开始个数（n）
//  @return :		最大值（max）	最小值（min）
//  @note   :		void
//================================================================//
int ArrayMax(int *array, int n)
{
	int *p = array;
	int max = *p;
	for (int i = 1; i < n; ++i)
	{
		if (*(++p) > max)
			max = *p;
	}
	return max;
}
int ArrayMin(int *array, int n)
{
	int *p = array;
	int min = *p;
	for (int i = 1; i < n; ++i)
	{
		if (*(++p) < min)
			min = *p;
	}
	return min;
}

//================================================================//
//  @brief  :		判断给出点上方颜色来判断内外跳变
//  @param  :		点行数（row） 列数（col）
//  @return :		内跳1	外跳2
//  @note   :		void
//================================================================//
int JumpType(int row, int col)
{
	++row;
	int TmpRow = (85 - row + FRONT_LINE) * 24;
	if ((ImgBuff[TmpRow + (col >> 3)] & Bit[col & 7]) == 0x00)
		return 2;
	else return 1;
}

//================================================================//
//  @brief  :		直线修剪
//  @param  :		*array line
//  @return :		line
//  @note   :		void
//================================================================//
int CutLine(int *array, int line)
{
	int *p = array;
	while (1)
	{
		if (*(p + 1) == LEFT_EAGE || *(p + 1) == RIGHT_EAGE)
			return line;
		if (*(p + 1) - *p < 6 && *p - *(p + 1) < 6)
			break;
		else
		{
			++line;
			++p;
		}
	}
	return line;
}
int CutLineDown(int *array, int line)
{
	int *p = array;
	while (1)
	{
		if (*(p - 1) == LEFT_EAGE || *(p - 1) == RIGHT_EAGE)
			return line;
		if (*p - *(p - 1) < 6 && *(p - 1) - *p < 6
			&& *(p - 1) - *(p - 2) < 6 && *(p - 2) - *(p - 1) < 6)
			break;
		else
		{
			--line;
			--p;
		}
	}
	return line;
}

//================================================================//
//  @brief  :		连通域
//  @param  :		两点点横纵坐标（x1，y1）（x2，y2）
//  @return :		连通1 不连通0
//  @note   :		void
//================================================================//
int Connect(int x1, int y1, int x2, int y2)
{
	//	str.Format("\r\n x1 = %d \r\n", x1); PrintDebug(str);
	//	str.Format("\r\n x2 = %d \r\n", x2); PrintDebug(str);
	//	str.Format("\r\n y1 = %d \r\n", y1); PrintDebug(str);
	//	str.Format("\r\n y2 = %d \r\n", y2); PrintDebug(str);
	if (x1 == x2)
		return 0;
	else
	{
		float k = (float)(y1 - y2) / (float)(x1 - x2);
		int Black_N = 0;
		int y;
		for (int i = x1 + 1; i < x2; ++i)
		{
			y = (int)(k * (i - x1) + y1 + 0.5);
			if ((ImgBuff[(85 - i + FRONT_LINE) * 24 + (y >> 3)] & Bit[y & 7]) != 0x00)
				Black_N++;
			if (2 == Black_N)
				return 0;
		}
	}
	return 1;
}

//================================================================//
//  @brief  :		动态阈值
//  @param  :		行数
//  @return :		阈值
//  @note   :		void
//================================================================//
int AutoValue(int line)
{
	if (line < 35)
		return 12;
	else if (line < 65)
		return 10;
	else if (line < 105)
		return 9;
	else return 7;

}

void show(int a)
{
	for (int j = LEFT_EAGE; j <= RIGHT_EAGE; ++j)
		ImageData[127 - a][j] = 128;
}