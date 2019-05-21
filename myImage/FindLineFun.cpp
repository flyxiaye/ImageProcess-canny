#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FindLineFun.h"
#include "FillSpecialLine.h"
#include "Judge.h"

//================================================================//
//  @brief  :		前十行寻找
//  @param  :		void（line）
//  @return :		void
//  @note   :		void
//================================================================//
void FindTenLine(void)
{
	int knum[10] = { 0 };
	for (; FindLine < 10; ++FindLine)
	{
		int tmp_l[100] = { 0 }, tmp_r[100] = { 0 }, MaxWidth = 0;
		int k = 0;
		int LeftFlag = 0, RightFlag = 0;
		for (int i = LEFT_EAGE; i <= RIGHT_EAGE; ++i)
		{
			if (IsEage(FIND_LEFT, FindLine, i, 3, 1))
			{
				tmp_l[k] = i;
				LeftFlag = 1;
			}
			else;

			if (LeftFlag && IsEage(FIND_RIGHT, FindLine, i, 3, 1))
			{
				tmp_r[k] = i;
				RightFlag = 1;
			}

			if (LeftFlag && RightFlag)
			{
				if (tmp_r[k] - tmp_l[k] >= MaxWidth)
				{
					MaxWidth = tmp_r[k] - tmp_l[k];
					LL[FindLine] = tmp_l[k];
					RL[FindLine] = tmp_r[k];
				}
				else;
				k++;
				LeftFlag = 0;
				RightFlag = 0;
			}
			else;
		}
		knum[FindLine] = k;
		//str.Format("\r\n k1 = %d \r\n", k); PrintDebug(str);
		
		//str.Format("\r\n ll = %d \r\n", LL[0]); PrintDebug(str);
		//str.Format("\r\n rl = %d \r\n", RL[0]); PrintDebug(str);
		ImageData[127 - FindLine][LL[FindLine]] = 100;
		ImageData[127 - FindLine][RL[FindLine]] = 128;
	}
	int TmpFlag = IsStopLine(knum);
	StopFillFlag |= TmpFlag;
	//判断刹车
	if (StopFillFlag && !g_stop_dis && !g_stop_line)
		g_stop_dis = 1;
	else;
	//str.Format("\r\n StopFlag = %d \r\n", StopFillFlag); PrintDebug(str);
}

