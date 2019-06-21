#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "CircleIsland.h"
#include "FirstLineProcess.h"
#include "MainProcess.h"

#define LEFT_PNT(row, type)	LeftPnt.ErrRow = (row); LeftPnt.ErrCol = LL[LeftPnt.ErrRow]; LeftPnt.Type = (type)
#define RIGHT_PNT(row, type) RightPnt.ErrRow = (row); RightPnt.ErrCol = RL[RightPnt.ErrRow]; RightPnt.Type = (type)
#define CIRFL_TH 6			//CircleIsland Find Line Threshold
#define CIRCROSSUP_TH 7
#define FIVE_SIX_TH 10

#define CIRCLEFINDUP_TH 10
Point PointA, PointB, PointC, PointD;
int LeftLost = 0, RightLost = 0;
int ChangeFlag = 0;
int ConstLeftEage = 30, ConstRightEage = 158;
//int SixFlag = 0;
//================================================================//
//  @brief  :		环岛判断
//  @param  :		void
//  @return :		CN不是 CL左环岛 CR右环岛
//  @note   :		void
//================================================================//
int IsCircleIsland(int type)
{
#define CIRCLEDIFF_TH 2		//环岛边缘差分阈值
#if CIRCLE == 1
	if (CL == type)
	{
		int TmpRow = LeftPnt.ErrRow;
		int MaxRow = MIN(10, DOWN_EAGE - LeftPnt.ErrRow);
		if (/*LL[TmpRow] - LL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 1] - LL[TmpRow] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 1] - LL[TmpRow + 2] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 2] - LL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 2] - LL[TmpRow + 3] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 3] - LL[TmpRow + 2] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 3] - LL[TmpRow + 4] <= CIRCLEDIFF_TH
			&& LL[TmpRow + 4] - LL[TmpRow + 3] <= CIRCLEDIFF_TH
			&& */1 == TrendArray(&LL[DOWN_EAGE], MaxRow))
		{
			string.Format("\r\n you = %d \r\n", 1); PrintDebug(string);
			if (LeftPnt.ErrRow >= UP_EAGE + 15 && RightPnt.ErrRow <= UP_EAGE + 1)
				return CL;
			else if (LeftPnt.ErrRow >= UP_EAGE + 20 && RightPnt.ErrRow <= UP_EAGE + 7)
				return CL;
			else if (LeftPnt.ErrRow > UP_EAGE + 28 && RightPnt.ErrRow <= UP_EAGE + 13)
				return CL;
			else if (LeftPnt.ErrRow >= UP_EAGE + 38 && RightPnt.ErrRow <= UP_EAGE + 20)
				return CL;
			else return CN;
		}
		else return CN;
	}
	else if (CR == type)
	{
		int TmpRow = RightPnt.ErrRow;
		int MaxRow = MIN(10, DOWN_EAGE - RightPnt.ErrRow);
		if (/*RL[TmpRow] - RL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 1] - RL[TmpRow] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 1] - RL[TmpRow + 2] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 2] - RL[TmpRow + 1] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 2] - RL[TmpRow + 3] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 3] - RL[TmpRow + 2] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 3] - RL[TmpRow + 4] <= CIRCLEDIFF_TH
			&& RL[TmpRow + 4] - RL[TmpRow + 3] <= CIRCLEDIFF_TH
			&& */-1 == TrendArray(&RL[DOWN_EAGE], MaxRow))
		{
			if (RightPnt.ErrRow >= UP_EAGE + 15 && LeftPnt.ErrRow <= UP_EAGE + 1)
				return CR;
			else if (RightPnt.ErrRow >= UP_EAGE + 20 && LeftPnt.ErrRow <= UP_EAGE + 7)
				return CR;
			else if (RightPnt.ErrRow >= UP_EAGE + 28 && LeftPnt.ErrRow <= UP_EAGE + 13)
				return CR;
			else if (RightPnt.ErrRow >= UP_EAGE + 38 && LeftPnt.ErrRow <= UP_EAGE + 20)
				return CR;
			else return CN;
		}
		else return CN;
	}
	else return CN;
#elif CIRCLE == 2
	int MaxRow = MIN(10, DOWN_EAGE - LeftPnt.ErrRow);
	int MaxRowR = MIN(10, DOWN_EAGE - RightPnt.ErrRow);
	if (CL == type && 1 == TrendArray(&LL[DOWN_EAGE], MaxRow)
		&& 1 == TrendArray(&LL[LeftPnt.ErrRow + MaxRow], MaxRow))
		return CL;
	else if (CR == type && -1 == TrendArray(&RL[DOWN_EAGE], MaxRow)
		&& -1 == TrendArray(&RL[RightPnt.ErrRow + MaxRow], MaxRow))
		return CR;
	else return CN;
#endif
}

