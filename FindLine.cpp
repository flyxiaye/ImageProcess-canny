#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FillSpecialLine.h"
#include "FirstLineProcess.h"
#include "FindLine.h"
#include "canny.h"


#define UP_TH 15		//向上搜边缘行数
#define LOST_TH 5	//首行丢边阈值	
#define JUMP_TH 10		//判断是否为真丢边跳变阈值
#define CROSSUP 4	//拐点向上阈值
#define CROSSUP_TH 5		//单丢边判断拐点跳变阈值
#define LEFT_PNT(row, type)	LeftPnt.ErrRow = (row); LeftPnt.ErrCol = LL[LeftPnt.ErrRow]; LeftPnt.Type = (type)
#define RIGHT_PNT(row, type) RightPnt.ErrRow = (row); RightPnt.ErrCol = RL[RightPnt.ErrRow]; RightPnt.Type = (type)
//================================================================//
//  @brief  :		首行搜线版本4(现用)
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FirstLineV4(void)
{
	int temp_1 = 0, temp_2 = 0;
	int mid = (LEFT_EAGE + RIGHT_EAGE) >> 1;
	for (int i = 1; i < 34; i += 4)
	{
		temp_1 += ImageData[DOWN_EAGE][mid - i];
		temp_1 += ImageData[DOWN_EAGE - 2][mid - i];
		temp_2 += ImageData[DOWN_EAGE][mid + i];
		temp_2 += ImageData[DOWN_EAGE - 2][mid + i];
	}
	temp_1 = temp_1 >> 4;
	temp_2 = temp_2 >> 4;
	FindLineType = 1;
	if (temp_1 >= LightThreshold && temp_2 >= LightThreshold)
	{
		LL[DOWN_EAGE] = SearchLeftEage(DOWN_EAGE, mid);
		RL[DOWN_EAGE] = SearchRightEage(DOWN_EAGE, mid);
	}
	else if (temp_1 >= temp_2)
	{
		LL[DOWN_EAGE] = SearchLeftEage(DOWN_EAGE, mid - 40);
		RL[DOWN_EAGE] = SearchRightEage(DOWN_EAGE, mid - 40);
	}
	else
	{
		LL[DOWN_EAGE] = SearchLeftEage(DOWN_EAGE, mid + 40);
		RL[DOWN_EAGE] = SearchRightEage(DOWN_EAGE, mid + 40);
	}
	FindLineType = 0;
	string.Format("\r\n LLDE = %d \r\n", LL[DOWN_EAGE]); PrintDebug(string);
	string.Format("\r\n RLDE = %d \r\n", RL[DOWN_EAGE]); PrintDebug(string);
}
//================================================================//
//  @brief  :		首行情况处理
//  @param  :		LL[DOWN_EAGE] RL[DOWN_EAGE]
//  @return :		RoadType LeftPnt RightPnt
//  @note   :		丢边为5、开区间
//================================================================//
int FirstRowProcess(void)
{
	int RoadType = 0;
	int Middle = (LL[DOWN_EAGE] + RL[DOWN_EAGE]) >> 1;

	if (LEFT_EAGE + LOST_TH > LL[DOWN_EAGE] && RIGHT_EAGE - LOST_TH < RL[DOWN_EAGE])	//两侧丢边 判定为双侧十字
	{
		LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
		LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
		RightPnt.ErrCol = RL[RightPnt.ErrRow];
		RoadType = 2;
	}
	else if (LEFT_EAGE + LOST_TH > LL[DOWN_EAGE])	//左侧丢边
	{
		unsigned char LeftLostFlag = 0, RightLostFlag = 0;   //the flag of lost eage
		//First detect
		//Lost Line Process
		int i;
		int NoLostFlag = 0;
		FindLineType = 1;
		for (i = DOWN_EAGE - 1; i > DOWN_EAGE - UP_TH; --i)
		{
			LL[i] = SearchLeftEage(i, Middle);
			if (i < DOWN_EAGE - 3)
			{
				if (LL[i] - LL[i + 4] > JUMP_TH
					&& LL[i + 1] - LL[i + 4] > JUMP_TH
					&& LL[i + 2] - LL[i + 4] > JUMP_TH
					&& LL[i + 3] - LL[i + 4] > JUMP_TH)		//非丢边 向下补线
				{
					string.Format("\r\n i = %d \r\n", i); PrintDebug(string);
					FillLineDown(LL, i + 3, i);
					LeftPnt.ErrRow = i;
					LeftPnt.Type = 0;
					LeftPnt.ErrCol = LL[i];
					NoLostFlag = i;
					break;
				}
			}
			if (LL[i] >= LEFT_EAGE + LOST_TH)
			{
				NoLostFlag = MAX(NoLostFlag, i);
			}
		}
		FindLineType = 0;
		if (DOWN_EAGE - UP_TH == i)
		{
			if (NoLostFlag)
			{
				LeftPnt.ErrRow = NoLostFlag;
				LeftPnt.ErrCol = LL[NoLostFlag];
				LeftPnt.Type = 0;
				LL[NoLostFlag - 1] = GetLL(NoLostFlag - 1, LL[NoLostFlag]);
				LL[NoLostFlag - 2] = GetLL(NoLostFlag - 2, LL[NoLostFlag - 1]);
				LL[NoLostFlag - 3] = GetLL(NoLostFlag - 3, LL[NoLostFlag - 2]);
				FillLineDown(LL, NoLostFlag, NoLostFlag - 3);
			}
			else
			{
				LeftPnt.ErrRow = DOWN_EAGE;
				LeftPnt.ErrCol = LL[DOWN_EAGE];
				LeftPnt.Type = 2;
				LeftLostFlag = 1;
			}
		}
		//Not Lost Line Process
		for (i = DOWN_EAGE - 1; i > DOWN_EAGE - UP_TH; i--)
		{
			RL[i] = GetRL(i, RL[i + 1]);
			if (RL[i] - RL[i + 1] > FINDLINE_TH || RL[i + 1] - RL[i] > FINDLINE_TH)		//Jump
			{
				RightPnt.ErrRow = DOWN_EAGE;
				RightPnt.ErrCol = RL[DOWN_EAGE];
				RightPnt.Type = 0;
				break;
			}
			if (RL[i] > RIGHT_EAGE - LOST_TH)		//Lost line
			{
				if (NoLostFlag)			//other side is no lost
				{
					RightPnt.ErrRow = DOWN_EAGE;
					RightPnt.ErrCol = RL[DOWN_EAGE];
					RightPnt.Type = 0;
				}
				else					//other side is lost, is RoadType2
				{
					RightLostFlag = 1;
					RightPnt.ErrRow = i;
					RightPnt.ErrCol = RL[i];
					RightPnt.Type = 0;
				}
				break;
			}
		}
		if (DOWN_EAGE - UP_TH == i)
		{
			if (1 == TrendArray(&RL[DOWN_EAGE], 10) && LeftLostFlag)
			{
				RightLostFlag = 1;
			}
			RightPnt.ErrRow = DOWN_EAGE;
			RightPnt.ErrCol = RL[DOWN_EAGE];
			RightPnt.Type = 0;
		}
		//judge roadtype
		if (LeftLostFlag & RightLostFlag)
			RoadType = 2;
		else if (LeftLostFlag)//Second detect
			RoadType = 1;
		else if (RightLostFlag)
			RoadType = 1;
		else RoadType = 0;
	}
	else if (RIGHT_EAGE - LOST_TH < RL[DOWN_EAGE])		//右侧丢边
	{
		unsigned char LeftLostFlag = 0, RightLostFlag = 0;  //the flag of lost eage
		//Lost Line Process
		int i;
		int NoLostFlag = 0;
		FindLineType = 1;
		for (i = DOWN_EAGE - 1; i > DOWN_EAGE - UP_TH; --i)
		{
			RL[i] = SearchRightEage(i, Middle);
			if (i < DOWN_EAGE - 3)
			{
				if (RL[i + 4] - RL[i] > JUMP_TH
					&& RL[i + 4] - RL[i + 1] > JUMP_TH
					&& RL[i + 4] - RL[i + 2] > JUMP_TH
					&& RL[i + 4] - RL[i + 3] > JUMP_TH)		//非丢边 向下补线
				{
					FillLineDown(RL, i + 3, i);
					RightPnt.ErrRow = i;
					RightPnt.Type = 0;
					RightPnt.ErrCol = RL[i];
					break;
				}
			}
			if (RL[i] <= RIGHT_EAGE - LOST_TH)
			{

				NoLostFlag = MAX(NoLostFlag, i);
			}
		}
		FindLineType = 0;
		if (DOWN_EAGE - UP_TH == i)
		{
			if (NoLostFlag)
			{
				RightPnt.ErrRow = NoLostFlag;
				RightPnt.ErrCol = RL[NoLostFlag];
				RightPnt.Type = 0;
				RL[NoLostFlag - 1] = GetRL(NoLostFlag - 1, RL[NoLostFlag]);
				RL[NoLostFlag - 2] = GetRL(NoLostFlag - 2, RL[NoLostFlag - 1]);
				RL[NoLostFlag - 3] = GetRL(NoLostFlag - 3, RL[NoLostFlag - 2]);
				FillLineDown(RL, NoLostFlag, NoLostFlag - 3);
			}
			else
			{
				RightPnt.ErrRow = DOWN_EAGE;
				RightPnt.Type = 2;
				RightPnt.ErrCol = RL[DOWN_EAGE];
				RightLostFlag = 1;
			}
		}
		//Not Lost Line Process
		for (i = DOWN_EAGE - 1; i > DOWN_EAGE - UP_TH; i--)
		{
			LL[i] = GetLL(i, LL[i + 1]);
			if (LL[i] - LL[i + 1] > FINDLINE_TH || LL[i + 1] - LL[i] > FINDLINE_TH)		//Jump
			{
				LeftPnt.ErrRow = DOWN_EAGE;
				LeftPnt.ErrCol = LL[DOWN_EAGE];
				LeftPnt.Type = 0;
				break;
			}
			if (LL[i] < LEFT_EAGE + LOST_TH)		//Lost line
			{
				if (NoLostFlag)			//other side is no lost
				{
					LeftPnt.ErrRow = DOWN_EAGE;
					LeftPnt.ErrCol = LL[DOWN_EAGE];
					LeftPnt.Type = 0;
				}
				else					//other side is lost, is RoadType2
				{
					LeftLostFlag = 1;
					LeftPnt.Type = 0;
					LeftPnt.ErrRow = i;
					LeftPnt.ErrCol = LL[i];
				}
				break;
			}
		}
		if (DOWN_EAGE - UP_TH == i)
		{
			if (-1 == TrendArray(&LL[DOWN_EAGE], 10) && RightLostFlag)
			{
				LeftLostFlag = 1;
			}
			LeftPnt.ErrRow = DOWN_EAGE;
			LeftPnt.ErrCol = LL[DOWN_EAGE];
			LeftPnt.Type = 0;
		}
		//judge roadtype
		if (LeftLostFlag & RightLostFlag)
			RoadType = 2;
		else if (LeftLostFlag | RightLostFlag)
			RoadType = 1;
		else RoadType = 0;
	}
	else			//两侧都不丢
	{
		LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
		LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
		RightPnt.ErrCol = RL[RightPnt.ErrRow];
		LeftPnt.Type = RightPnt.Type = 0;
		RoadType = 0;
		int i;
		int LeftFlag = 0, RightFlag = 0;
		for (i = DOWN_EAGE - 1; i > DOWN_EAGE - UP_TH; --i)
		{
			if (!LeftFlag)
				LL[i] = GetLL(i, LL[i + 1]);
			if (!RightFlag)
				RL[i] = GetRL(i, RL[i + 1]);
			if (LEFT_EAGE + LOST_TH > LL[i])
			{
				LeftFlag = 1;
				LeftPnt.ErrRow = MIN(i + 2, DOWN_EAGE);
				LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
				LeftPnt.Type = 0;
			}
			if (RIGHT_EAGE - LOST_TH < RL[i])
			{
				RightFlag = 1;
				RightPnt.ErrRow = MIN(i + 2, DOWN_EAGE);
				RightPnt.ErrCol = RL[RightPnt.ErrRow];
				RightPnt.Type = 0;
			}
			if (LeftFlag & RightFlag)
			{
				RoadType = 2;
				break;
			}
		}
		if (DOWN_EAGE - UP_TH == i)
		{
			LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
			LeftPnt.Type = RightPnt.Type = 0;
			LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
			RightPnt.ErrCol = RL[RightPnt.ErrRow];
			RoadType = 0;
		}
	}
	return RoadType;
}