//================================================================//
//  @brief  :		前十行判断类型
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void TenType(void)
{
	if (CircleFlag)
	{
		LeftPnt.ErrType = 10;
		RightPnt.ErrType = 10;
		return;
	}
	//左右丢边
	ThreeFlag = 0;
	int leftlost_num = 0, rightlost_num = 0;	//为边界点的数量
	for (int i = 0; i < 10; i++)
	{
		if (LL[i] == LEFT_EAGE)
			leftlost_num++;
		if (RL[i] == RIGHT_EAGE)
			rightlost_num++;
	}
	//if (StopFillFlag)											//停车标志
	//{
	//	LeftPnt.ErrType = 8;
	//	RightPnt.ErrType = 8;

	//}
	 if (10 == leftlost_num && 10 == rightlost_num)			//全丢线
	{
		FirstRoadType = 4;
		LeftPnt.ErrType = 8;
		RightPnt.ErrType = 8;
	}
	else if (0 == leftlost_num && 0 == rightlost_num)		//全不丢线
	{
		FirstRoadType = 0;
		FindLine = 1;
	}
	else if (10 == leftlost_num && 0 == rightlost_num)		//左边全丢 右边全不丢
	{
		if (-1 == trend(RL, 10))
		{
			FirstRoadType = 3;
			LeftPnt.ErrType = 7;
			RightPnt.ErrRow = CutLine(RL, 0);
			g_trend_flag = -1;
			LeftLost = 0;
			ThreeFlag = 1;
			FindLine = 1;

		}
		else
		{
			FirstRoadType = 4;
			LeftPnt.ErrType = 8;
			RightPnt.ErrType = 8;
		}
	}
	else if (0 == leftlost_num && 10 == rightlost_num)		//右边全丢 左边全不丢
	{
		if (1 == trend(LL, 10))
		{

			FirstRoadType = 3;
			RightPnt.ErrType = 7;
			LeftPnt.ErrRow = CutLine(LL, 0);
			g_trend_flag = 1;
			RightLost = 0;
			ThreeFlag = 2;
			FindLine = 1;
			g_trend_flag = 1;
		}
		else
		{
			FirstRoadType = 4;
			LeftPnt.ErrType = 8;
			RightPnt.ErrType = 8;
		}
	}
	else if (10 == leftlost_num || 10 == rightlost_num)			//一边全丢 一边部分丢
	{
		FirstRoadType = 4;
		LeftPnt.ErrType = 8;
		RightPnt.ErrType = 8;
	}
	else if (0 == leftlost_num)									//左边不丢 右边部分丢
	{
		if (RIGHT_EAGE == RL[9])
		{
			FindLine = 1;
			FirstRoadType = 0;
		}
		else if (RIGHT_EAGE == RL[0])					//返回函数值 或者从最高行开始补线
		{
			FirstRoadType = 6;
			LeftPnt.ErrType = 8;
			RightPnt.ErrType = 8;
			//ErrorFlag = 1;
		}
		else
		{
			FirstRoadType = 6;
			LeftPnt.ErrType = 8;
			RightPnt.ErrType = 8;
		}

	}
	else if (0 == rightlost_num)								//右边不丢 左边部分丢
	{
		if (LEFT_EAGE == LL[9])
		{
			FindLine = 1;
			FirstRoadType = 0;
		}
		else if (LEFT_EAGE == LL[0])				//返回函数值 或者从最高行开始补线
		{
			FirstRoadType = 6;
			LeftPnt.ErrType = 8;
			RightPnt.ErrType = 8;
			//ErrorFlag = 1;
		}
		else
		{
			LeftPnt.ErrType = 8;
			RightPnt.ErrType = 8;
			FirstRoadType = 6;
		}
	}
	else                                                          //两边部分丢
	{
		FirstRoadType = 4;
		LeftPnt.ErrType = 8;
		RightPnt.ErrType = 8;
	}
}