//================================================================//
//  @brief  :		环岛补图
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFill(void)
{
	CircleIslandImageInit();
	//首行
	CircleFirstLine();
	//string.Format("\r\n LLDOWN = %d \r\n", LL[DOWN_EAGE]); PrintDebug(string);
	//string.Format("\r\n RLDOWN = %d \r\n", RL[DOWN_EAGE]); PrintDebug(string);
	//类型转化
	ChangeState(0);
	string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
	string.Format("\r\n CircleState = %d \r\n", CircleState); PrintDebug(string);
	//搜线
	CircleFindLine();
	string.Format("\r\n LeftPnt = %d %d %d \r\n", LeftPnt.Type, LeftPnt.ErrRow, LeftPnt.ErrCol); PrintDebug(string);
	string.Format("\r\n RightPnt = %d %d %d \r\n", RightPnt.Type, RightPnt.ErrRow, RightPnt.ErrCol); PrintDebug(string);
	//搜A点
	GetPointA();
	string.Format("\r\n PointA = %d %d \r\n", PointA.Row, PointA.Col); PrintDebug(string);
	//搜B点
	GetPointB();
	string.Format("\r\n PointB = %d %d \r\n", PointB.Row, PointB.Col); PrintDebug(string);
	//搜C点
	GetPointC();
	string.Format("\r\n PointC = %d %d \r\n", PointC.Row, PointC.Col); PrintDebug(string);
	//搜D点
	GetPointD();
	string.Format("\r\n PointD = %d %d \r\n", PointD.Row, PointD.Col); PrintDebug(string);
	//连接AB CD
	FillLineAB();
	FillLineCD();
	//补齐赛道
	FillAllEage();
	//补中线
	FillMiddleLine();
	//后类型转化
	ChangeState(1);
	string.Format("\r\n EndCircleState = %d \r\n", CircleState); PrintDebug(string);
}

