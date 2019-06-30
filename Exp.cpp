#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "Exp.h"

//首行搜边
void GetFirstL(void)
{
	int mid = 94;
	FindLineType = 1;
	int LLF = SearchLeftEage(DOWN_EAGE, mid);
	int RLF = SearchRightEage(DOWN_EAGE, mid);
	string.Format("\r\n LL = %d \r\n", LLF); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", RLF); PrintDebug(string);
	FindLineType = 0;
	for (int i = 0; i < IMG_ROW; i++)
	{
		ImageData[i][LLF] = 100;
		ImageData[i][RLF] = 128;
	}
}

void Exp(void)
{
	//GetFirstL();
	VarInit();
	FirstLineV4();
	//FindLineNormalNew(0);

	string.Format("\r\n LL = %d \r\n", LL[DOWN_EAGE]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", RL[DOWN_EAGE]); PrintDebug(string);
	for (int i = 0; i < IMG_ROW; i++)
	{
		ImageData[i][LL[70]] = 254;
		ImageData[i][RL[70]] = 254;
	}
	string.Format("\r\n RL = %d \r\n", ImageEage[DOWN_EAGE][165]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", ImageEage[DOWN_EAGE][166]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", ImageEage[DOWN_EAGE][167]); PrintDebug(string);
}

unsigned char IndJudgeCircle(unsigned char type)
{
	return 1;
}

//================================================================//
//  @brief  :		搜左边线new
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int GetLL_1(int row, int col)
{
	int TmpCol;
	col += 8;
	if (col > RIGHT_EAGE) col = RIGHT_EAGE;
	if (!IsEage(row, col))
		return SearchLeftEage(row, col);
	else
		return SearchRightNoEage(row, col) - 1;
}

//================================================================//
//  @brief  :		搜右边线new
//  @param  :		void
//  @return :		边界点所在列
//  @note   :		void
//================================================================//
int GetRL_1(int row, int col)
{
	int TmpCol;
	col -= 8;
	if (col <= LEFT_EAGE) col = LEFT_EAGE + 1;
	if (!IsEage(row, col))
		return SearchRightEage(row, col);
	else
		return SearchLeftNoEage(row, col) + 1;
}
//================================================================//
//  @brief  :		普通寻线新版
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FindLineNormal_1(int Fill)
{
	int LeftFindFlag = 0, RightFindFlag = 0;
	int StartRow = MAX(LeftPnt.ErrRow, RightPnt.ErrRow) - 1;
	if (0 != LeftPnt.Type)
	{
		LeftFindFlag = 1;
		StartRow = RightPnt.ErrRow - 1;
	}
	if (0 != RightPnt.Type)
	{
		RightFindFlag = 1;
		StartRow = LeftPnt.ErrRow - 1;
	}
	int row_i;
	for (row_i = StartRow; row_i > UP_EAGE; row_i--)
	{
		if (!LeftFindFlag)
		{
			if (row_i >= LeftPnt.ErrRow)
				;
			else
			{
				LL[row_i] = GetLL_1(row_i, LL[row_i + 1]);
				if (LL[row_i] - LL[row_i + 1] > 8 && !RightFindFlag)
				{
					LL[row_i] = SearchLeftNoEage(row_i, LL[row_i] + 3);
					LeftIntLine = MAX(LeftIntLine, row_i);
				}
				else if (LL[row_i] - LL[row_i + 1] > 8
					|| LL[row_i + 1] - LL[row_i] > 8
					|| LEFT_EAGE + 3 >= LL[row_i])
				{
					LeftPnt.ErrRow = row_i + 1;
					LeftPnt.ErrCol = LL[row_i + 1];
					LeftFindFlag = 1;
					if (RightPnt.Type == 0)
						LeftPnt.Type = 2;
					else LeftPnt.Type = 1;
				}
			}
		}
		if (!RightFindFlag)
		{
			if (row_i >= RightPnt.ErrRow)
				;
			else
			{
				RL[row_i] = GetRL_1(row_i, RL[row_i + 1]);
				if (RL[row_i + 1] - RL[row_i] > 8 && !LeftFindFlag)
				{
					RL[row_i] = SearchRightNoEage(row_i, RL[row_i] - 3);
					RightIntLine = MAX(RightIntLine, row_i);
				}
				else if (RL[row_i] - RL[row_i + 1] > 8
					|| RL[row_i + 1] - RL[row_i] > 8
					|| RIGHT_EAGE - 3 <= RL[row_i])
				{
					RightPnt.ErrRow = row_i + 1;
					RightPnt.ErrCol = RL[row_i + 1];
					RightFindFlag = 1;
					if (LeftPnt.Type == 0)
						RightPnt.Type = 2;
					else RightPnt.Type = 1;
				}
			}
		}
	}
	if (UP_EAGE == row_i)
	{
		if (!LeftFindFlag)
		{
			LeftPnt.Type = 4;
			LeftPnt.ErrRow = UP_EAGE + 1;
			LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
		}
		if (!RightFindFlag)
		{
			RightPnt.Type = 4;
			RightPnt.ErrRow = UP_EAGE + 1;
			RightPnt.ErrCol = RL[RightPnt.ErrRow];
		}
	}
	string.Format("\r\n 0x02LeftPnt = %d %d %d \r\n", LeftPnt.Type, LeftPnt.ErrRow, LeftPnt.ErrCol); PrintDebug(string);
	string.Format("\r\n 0x02RightPnt = %d %d %d \r\n", RightPnt.Type, RightPnt.ErrRow, RightPnt.ErrCol); PrintDebug(string);
	if (Fill)
	{
		if (LeftPnt.Type == 2 && RightPnt.ErrRow < LeftPnt.ErrRow)
		{
			int TmpRow;
			for (int i = LeftPnt.ErrRow; i > RightPnt.ErrRow; i--)
			{
				TmpRow = SearchUpEage(i, RL[i] - 1);
				if (i - TmpRow > 4)
				{
					RightPnt.Type = 2;
					RightPnt.ErrRow = i;
					RightPnt.ErrCol = RL[i];
					break;
				}
				else if (TmpRow == UP_EAGE)
				{
					RightPnt.Type = 1;
					RightPnt.ErrRow = i;
					RightPnt.ErrCol = RL[i];
					break;
				}
			}
		}
		else if (RightPnt.Type == 2 && LeftPnt.Type < RightPnt.ErrRow)
		{
			int TmpRow;
			for (int i = RightPnt.ErrRow; i >= LeftPnt.ErrRow; i--)
			{
				TmpRow = SearchUpEage(i, LL[i] + 1);
				if (i - TmpRow > 4)
				{
					LeftPnt.Type = 2;
					LeftPnt.ErrRow = i;
					LeftPnt.ErrCol = LL[i];
					break;
				}
				else if (TmpRow == UP_EAGE)
				{
					LeftPnt.Type = 1;
					LeftPnt.ErrRow = i;
					LeftPnt.ErrCol = LL[i];
					break;
				}
			}
		}
	}
}

//================================================================//
//  @brief  :		单边丢寻线新版
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FindLineLost_1(void)
{
	if (!RightPnt.Type)
	{
		LeftPnt.Type = 2;
		int row_i;
		for (row_i = DOWN_EAGE - 1; row_i > UP_EAGE; row_i--)
		{
			RL[row_i] = GetRL_1(row_i, RL[row_i + 1]);
			if (RL[row_i] - RL[row_i + 1] > 8
				|| RL[row_i + 1] - RL[row_i] > 8
				|| RIGHT_EAGE == RL[row_i])
			{
				RightPnt.ErrRow = row_i + 1;
				RightPnt.ErrCol = RL[row_i + 1];
				RightPnt.Type = 1;
				break;
			}
		}
		if (UP_EAGE == row_i)
		{
			RightPnt.ErrRow = UP_EAGE + 1;
			RightPnt.Type = 4;
			RightPnt.ErrCol = RL[RightPnt.ErrRow];
		}
		//对拐点的判断
		for (int i = LeftPnt.ErrRow; i > RightPnt.ErrRow; --i)
		{
			int TmpRow = SearchUpEage(i, RL[i] - 1);
			if (i - TmpRow > 5)
			{
				RightPnt.Type = 2;
				RightPnt.ErrRow = i;
				RightPnt.ErrCol = RL[RightPnt.ErrRow];
				break;
			}
			else if (TmpRow == UP_EAGE)
			{
				RightPnt.Type = 1;
				RightPnt.ErrRow = i;
				RightPnt.ErrCol = RL[RightPnt.ErrRow];
				break;
			}
		}
		//对双侧十字的判断	跳变点的列数不合常理 则认为是双侧十字
		if (RightPnt.ErrCol > RIGHT_EAGE - 40)
		{
			g_RoadType = 2;
			return;
		}
	}
	else if (!LeftPnt.Type)
	{
		RightPnt.Type = 2;
		int row_i;
		for (row_i = DOWN_EAGE - 1; row_i > UP_EAGE; row_i--)
		{
			LL[row_i] = GetLL_1(row_i, LL[row_i + 1]);
			if (LL[row_i] - LL[row_i + 1] > 8
				|| LL[row_i + 1] - LL[row_i] > 8
				|| LEFT_EAGE == LL[row_i])
			{
				LeftPnt.ErrRow = row_i + 1;
				LeftPnt.ErrCol = LL[row_i + 1];
				LeftPnt.Type = 1;
				break;
			}
		}
		if (UP_EAGE == row_i)
		{
			LeftPnt.Type = 4;
			LeftPnt.ErrRow = UP_EAGE + 1;
			LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
		}
		//对拐点的判断
		for (int i = RightPnt.ErrRow; i > LeftPnt.ErrRow; --i)
		{
			int TmpRow = SearchUpEage(i, LL[i] + 1);
			if (i - TmpRow > 5)
			{
				LeftPnt.Type = 2;
				LeftPnt.ErrRow = i;
				LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
				break;
			}
			else if (TmpRow == UP_EAGE)
			{
				LeftPnt.Type = 1;
				LeftPnt.ErrRow = i;
				LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
				break;
			}
		}
		//对双侧十字的判断	跳变点的列数不合常理 则认为是双侧十字
		if (LeftPnt.ErrCol < LEFT_EAGE + 40)
		{
			g_RoadType = 2;
			return;
		}
	}
	string.Format("\r\n 0x01LeftPnt = %d %d %d \r\n", LeftPnt.Type, LeftPnt.ErrRow, LeftPnt.ErrCol); PrintDebug(string);
	string.Format("\r\n 0x01RightPnt = %d %d %d \r\n", RightPnt.Type, RightPnt.ErrRow, RightPnt.ErrCol); PrintDebug(string);

}

//================================================================//
//  @brief  :		延距清环岛7状态标志
//  @param  :		void
//  @return :		void
//  @note   :		放在中断
//================================================================//
unsigned char Dist_ClearIslandSeven(void)
{
	return 1;
}