//================================================================//
//  @brief  :		普通寻线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FindLineNormal(void)
{
	IsCross3(0, 0);
	LeftFindFlag = RightFindFlag = 0;
	if (0 == LeftPnt.ErrType && 0 == RightPnt.ErrType)
		FindLine = MIN(LeftPnt.ErrRow, RightPnt.ErrRow);
	else if (5 == LeftPnt.ErrType || 7 == LeftPnt.ErrType)
	{
		FindLine = RightPnt.ErrRow;
		LeftFindFlag = 1;
	}
	else if (6 == RightPnt.ErrType || 7 == RightPnt.ErrType)
	{
		FindLine = LeftPnt.ErrRow;
		RightFindFlag = 1;
	}
	else
		FindLine = MIN(LeftPnt.ErrRow, RightPnt.ErrRow);
	FindLine = MAX(FindLine, 1);
	for (int i = FindLine; i < IMG_ROWS; ++i)
	{
		if (!LeftFindFlag)
		{
			int t = FindL(FIND_LEFT, i, i - 1, AutoValue(i));
			if (0 == t)
			{
				LeftPnt.ErrRow = i - row_n - 1;
				LeftPnt.ErrCol = LL[i - row_n - 1];
				LeftPnt.ErrType = JumpType(i - row_n - 1, LL[i - row_n - 1]);
				LeftFindFlag = 1;
			}
			else if (2 == t || 3 == t)
			{
				LeftPnt.ErrRow = i - row_n;
				LeftPnt.ErrCol = LL[i - row_n];
				if (LeftPnt.ErrCol <= LEFT_EAGE)
					LeftPnt.ErrType = 5;
				else if (LeftPnt.ErrCol >= RIGHT_EAGE)
					LeftPnt.ErrType = 6;
				else
					LeftPnt.ErrType = JumpType(i - row_n - 1, LL[i - row_n - 1]);
				LeftFindFlag = 1;
			}
			if (2 == t && LeftPnt.ErrType == 5)
			{
				if ((ImgBuff[(85 - i + row_n + FRONT_LINE) * 24 + LEFT_EAGE / 8] & Bit[LEFT_EAGE % 8]) == 0x00)
				{
					LeftPnt.ErrType = 2;
					LeftPnt.ErrRow = i - row_n - 1;
					LeftPnt.ErrCol = LL[i - row_n - 1];
				}
			}
			if (RightFindFlag)			//右边先找到特殊点 开始转弯判断
			{
				int a = IsCross3(FIND_LEFT, i);
				if (a)
				{
					LeftPnt.ErrType = 3;
					LeftPnt.ErrRow = ++a;
					LeftPnt.ErrCol = LL[a];
					LeftFindFlag = 1;
				}
			}
		}
		if (!RightFindFlag)
		{
			int s = FindL(FIND_RIGHT, i, i - 1, AutoValue(i));
			//str.Format("\r\n s = %d \r\n", s); PrintDebug(str);
			if (0 == s)
			{
				RightPnt.ErrRow = i - row_n - 1;
				RightPnt.ErrCol = RL[i - row_n - 1];
				RightPnt.ErrType = JumpType(i - row_n - 1, RL[i - row_n - 1]);
				RightFindFlag = 1;
			}
			else if (2 == s || 3 == s)
			{
				RightPnt.ErrRow = i - row_n;
				RightPnt.ErrCol = RL[i - row_n];
				if (RightPnt.ErrCol <= LEFT_EAGE)
					RightPnt.ErrType = 5;
				else if (RightPnt.ErrCol >= RIGHT_EAGE)
					RightPnt.ErrType = 6;
				else
					RightPnt.ErrType = JumpType(i - row_n - 1, RL[i - row_n - 1]);
				RightFindFlag = 1;
			}
			if (2 == s && RightPnt.ErrType == 6)
			{
				if ((ImgBuff[(85 - i + row_n + FRONT_LINE) * 24 + RIGHT_EAGE / 8] & Bit[RIGHT_EAGE % 8]) == 0x00)
				{
					RightPnt.ErrType = 2;
					RightPnt.ErrRow = i - row_n - 1;
					RightPnt.ErrCol = RL[i - row_n - 1];
				}
			}
			if (LeftFindFlag)
			{
				int b = IsCross3(FIND_RIGHT, i);
				if (b)
				{
					RightPnt.ErrType = 3;
					RightPnt.ErrRow = ++b;
					RightPnt.ErrCol = RL[b];
					RightFindFlag = 1;
				}
			}
		}
		if (LeftFindFlag & RightFindFlag)
			return;
	}
	if (!LeftFindFlag)
	{
		LeftPnt.ErrType = 4;
		LeftPnt.ErrRow = IMG_ROWS - 1;
		LeftPnt.ErrCol = LL[IMG_ROWS - 1];
	}
	if (!RightFindFlag)
	{
		RightPnt.ErrType = 4;
		RightPnt.ErrRow = IMG_ROWS - 1;
		RightPnt.ErrCol = RL[IMG_ROWS - 1];
	}
}