//================================================================//
//  @brief  :		环岛处理初始化
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleIslandImageInit(void)
{
	VarInit();
	LeftLost = 0; RightLost = 0;
	LeftPnt.Type = RightPnt.Type = 0;
	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	LeftPnt.ErrCol = LEFT_EAGE;
	RightPnt.ErrCol = RIGHT_EAGE;
}
//================================================================//
//  @brief  :		环岛首行搜线以及边缘校正
//  @param  :		void
//  @return :		void
//  @note   :		小于等于3算丢边
//================================================================//
void CircleFirstLine(void)
{
	SelectFirstLine();//首行搜线
	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	LeftPnt.ErrCol = LL[DOWN_EAGE];
	RightPnt.ErrCol = RL[DOWN_EAGE];
	if (LL[DOWN_EAGE] <= LEFT_EAGE + 3)			//left lost
		LeftLost = 1;
	else LeftLost = 0;
	if (RL[DOWN_EAGE] >= RIGHT_EAGE - 3)		//right lost
		RightLost = 1;
	else RightLost = 0;
#define LOST_EAGE_TH 4	//判断为非丢边的阈值
	if (!LeftLost & !RightLost)  //左右均不丢，函数返回
		return;
	switch (CircleState)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 7:
		if (LeftLost && CR == CircleFlag)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] > LEFT_EAGE + LOST_EAGE_TH)		//no lost eage
				{
					LEFT_PNT(i, 0);
					LeftLost = 0;
					break;
				}
			}
		}
		if (RightLost && CL == CircleFlag)
		{
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] < RIGHT_EAGE - LOST_EAGE_TH)
				{
					RIGHT_PNT(i, 0);
					RightLost = 0;
					break;
				}
			}
		}
		break;
	case 4:
		if (CL == CircleFlag && RightLost)
		{
			RightPnt.Type = 1;
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				RL[i] = GetRL(i, RL[i + 1]);
			}
		}
		else if (CR == CircleFlag && LeftLost)
		{
			LeftPnt.Type = 1;
			for (int i = DOWN_EAGE - 1; i > UP_EAGE; i--)
			{
				LL[i] = GetLL(i, LL[i + 1]);
			}
		}
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		环岛状态变化
//  @param  :		0补图前 1补图后
//  @return :		void
//  @note   :		void
//================================================================//
void ChangeState(int Order)
{
	if (0 == Order)			//补图前
	{
		switch (CircleState)
		{
		case 1:
#define LOST_TH 50
			if (CL == CircleFlag && 1 == LeftLost && RL[DOWN_EAGE] > RIGHT_EAGE - LOST_TH)
				CircleState = 2;
			else if (CR == CircleFlag && 1 == RightLost && LL[DOWN_EAGE] < LEFT_EAGE + LOST_TH)
				CircleState = 2;
			break;
		case 2:
			if (CL == CircleFlag && 0 == LeftLost)
				CircleState = 3;
			else if (CR == CircleFlag && 0 == RightLost)
				CircleState = 3;
			break;
		case 3:
			if (CL == CircleFlag && RIGHT_EAGE - 3 <= RL[DOWN_EAGE])
				CircleState = 4;
			else if (CR == CircleFlag && LEFT_EAGE + 3 >= LL[DOWN_EAGE])
				CircleState = 4;
			break;
		case 4:
			if (CL == CircleFlag && 0 == RightLost)
				CircleState = 5;
			else if (CR == CircleFlag && 0 == LeftLost)
				CircleState = 5;
			break;
		case 6:
			if (CL == CircleFlag && 0 == RightLost)
			{
				int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE;
				int TmpCol = RL[DOWN_EAGE];
				while (1)
				{
					NewRow = SearchUpEage(OldRow + 2, TmpCol - 1);
					if (OldRow - NewRow > 12 || OldRow > DOWN_EAGE + 10 || TmpCol < MIDDLE - MIDDLE / 2)
					{
						break;
					}
					else if (OldRow <= DOWN_EAGE - 20)
					{
						if (IndJudgeCircle(0))
							CircleState = 7;
						break;
					}
					else
					{
						OldRow = NewRow;
						TmpCol--;
					}
				}
			}
			else if (CR == CircleFlag && 0 == LeftLost)
			{
				int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE;
				int TmpCol = LL[DOWN_EAGE];
				while (1)
				{
					NewRow = SearchUpEage(OldRow + 2, TmpCol + 1);
					if (OldRow - NewRow > 12 || OldRow > DOWN_EAGE + 10 || TmpCol > MIDDLE + MIDDLE / 2)
					{
						break;
					}
					else if (OldRow <= DOWN_EAGE - 20)
					{
						if (IndJudgeCircle(0))
							CircleState = 7;
						break;
					}
					else
					{
						OldRow = NewRow;
						TmpCol++;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else			//补图后
	{
		switch (CircleState)
		{
		case 5:
			if (1 == ChangeFlag)
			{
				ChangeFlag = 0;
				CircleState = 6;
			}
			break;
		case 7:
			if (1 == ChangeFlag)
			{
				ChangeFlag = 0;
				CircleState = 0;
				CircleFlag = 0;
			}
			break;
		default:
			break;
		}
	}
}

//================================================================//
//  @brief  :		环岛搜线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFindLine(void)
{
	FindLineNormal(0);
	switch (CircleState)
	{
	case 5:
	case 6:
		//拐点判断
		if (CL == CircleFlag)
		{
			LeftPnt.Type = 2;
			RightPnt.Type = 1;
			for (int i = DOWN_EAGE - 1; i > RightPnt.ErrRow; --i)
			{
				int TmpRow = SearchUpEage(i, RL[i] - 1);
				if (i - TmpRow > CIRCROSSUP_TH || UP_EAGE == TmpRow)
				{
					RIGHT_PNT(i, 2);
					break;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			RightPnt.Type = 2;
			LeftPnt.Type = 1;
			for (int i = DOWN_EAGE - 1; i > LeftPnt.ErrRow; --i)
			{
				int TmpRow = SearchUpEage(i, LL[i] + 1);
				if (i - TmpRow > CIRCROSSUP_TH || UP_EAGE == TmpRow)
				{
					LEFT_PNT(i, 2);
					break;
				}
			}
		}
		break;
	case 7:
		//detect exit island
#define EXIT_ISLAND 10
		if (RightPnt.ErrRow - LeftPnt.ErrRow < EXIT_ISLAND && LeftPnt.ErrRow - RightPnt.ErrRow < EXIT_ISLAND
			&& RightPnt.ErrCol - LeftPnt.ErrCol < 4 * EXIT_ISLAND)
		{
#if CI_IND
			if (ind_left + ind_right <= ind_ci_th)
#endif // CI_IND
				ChangeFlag = 1;
		}
		break;
	default:
		break;
	}
}



//================================================================//
//  @brief  :		搜A点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointA(void)
{
	switch (CircleState)
	{
	case 1:
		if (CL == CircleFlag)
		{
			PointA.Row = LeftPnt.ErrRow;
			PointA.Col = LeftPnt.ErrCol;
		}
		else if (CR == CircleFlag)
		{
			PointA.Row = RightPnt.ErrRow;
			PointA.Col = RightPnt.ErrCol;
		}
		else
			return;
		break;
	case 2:
		//give const PointA
		PointA.Row = DOWN_EAGE;
		if (CL == CircleFlag)
			PointA.Col = ConstLeftEage;
		else if (CR == CircleFlag)
			PointA.Col = ConstRightEage;
		break;
	case 3:
	case 4:
		PointA.Row = DOWN_EAGE;
		if (CL == CircleFlag)
			PointA.Col = LL[DOWN_EAGE] + 1;
		else if (CR == CircleFlag)
			PointA.Col = RL[DOWN_EAGE] - 1;
		break;
	case 7:
		if (CL == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE - 2, TmpCol = LL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow + 2, TmpCol + 1);
				if (OldRow - NewRow > 12 || NewRow <= UP_EAGE)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else if (TmpCol >= LL[DOWN_EAGE] + 40)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else
				{
					OldRow = NewRow;
					TmpCol++;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE - 2, TmpCol = RL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow + 2, TmpCol - 1);
				if (OldRow - NewRow > 12 || NewRow <= UP_EAGE)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else if (TmpCol <= RL[DOWN_EAGE] - 40)
				{
					PointA.Row = OldRow;
					PointA.Col = TmpCol;
					break;
				}
				else
				{
					OldRow = NewRow;
					TmpCol--;
				}
			}
		}
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		搜B点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointB(void)
{

	switch (CircleState)
	{
	case 1:
	case 2:
	case 3:
		int RowNew, RowOld, TmpCol;
		if (CL == CircleFlag)
		{
			RowNew = SearchUpEage(PointA.Row - 1, PointA.Col + 1);
			if (3 == CircleState && (PointA.Row - RowNew > 20 && 0 == LeftLost || LeftLost))
			{
				PointB = PointA;
			}
			else if (RowNew == UP_EAGE) //上边界 向上补线
			{
				PointB.Row = UP_EAGE;
			}
			else
			{
				RowOld = RowNew;
				TmpCol = PointA.Col;
				while (1)
				{
					RowNew = SearchUpEage(RowOld + 2, TmpCol + 1);
					if (RowOld - RowNew > CIRCLEFINDUP_TH || RowNew <= UP_EAGE || TmpCol >= RIGHT_EAGE)	//找到B点
					{
						PointB.Row = RowOld;
						PointB.Col = TmpCol;
						break;
					}
					else if (RightPnt.ErrCol <= TmpCol || DOWN_EAGE + 5 < RowNew)
					{
						PointB.Row = UP_EAGE;
						break;
					}
					else
					{
						RowOld = RowNew;
						TmpCol++;
					}
				}
			}

		}
		else if (CR == CircleFlag)
		{
			RowNew = SearchUpEage(PointA.Row - 1, PointA.Col - 1);
			if (3 == CircleState && (PointA.Row - RowNew > 20 && 0 == RightLost || RightLost))
			{
				PointB = PointA;
			}
			else if (RowNew == UP_EAGE) //上边界 向上补线
			{
				PointB.Row = UP_EAGE;
			}
			else
			{
				RowOld = RowNew;
				TmpCol = PointA.Col;
				while (1)
				{
					RowNew = SearchUpEage(RowOld + 2, TmpCol - 1);
					if (RowOld - RowNew > CIRCLEFINDUP_TH || RowNew <= UP_EAGE || TmpCol <= LEFT_EAGE)	//找到B点
					{
						PointB.Row = RowOld;
						PointB.Col = TmpCol;
						break;
					}
					else if (LeftPnt.ErrCol >= TmpCol || DOWN_EAGE + 5 < RowNew)
					{
						PointB.Row = UP_EAGE;
						break;
					}
					else
					{
						RowOld = RowNew;
						TmpCol--;
					}
				}
			}
		}
		else
			return;
		break;
	case 4:
		if (CL == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE, TmpCol = LL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow, TmpCol + 1);
				if (OldRow - NewRow > 15 || NewRow == UP_EAGE)
				{
					PointB.Row = OldRow;
					PointB.Col = TmpCol;
					break;
				}
				else if (TmpCol >= RIGHT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					TmpCol++;
					OldRow = NewRow;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			int NewRow = DOWN_EAGE, OldRow = DOWN_EAGE, TmpCol = RL[DOWN_EAGE];
			while (1)
			{
				NewRow = SearchUpEage(OldRow, TmpCol - 1);
				if (OldRow - NewRow > 15 || NewRow == UP_EAGE)
				{
					PointB.Row = OldRow;
					PointB.Col = TmpCol;
					break;
				}
				else if (TmpCol <= LEFT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					TmpCol--;
					OldRow = NewRow;
				}
			}
		}
		break;
	case 7:
		if (CL == CircleFlag)
		{
			Point PointNew = { SearchUpEage(PointA.Row + 1, PointA.Col + 1), PointA.Col + 1 };
			Point PointOld = PointNew;
			if (UP_EAGE == PointNew.Row)
			{
				PointB.Row = UP_EAGE;
				break;
			}
			while (1)
			{
				PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
				if (PointOld.Row - PointNew.Row > 3 || PointNew.Row <= UP_EAGE)
				{
					PointB = PointOld;
					break;
				}
				else if (PointNew.Row >= DOWN_EAGE || PointNew.Col >= RIGHT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					PointOld = PointNew;
				}
			}
		}
		else if (CR == CircleFlag)
		{
			Point PointNew = { SearchUpEage(PointA.Row + 1, PointA.Col - 1), PointA.Col - 1 };
			Point PointOld = PointNew;
			if (UP_EAGE == PointNew.Row)
			{
				PointB.Row = UP_EAGE;
				break;
			}
			while (1)
			{
				PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
				if (PointOld.Row - PointNew.Row > 3 || PointNew.Row <= UP_EAGE)
				{
					PointB = PointOld;
					break;
				}
				else if (PointNew.Row >= DOWN_EAGE || PointNew.Col <= LEFT_EAGE)
				{
					PointB.Row = UP_EAGE;
					break;
				}
				else
				{
					PointOld = PointNew;
				}
			}
		}
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		搜C点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointC(void)
{
	const int UpTh = 3;
	switch (CircleState)
	{
	case 1:
		break;
	case 2:
		if (PointB.Row == UP_EAGE) //Not Find PointB
			PointC.Row = UP_EAGE;
		else
		{
			int TmpRow;
			if (CL == CircleFlag)		//Left CircleIsland
			{
				TmpRow = SearchUpEage(PointB.Row - 1, PointB.Col + 1);
				PointC.Row = TmpRow;
				PointC.Col = PointB.Col + 20;
			}
			else if (CR == CircleFlag)		//Right CircleIsland
			{
				TmpRow = SearchUpEage(PointB.Row - 1, PointB.Col - 1);
				PointC.Row = TmpRow;
				PointC.Col = PointB.Col - 20;
			}
			else return;
		}
		break;
	case 3:
		if (PointB.Row == UP_EAGE) //Not Find PointB
		{
			PointC.Row = UP_EAGE;
		}
		else
		{
			int TmpRow;
			if (CL == CircleFlag)		//Left CircleIsland
			{
				TmpRow = SearchUpEage(PointB.Row - 1, PointB.Col + 1);
				PointC.Row = TmpRow;
				PointC.Col = PointB.Col + 11;
			}
			else if (CR == CircleFlag)		//Right CircleIsland
			{
				TmpRow = SearchUpEage(PointB.Row - 1, PointB.Col - 1);
				PointC.Row = TmpRow;
				PointC.Col = PointB.Col - 11;
			}
			else return;
		}
		break;
	case 4:
		if (CL == CircleFlag)
		{
			Point TmpPnt = SearchRightUpEage45(PointB.Row, PointB.Col + 1);
			int tmp_col = SearchRightEage(TmpPnt.Row + 1, TmpPnt.Col);
			if (TmpPnt.Row < UP_EAGE + 10 || (TmpPnt.Col - RL[TmpPnt.Row] < 8 && RL[TmpPnt.Row] - TmpPnt.Col < 8) || tmp_col == RL[TmpPnt.Row + 1])
			{
				PointC.Row = UP_EAGE;
				PointC.Col = PointB.Col;
				RL[PointC.Row] = PointC.Col;
				RIGHT_PNT(PointC.Row + 1, 1);
				break;
			}
			RL[TmpPnt.Row + 1] = tmp_col;
			RIGHT_PNT(TmpPnt.Row + 1, 1);
			int i;
			for (i = TmpPnt.Row + 2; i < DOWN_EAGE; ++i)
			{
				RL[i] = GetRL(i, RL[i - 1]);
				if (RL[i] - RL[i - 1] > 2 * CIRFL_TH || RL[i - 1] - RL[i] > 2 * CIRFL_TH || RL[i] >= RIGHT_EAGE - 3)
				{
					PointC.Row = i - 1;
					PointC.Col = RL[i - 1];
					break;
				}
			}
			if (DOWN_EAGE == i)
			{
				PointC.Row = UP_EAGE;
				PointC.Col = PointB.Col;
				RL[PointC.Row] = PointC.Col;
				RIGHT_PNT(PointC.Row + 1, 1);
			}
		}
		else if (CR == CircleFlag)
		{
			Point TmpPnt = SearchLeftUpEage45(PointB.Row, PointB.Col - 1);
			int tmp_col = SearchLeftEage(TmpPnt.Row + 1, TmpPnt.Col);
			if (TmpPnt.Row < UP_EAGE + 10 || (TmpPnt.Col - LL[TmpPnt.Row] < 8 && LL[TmpPnt.Row] - TmpPnt.Col < 8) || tmp_col == LL[TmpPnt.Row + 1])
			{
				PointC.Row = UP_EAGE;
				PointC.Col = PointB.Col;
				LL[PointC.Row] = PointC.Col;
				LEFT_PNT(PointC.Row + 1, 1);
				break;
			}
			LL[TmpPnt.Row + 1] = tmp_col;
			LEFT_PNT(TmpPnt.Row + 1, 1);
			int i;
			for (i = TmpPnt.Row + 2; i < DOWN_EAGE; ++i)
			{
				LL[i] = GetLL(i, LL[i - 1]);
				if (LL[i] - LL[i - 1] > 2 * CIRFL_TH || LL[i - 1] - LL[i] > 2 * CIRFL_TH || LL[i] <= LEFT_EAGE + 3)
				{
					PointC.Row = i - 1;
					PointC.Col = LL[i - 1];
					break;
				}
			}
			if (DOWN_EAGE == i)
			{
				PointC.Row = UP_EAGE;
				PointC.Col = PointB.Col;
				LL[PointC.Row] = PointC.Col;
				LEFT_PNT(PointC.Row + 1, 1);
			}
		}
		else return;
		break;
	case 5:
		if (CL == CircleFlag)
		{
			if (RightPnt.Type == 2 && RightPnt.ErrRow > DOWN_EAGE - FIVE_SIX_TH)// || DOWN_EAGE == RightPnt.ErrRow)
			{
				//拐点确认
				int UpSum = 0, DownSum = 0;
				for (int i = 1; i < 5; i++)
				{
					UpSum += RightPnt.ErrCol - RL[RightPnt.ErrRow - i];
					DownSum += RightPnt.ErrCol - RL[RightPnt.ErrRow + i];
				}
				if (UpSum <= 0 && DownSum <= 0)
					ChangeFlag = 1;

			}
			PointC.Col = (LeftPnt.ErrCol + RightPnt.ErrCol) >> 1;
			PointC.Row = SearchUpEage((LeftPnt.ErrRow + RightPnt.ErrRow) >> 1, PointC.Col);
		}
		else if (CR == CircleFlag)
		{
			if (LeftPnt.Type == 2 && LeftPnt.ErrRow > DOWN_EAGE - FIVE_SIX_TH)// || DOWN_EAGE == LeftPnt.ErrRow)
			{
				//拐点确认
				int UpSum = 0, DownSum = 0;
				for (int i = 1; i < 5; i++)
				{
					UpSum += LeftPnt.ErrCol - LL[LeftPnt.ErrRow - i];
					DownSum += LeftPnt.ErrCol - LL[LeftPnt.ErrRow + i];
				}
				if (UpSum >= 0 && DownSum >= 0)
					ChangeFlag = 1;
				PointC.Col = (LeftPnt.ErrCol + RightPnt.ErrCol) >> 1;
				PointC.Row = SearchUpEage((RightPnt.ErrRow + LeftPnt.ErrRow) >> 1, PointC.Col);
			}
			PointC.Col = (LeftPnt.ErrCol + RightPnt.ErrCol) >> 1;
			PointC.Row = SearchUpEage((RightPnt.ErrRow + LeftPnt.ErrRow) >> 1, PointC.Col);
		}
		else return;
		break;
	case 6:
		if (CL == CircleFlag)
		{
			int Eage = MIN(ConstRightEage, RightPnt.ErrCol);
			PointC.Col = (LeftPnt.ErrCol + Eage) >> 1;
		}
		else if (CR == CircleFlag)
		{
			int Eage = MAX(ConstLeftEage, LeftPnt.ErrCol);
			PointC.Col = (Eage + RightPnt.ErrCol) >> 1;
		}
		PointC.Row = SearchUpEage((LeftPnt.ErrRow + RightPnt.ErrRow) >> 1, PointC.Col);
		break;
	case 7:
		if (CL == CircleFlag && DOWN_EAGE - 25 < RightPnt.ErrRow
			|| CR == CircleFlag && DOWN_EAGE - 25 < LeftPnt.ErrRow)
		{
			PointC.Col = (LeftPnt.ErrCol + RightPnt.ErrCol) >> 1;
			PointC.Row = SearchUpEage((LeftPnt.ErrRow + RightPnt.ErrRow) >> 1, PointC.Col);
		}
		else PointC.Row = UP_EAGE;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		搜D点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointD(void)
{
	switch (CircleState)
	{
	case 1:
		break;
	case 2:
		if (UP_EAGE == PointB.Row)			//Not find PointB
			PointD.Row = UP_EAGE;
		else if (CL == CircleFlag)		//left CircleIsland
		{
			PointD.Row = DOWN_EAGE;
			PointD.Col = RL[DOWN_EAGE];
		}
		else if (CR == CircleFlag)
		{
			PointD.Row = DOWN_EAGE;
			PointD.Col = LL[DOWN_EAGE];
		}
		else return;
		break;
	case 3:
		if (UP_EAGE == PointB.Row)
			PointD.Row = UP_EAGE;
		else if (CL == CircleFlag)
		{
			PointD.Row = DOWN_EAGE;
			PointD.Col = MIN(ConstRightEage, RL[DOWN_EAGE]);
		}
		else if (CR == CircleFlag)
		{
			PointD.Row = DOWN_EAGE;
			PointD.Col = MAX(ConstLeftEage, LL[DOWN_EAGE]);
		}
		else return;
		break;
	case 4:
		PointD.Row = DOWN_EAGE;
		if (CL == CircleFlag)
		{
			if (LL[DOWN_EAGE] + 130 > RIGHT_EAGE)
				PointD.Col = RIGHT_EAGE;
			else PointD.Col = LL[DOWN_EAGE] + 130;
		}
		else if (CR == CircleFlag)
		{
			if (RL[DOWN_EAGE] - 130 < LEFT_EAGE)
				PointD.Col = LEFT_EAGE;
			else
				PointD.Col = RL[DOWN_EAGE] - 130;
		}
		else return;
		break;
	case 5:
		if (CL == CircleFlag)
		{
			PointD.Row = RightPnt.ErrRow;
			PointD.Col = RightPnt.ErrCol;
		}
		else if (CR == CircleFlag)
		{
			PointD.Row = LeftPnt.ErrRow;
			PointD.Col = LeftPnt.ErrCol;
		}
		break;
	case 6:
		if (CL == CircleFlag)
		{
			PointD.Row = RightPnt.ErrRow;
			PointD.Col = MIN(ConstRightEage, RightPnt.ErrCol);
		}
		else if (CR == CircleFlag)
		{
			PointD.Row = LeftPnt.ErrRow;
			PointD.Col = MAX(ConstLeftEage, LeftPnt.ErrCol);
		}
		break;
	case 7:
		if (UP_EAGE != PointC.Row)
		{
			if (CL == CircleFlag)
			{
				PointD.Row = RightPnt.ErrRow;
				PointD.Col = RL[PointD.Row];
			}
			else if (CR == CircleFlag)
			{
				PointD.Row = LeftPnt.ErrRow;
				PointD.Col = LL[PointD.Row];
			}
		}
		else
			PointD.Row = UP_EAGE;
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		连接AB
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillLineAB(void)
{
	switch (CircleState)
	{
	case 1:
		if (CL == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				LeftPnt = FillLineUp(LL, PointA.Row + 6, PointA.Row + 2);
			else
			{
				LL[PointB.Row] = PointB.Col;
				LL[PointA.Row] = PointA.Col;
				FillLinePoint(LL, PointA.Row, PointB.Row);
				LEFT_PNT(PointB.Row, 0);
			}
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				RightPnt = FillLineUp(RL, PointA.Row + 6, PointA.Row + 2);
			else
			{
				RL[PointB.Row] = PointB.Col;
				RL[PointA.Row] = PointA.Col;
				FillLinePoint(RL, PointA.Row, PointB.Row);
				RIGHT_PNT(PointB.Row, 0);
			}
		}
		break;
	case 2:
		if (CL == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				PointB.Col = 78;
			LL[PointB.Row] = PointB.Col;
			LL[PointA.Row] = PointA.Col;
			FillLinePoint(LL, PointA.Row, PointB.Row);
			LEFT_PNT(PointB.Row, 0);
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE == PointB.Row)
				PointB.Col = 110;
			RL[PointB.Row] = PointB.Col;
			RL[PointA.Row] = PointA.Col;
			FillLinePoint(RL, PointA.Row, PointB.Row);
			RIGHT_PNT(PointB.Row, 0);
		}
		break;
	case 7:
		if (CL == CircleFlag)
		{
			if (DOWN_EAGE - 40 >= PointB.Row)		//no find line AB
			{
				if (RL[DOWN_EAGE - 30] < LEFT_EAGE + 60)
					;
				else
				{
					PointB.Row = DOWN_EAGE - 30;
					PointB.Col = RL[DOWN_EAGE - 30] - 60;
					if (LeftLost)			//lost A
					{
						PointA.Col = LEFT_EAGE;
						PointA.Row = DOWN_EAGE;
					}
					else if (RL[DOWN_EAGE] - PointA.Col > 120)
					{
						PointA.Col = RL[DOWN_EAGE] - 120;
						PointA.Row = DOWN_EAGE;
					}
					LL[PointB.Row] = PointB.Col;
					LL[PointA.Row] = PointA.Col;
					FillLinePoint(LL, PointA.Row, PointB.Row);
					LeftPnt = FillLineUp(LL, PointB.Row + 3, PointB.Row);
				}
			}
			else
			{
				LL[PointB.Row] = PointB.Col;
				LEFT_PNT(PointB.Row, 0);
				if (RL[DOWN_EAGE] - PointA.Col > 120)				//lost A
				{
					PointA.Row = DOWN_EAGE;
					PointA.Col = RL[DOWN_EAGE] - 120;
				}
				LL[PointA.Row] = PointA.Col;
				FillLinePoint(LL, PointA.Row, PointB.Row);
			}
		}
		else if (CR == CircleFlag)
		{
			if (DOWN_EAGE - 40 >= PointB.Row)		//no find line AB
			{
				if (LL[DOWN_EAGE - 30] > RIGHT_EAGE - 60)
					;
				else
				{
					PointB.Row = DOWN_EAGE - 30;
					PointB.Col = LL[DOWN_EAGE - 30] + 60;
					if (RightLost)			//lost A
					{
						PointA.Col = RIGHT_EAGE;
						PointA.Row = DOWN_EAGE;
					}
					else if (PointA.Col - LL[DOWN_EAGE] > 120)
					{
						PointA.Col = LL[DOWN_EAGE] + 120;
						PointA.Row = DOWN_EAGE;
					}
					RL[PointB.Row] = PointB.Col;
					RL[PointA.Row] = PointA.Col;
					FillLinePoint(RL, PointA.Row, PointB.Row);
					RightPnt = FillLineUp(RL, PointB.Row + 3, PointB.Row);
				}
			}
			else
			{
				RL[PointB.Row] = PointB.Col;
				RIGHT_PNT(PointB.Row, 0);
				if (PointA.Col - LL[DOWN_EAGE] > 120)				//lost A
				{
					PointA.Row = DOWN_EAGE;
					PointA.Col = LL[DOWN_EAGE] + 120;
				}
				RL[PointA.Row] = PointA.Col;
				FillLinePoint(RL, PointA.Row, PointB.Row);
			}
		}
		break;
	default:
		break;
	}
}
//================================================================//
//  @brief  :		连接CD
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillLineCD(void)
{
	switch (CircleState)
	{
	case 3:
		if (UP_EAGE == PointB.Row)
			return;
		else if (CL == CircleFlag)
		{
			RL[PointD.Row] = PointD.Col;
			RL[PointC.Row] = PointC.Col;
			Connect_Point_Curve(RL, PointD.Row, PointD.Col, PointC.Row, PointC.Col);
			RIGHT_PNT(PointC.Row, 1);
		}
		else if (CR == CircleFlag)
		{
			LL[PointD.Row] = PointD.Col;
			LL[PointC.Row] = PointC.Col;
			Connect_Point_Curve(LL, PointD.Row, PointD.Col, PointC.Row, PointC.Col);
			LEFT_PNT(PointC.Row, 1);
		}
		break;
	case 4:
		if (CL == CircleFlag)
		{
			RL[PointD.Row] = PointD.Col;
			FillLinePoint(RL, PointD.Row, PointC.Row);
		}
		else if (CR == CircleFlag)
		{
			LL[PointD.Row] = PointD.Col;
			FillLinePoint(LL, PointD.Row, PointC.Row);
		}
		break;
	case 5:
		if (CL == CircleFlag && 2 == RightPnt.Type)
		{
			RL[PointD.Row] = PointD.Col;
			RL[PointC.Row] = PointC.Col;
			FillLinePoint(RL, PointD.Row, PointC.Row);
			RIGHT_PNT(PointC.Row, 1);
		}
		else if (CR == CircleFlag && 2 == LeftPnt.Type)
		{
			LL[PointD.Row] = PointD.Col;
			LL[PointC.Row] = PointC.Col;
			FillLinePoint(LL, PointD.Row, PointC.Row);
			LEFT_PNT(PointC.Row, 1);
		}
		break;
	case 6:
		if (CL == CircleFlag)
		{
			RL[PointD.Row] = PointD.Col;
			RL[PointC.Row] = PointC.Col;
			FillLinePoint(RL, PointD.Row, PointC.Row);
			RIGHT_PNT(PointC.Row, 1);
		}
		else if (CR == CircleFlag)
		{
			LL[PointD.Row] = PointD.Col;
			LL[PointC.Row] = PointC.Col;
			FillLinePoint(LL, PointD.Row, PointC.Row);
			LEFT_PNT(PointC.Row, 1);
		}
		break;
	case 7:
		if (CL == CircleFlag)
		{
			if (UP_EAGE != PointC.Row)	//fill line CD
			{
				RL[PointC.Row] = PointC.Col;
				RL[PointD.Row] = PointD.Col;
				FillLinePoint(RL, PointD.Row, PointC.Row);
				RIGHT_PNT(PointC.Row, 0);
			}
		}
		else if (CR == CircleFlag)
		{
			if (UP_EAGE != PointC.Row)	//fill line CD
			{
				LL[PointC.Row] = PointC.Col;
				LL[PointD.Row] = PointD.Col;
				FillLinePoint(LL, PointD.Row, PointC.Row);
				LEFT_PNT(PointC.Row, 0);
			}
		}
		break;
	default:
		break;
	}
}
//================================================================//
//  @brief  :		补齐赛道
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillAllEage(void)
{
	switch (CircleState)
	{
	case 1:
	case 2:
	case 3:
	case 7:
		FindLineNormal(0);
		break;
	default:
		break;
	}
}

//================================================================//
//  @brief  :		给定斜率补线
//  @param  :		void
//  @return :		行数
//  @note   :		void
//================================================================//
int FillLineGradient(int* line, int row, int col, float Gradient)
{
	int* p = line + row - 1;
	for (int i = row - 1; i > UP_EAGE; i--)
	{
		*p = (int)((i - row) * Gradient + col + 0.5);
		if (*p > RIGHT_EAGE || *p < LEFT_EAGE)
		{
			return i + 1;
		}
		p--;
	}
	return UP_EAGE + 1;
}

//================================================================//
//  @brief  :		抛物线连接两点
//  @param  :		两点行坐标(x_position)和列坐标(y_position)
//  @return :		void
//  @note   :		针对Left_border[],Right_border[],Middle_line[];
//					point1为中间点，point2为两侧点
//================================================================//
void Connect_Point_Curve(int* line_process, int x_position1, int y_position1, int x_position2, int y_position2)
{
	float c_K, c_B;
	if (x_position1 == x_position2 || y_position1 == y_position2)
	{
		return;
	}
	line_process += x_position1;
	if (y_position1 > y_position2)
	{
		c_K = (float)(y_position1 - y_position2) * (y_position1 + y_position2) / (x_position1 - x_position2);
		c_B = c_K * x_position1 - y_position1 * y_position1;
	}
	else
	{
		c_K = (float)(y_position2 - y_position1) * (y_position1 + y_position2) / (x_position1 - x_position2);
		c_B = c_K * x_position1 - y_position2 * y_position2;
	}

	if (y_position1 > y_position2)
	{
		for (int c_point = x_position1; c_point >= x_position2; c_point--)
		{
			*line_process = (int)sqrt(c_K * c_point - c_B);
			line_process--;
		}
	}
	else
	{
		for (int c_point = x_position1; c_point >= x_position2; c_point--)
		{
			*line_process = y_position2 + y_position1 - (int)sqrt(c_K * c_point - c_B);
			line_process--;
		}
	}
}