//================================================================//
//  @brief  :		普通寻线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FindLineNormal(int Fill)
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
	int i;
	for (i = StartRow; i > UP_EAGE; --i)
	{
		if (!LeftFindFlag)				//左边未找到特殊点
		{
			if (i >= LeftPnt.ErrRow)		//左边已有线 则不找
				;
			else
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] - LL[i + 1] > FINDLINE_TH && !RightFindFlag)
				{
					LL[i] = SearchLeftNoEage(i, LL[i]) + 3;
					LeftIntLine = MAX(LeftIntLine, i);
				}
				else if (LL[i] - LL[i + 1] > FINDLINE_TH || LL[i + 1] - LL[i] > FINDLINE_TH || LEFT_EAGE + 3 >= LL[i])
				{
					LeftPnt.ErrRow = i + 1;
					LeftPnt.ErrCol = LL[i + 1];
					LeftFindFlag = 1;
					if (RightPnt.Type == 0)
						LeftPnt.Type = 2;
					else LeftPnt.Type = 1;
				}
			}
		}
		if (!RightFindFlag)
		{
			if (i >= RightPnt.ErrRow)		//右边已有线 则不找
				;
			else
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i + 1] - RL[i] > FINDLINE_TH && !LeftFindFlag)
				{
					RL[i] = SearchRightNoEage(i, RL[i]) - 3;
					RightIntLine = MAX(RightIntLine, i);
				}
				else if (RL[i] - RL[i + 1] > FINDLINE_TH || RL[i + 1] - RL[i] > FINDLINE_TH || RIGHT_EAGE - 3 <= RL[i])
				{
					RightPnt.ErrRow = i + 1;
					RightPnt.ErrCol = RL[i + 1];
					RightFindFlag = 1;
					if (LeftPnt.Type == 0)
						RightPnt.Type = 2;
					else RightPnt.Type = 1;
				}
			}
		}
	}
	if (UP_EAGE == i)
	{
		if (!LeftFindFlag)		//左线未找到 则认为是上边界
		{
			LeftPnt.Type = 4;
			LeftPnt.ErrRow = UP_EAGE + 1;
			LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
		}
		if (!RightFindFlag)		//右线未找到 则认为是上边界
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
		//对拐点的判断
		if (LeftPnt.Type == 2 && RightPnt.ErrRow < LeftPnt.ErrRow)	//左外跳 左比右低
		{
			int TmpRow;
			for (int i = LeftPnt.ErrRow; i >= RightPnt.ErrRow; --i)
			{
				TmpRow = SearchUpEage(i, RL[i] - 1);
				if (i - TmpRow > CROSSUP)		//是拐点
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
		else if (RightPnt.Type == 2 && LeftPnt.ErrRow < RightPnt.ErrRow) //右外跳 右比左低
		{
			int TmpRow;
			for (int i = RightPnt.ErrRow; i >= LeftPnt.ErrRow; --i)
			{
				TmpRow = SearchUpEage(i, LL[i] + 1);
				if (i - TmpRow > CROSSUP)
				{
					string.Format("\r\n i = %d \r\n", i); PrintDebug(string);
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
	//对双侧十字的判断 左右特殊点之差不合常理 则认为是双侧十字


}
//================================================================//
//  @brief  :		单边丢寻线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FindLineLost(void)
{
	if (0 == RightPnt.Type)			//左丢边 右寻线
	{
		LeftPnt.Type = 2;
		int i;
		for (i = DOWN_EAGE - 1; i > UP_EAGE; --i)
		{
			RL[i] = GetRL(i, RL[i + 1]);
			if (RL[i] - RL[i + 1] > FINDLINE_TH || RL[i + 1] - RL[i] > FINDLINE_TH || RIGHT_EAGE == RL[i])
			{
				RightPnt.ErrRow = i + 1;
				RightPnt.ErrCol = RL[i + 1];
				RightPnt.Type = 1;
				break;
			}
		}
		if (UP_EAGE == i)
		{
			RightPnt.ErrRow = UP_EAGE + 1;
			RightPnt.Type = 4;
			RightPnt.ErrCol = RL[RightPnt.ErrRow];
		}
		//对拐点的判断
		for (int i = LeftPnt.ErrRow; i > RightPnt.ErrRow; --i)
		{
			int TmpRow = SearchUpEage(i, RL[i] - 1);
			if (i - TmpRow > CROSSUP_TH || TmpRow == UP_EAGE)
			{
				RightPnt.Type = 2;
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
	else if (0 == LeftPnt.Type)			//右丢边 左寻线
	{
		RightPnt.Type = 2;
		int i;
		for (i = DOWN_EAGE - 1; i > UP_EAGE; --i)
		{
			LL[i] = GetLL(i, LL[i + 1]);
			if (LL[i] - LL[i + 1] > FINDLINE_TH || LL[i + 1] - LL[i] > FINDLINE_TH || LEFT_EAGE == LL[i])
			{
				LeftPnt.ErrRow = i + 1;
				LeftPnt.ErrCol = LL[i + 1];
				LeftPnt.Type = 1;
				break;
			}
		}
		if (UP_EAGE == i)
		{
			LeftPnt.Type = 4;
			LeftPnt.ErrRow = UP_EAGE + 1;
			LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
		}
		//对拐点的判断
		for (int i = RightPnt.ErrRow; i > LeftPnt.ErrRow; --i)
		{
			int TmpRow = SearchUpEage(i, LL[i] + 1);
			if (i - TmpRow > CROSSUP_TH || TmpRow == UP_EAGE)
			{
				LeftPnt.Type = 2;
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
//  @brief  :		寻找最低点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
Point FindLowPoint(int row, int col1, int col2, int Step)
{
	Point PointLow = { UP_EAGE, col1 };
	for (int j = col1; j <= col2; j += Step)
	{
		int TmpRow = SearchUpEage(row, j);
		if (TmpRow > PointLow.Row)
		{
			PointLow.Row = TmpRow;
			PointLow.Col = j;
		}
	}
	return PointLow;
}