//================================================================//
//  @brief  :		一边丢边寻线
//  @param  :		寻线行（line）
//  @return :		-1错误 0未找到线 1找到边缘 2图片边界
//  @note   :		void
//================================================================//
void FindLineLost(void)
{
	LeftFindFlag = RightFindFlag = 0;
	int EageN = 0, StartLine = IMG_ROWS;
	str.Format("\r\n RightPnt.ow = %d \r\n", RightPnt.ErrRow); PrintDebug(str);
	if (7 == LeftPnt.ErrType)			//左丢边
	{
		LeftPnt.ErrType = 5;
		int TmpLine = RightPnt.ErrRow;
		for (int i = RightPnt.ErrRow + 1; i < IMG_ROWS; ++i)
		{
			//右边寻线
			if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
			{
				RightPnt.ErrRow = i - row_n - 1;
				RightPnt.ErrCol = RL[i - row_n - 1];
				RightPnt.ErrType = JumpType(i - row_n - 1, RL[i - row_n - 1]);
				RightFindFlag = 1;
			}
			int a = IsCross3(FIND_RIGHT, i);
			if (a)
			{
				RightPnt.ErrType = 3;
				RightPnt.ErrRow = ++a;
				RightPnt.ErrCol = RL[a];
				RightFindFlag = 1;

			}

			//左边横向扫描
			int j;
			for (j = RL[i - 1]; j >= LEFT_EAGE; --j)
			{
				if (1 == IsEage(FIND_LEFT, i, j, 4, 2))
				{
					LL[i] = j;
					StartLine = MIN(StartLine, i);
					++EageN;
					break;
				}
			}
			if (LEFT_EAGE - 1 == j)
			{
				EageN = 0;
				StartLine = IMG_ROWS;
			}
			if (10 == EageN)
			{
				if (ArrayMin(&LL[StartLine], 10) > LEFT_EAGE + 30)
				{
					LeftPnt.ErrType = 0;
					LeftPnt.ErrRow = i;
					LeftPnt.ErrCol = LL[i];
					LeftFindFlag = 1;
					FillLineDown(FIND_LEFT, StartLine);
				}
				else LeftFindFlag = 1;
			}

			if (RightFindFlag)
			{
				if (TmpLine)
					FillLineDownK(FIND_RIGHT, TmpLine, MIN(TmpLine + 3, i));
				return;
			}
			else if (LeftFindFlag)
			{
				if (TmpLine)
					FillLineDownK(FIND_RIGHT, TmpLine, MIN(TmpLine + 3, i));
				RightPnt.ErrType = 0;
				RightPnt.ErrRow = i;
				RightPnt.ErrCol = RL[i];
				return;
			}
		}
		RightPnt.ErrType = 4;
		RightPnt.ErrRow = IMG_ROWS - 1;
		RightPnt.ErrCol = RL[IMG_ROWS - 1];
	}
	else if (7 == RightPnt.ErrType)			//右丢边
	{
		RightPnt.ErrType = 6;
		int TmpLine = LeftPnt.ErrRow;
		for (int i = LeftPnt.ErrRow + 1; i < IMG_ROWS; ++i)
		{
			if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
			{
				LeftPnt.ErrRow = i - row_n - 1;
				LeftPnt.ErrCol = LL[i - row_n - 1];
				LeftPnt.ErrType = JumpType(i - row_n - 1, LL[i - row_n - 1]);
				LeftFindFlag = 1;
			}
			int a = IsCross3(FIND_LEFT, i);
			if (a)
			{
				LeftPnt.ErrType = 3;
				LeftPnt.ErrRow = ++a;
				LeftPnt.ErrCol = LL[a];
				LeftFindFlag = 1;
			}

			//右边横向扫描
			int j;
			for (j = LL[i - 1]; j <= RIGHT_EAGE; ++j)
			{
				if (1 == IsEage(FIND_RIGHT, i, j, 4, 2))
				{
					RL[i] = j;
					StartLine = MIN(StartLine, i);
					++EageN;
					break;
				}
			}
			if (RIGHT_EAGE + 1 == j)
			{
				EageN = 0;
				StartLine = IMG_ROWS;
			}
			if (10 == EageN)
			{
				if (ArrayMax(&RL[StartLine], 10) < RIGHT_EAGE - 30)
				{
					RightPnt.ErrType = 0;
					RightPnt.ErrRow = i;
					RightPnt.ErrCol = RL[i];
					RightFindFlag = 1;
					FillLineDown(FIND_RIGHT, StartLine);
				}
				else RightFindFlag = 1;
			}
			if (LeftFindFlag)
			{
				if (TmpLine)
					FillLineDownK(FIND_LEFT, TmpLine, MIN(TmpLine + 3, i));
				return;
			}
			else if (RightFindFlag)
			{
				if (TmpLine)
					FillLineDownK(FIND_LEFT, TmpLine, MIN(TmpLine + 3, i));
				LeftPnt.ErrType = 0;
				LeftPnt.ErrRow = i;
				LeftPnt.ErrCol = LL[i];
				return;
			}
		}
		LeftPnt.ErrType = 4;
		LeftPnt.ErrRow = IMG_ROWS - 1;
		LeftPnt.ErrCol = LL[IMG_ROWS - 1];
	}
	else;
}

//================================================================//
//  @brief  :		平十字找线
//  @param  :		寻线行（line）开始列（col） 结束列（end_line）
//  @return :		-1错误 0未找到 else 行数
//  @note   :		void
//================================================================//
int FindCross_1(int line, int col, int end_col)
{
	if (col < LEFT_EAGE || col > RIGHT_EAGE || end_col < LEFT_EAGE || end_col > RIGHT_EAGE || line < 0 || line > IMG_ROWS)
		return -1;

	++line; ++line;

	int TmpLeftCol = 0, TmpRightCol = 0;
	for (int i = line; i < IMG_ROWS; ++i)
	{
		int FlagL = 0, FlagR = 0;
		int j = col;
		for (; j <= end_col; ++j)
		{
			int LE = IsEage(FIND_LEFT, i, j, 4, 1);
			if (1 == LE)
			{
				TmpLeftCol = j;
				//LL[i] = j;
				FlagL = 1;
				break;
			}
		}
		for (; j <= end_col; ++j)
		{
			int RE = IsEage(FIND_RIGHT, i, j, 4, 1);
			if (1 == RE)
			{
				TmpRightCol = j;
				//RL[i] = j;
				FlagR = 1;
				break;
			}
		}
		if (FlagL & FlagR)
		{
			LL[i] = TmpLeftCol;
			RL[i] = TmpRightCol;
			int k;
			for (k = 0; k < 10; ++k)
			{
				if (1 != FindL(FIND_LEFT, i + k + 1, i + k, AutoValue(i + k + 1)))
					break;
				if (1 != FindL(FIND_RIGHT, i + k + 1, i + k, AutoValue(i + k + 1)))
					break;
			}
			if (10 == k)
				return i;
			else continue;
		}
	}
	return 0;

}

//================================================================//
//  @brief  :		平十字快速确定行
//  @param  :		寻线行（line）左边界（LeftEage）右边界（RightEage） 偏置（offset）
//  @return :		-1错误 else 行数
//  @note   :		void
//================================================================//
int FindCrossLine_1(int line, int LeftEage, int RightEage, int offset)
{
	int LeftColEage = IMG_ROWS, RightColEage = IMG_ROWS, LeftColEage_2 = IMG_ROWS, RightColEage_2 = IMG_ROWS;
	for (int i = line; i < IMG_ROWS; ++i)
	{
		if (IMG_ROWS == RightColEage && IsEageCol(i, RightEage, 3, 0))
		{
			RightColEage = i;
			//str.Format("\r\n RightColEage = %d \r\n", i); PrintDebug(str);
		}
		if (IMG_ROWS == LeftColEage && IsEageCol(i, LeftEage, 3, 0))
		{
			LeftColEage = i;
			//str.Format("\r\n LeftColEage = %d \r\n", i); PrintDebug(str);
		}

		if (offset)
		{
			if (IMG_ROWS == RightColEage_2 && IsEageCol(i, RightEage - offset, 3, 0))
			{
				RightColEage_2 = i;
				//str.Format("\r\n RightColEage_2 = %d \r\n", i); PrintDebug(str);
			}
			if (IMG_ROWS == LeftColEage_2 && IsEageCol(i, LeftEage + offset, 3, 0))
			{
				LeftColEage_2 = i;
				//str.Format("\r\n LeftColEage_2 = %d \r\n", i); PrintDebug(str);
			}
			if (IMG_ROWS != LeftColEage && IMG_ROWS != RightColEage && IMG_ROWS != LeftColEage_2 && IMG_ROWS != RightColEage_2 || IMG_ROWS - 1 == i)
			{
				int tmp_min_1 = MIN(LeftColEage, RightColEage);
				int tmp_min_2 = MIN(LeftColEage_2, RightColEage_2);
				return MIN(tmp_min_1, tmp_min_2);
			}
		}
		else if (IMG_ROWS != LeftColEage && IMG_ROWS != RightColEage || IMG_ROWS - 1 == i)
			return MIN(LeftColEage, RightColEage);
	}
}

//================================================================//
//  @brief  :		斜十字找线
//  @param  :		寻线行（line）
//  @return :		0未找到 else找到的线行数
//  @note   :		void
//================================================================//
int FindCross_2(int type)
{
	if (FIND_LEFT == type)			//左斜十字
	{
		int MaxLine = IMG_ROWS;
		for (int i = RightPnt.ErrRow + 3; i < IMG_ROWS; ++i)
		{
			if (IsEageCol(i, RightPnt.ErrCol, 4, 1))
			{
				MaxLine = i;
				break;
			}
		}
		for (int i = RightPnt.ErrRow + 3; i <= MaxLine; ++i)
		{
			for (int j = RightPnt.ErrCol; j > (MAX(LEFT_EAGE, RightPnt.ErrCol - 100)); --j)
			{
				int a = IsEage(FIND_RIGHT, i, j, 4, 2);
				if (1 == a)
				{
					RL[i] = j;
					str.Format("\r\n i = %d \r\n", i); PrintDebug(str);
					str.Format("\r\n j = %d \r\n", j); PrintDebug(str);
					int k, b;
					for (k = 0; k < 10; ++k)
					{
						b = FindL(FIND_RIGHT, i + k + 1, i + k, AutoValue(i + k + 1));
						if (1 != b)
							break;
					}
					str.Format("\r\n k = %d \r\n", k); PrintDebug(str);
					if (1 == a)
					{
						if (2 == b || 3 == b)
							return i;
						else if (10 == k)
							return i;
						else break;
					}
				}
			}
		}
	}
	else if (FIND_RIGHT == type)		//右斜十字 右转
	{
		int MaxLine = IMG_ROWS;
		for (int i = LeftPnt.ErrRow + 3; i < IMG_ROWS; ++i)
		{
			if (IsEageCol(i, LeftPnt.ErrCol, 4, 1))
			{
				MaxLine = i;
				break;
			}
		}
		for (int i = LeftPnt.ErrRow + 3; i < MaxLine; ++i)
		{
			for (int j = LeftPnt.ErrCol; j < (MIN(RIGHT_EAGE, LeftPnt.ErrCol + 100)); ++j)
			{
				int a = IsEage(FIND_LEFT, i, j, 4, 2);
				if (1 == a)
				{
					LL[i] = j;
					int k, b;
					for (k = 0; k < 10; ++k)
					{
						b = FindL(FIND_LEFT, i + k + 1, i + k, AutoValue(i + k + 1));
						if (1 != b)
							break;
					}
					if (1 == a)
					{
						if (2 == b || 3 == b)
							return i;
						else if (10 == k)
							return i;
						else break;
					}
				}
			}
		}
	}
	return 0;
}