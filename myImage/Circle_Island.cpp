#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"
#include "Judge.h"
#include "exp.h"
#include "Circle_Island.h"

//==============全局变量=========================//
#define THREE_TO_FOUR_TREND_TH 30		//状态3转状态4趋势采用有效行
#define STATE_FOUR_WIDTH 165			//状态4赛道宽度
#define STATE_FOUR_PB_EAGE_OFFSET 40	//状态4点B相对赛道边界偏移量
#define STATE_FOUR_FILLLINE_TYPE_TH 50	//状态4向下补线方式阈值
#define STATE_SIX_FINDLINE_TYPE_TH	70	//状态6是否搜E点阈值
#define GETPOINTB_TH 10					//搜B点阈值
#define GETPOINTC_TH 30					//搜C点阈值
#define GETPOINTE_TH 20					//搜E点阈值
typedef struct
{
	int Row;
	int Col;
}Point;
Point PointA, PointB, PointC, PointD, PointE;
int LastLeft, LastRight, LastMiddle;
int CircleLeftLost = 0, CircleRightLost = 0;  //左右丢边情况
int SwitchFlag = 0;								//状态变换标志

int OneState = 0, TwoState = 0;					//状态5 0-1-0判断


//================================================================//
//  @brief  :		判断是否为环岛
//  @param  :		void
//  @return :		1是左环岛 2是右环岛 0不是
//  @note   :		void
//================================================================//
int IsCircleIsland(int type)
{
	if (FIND_LEFT == type)
	{
		for (int i = LeftPnt.ErrRow; i < RightPnt.ErrRow; ++i)
		{
			if (IsEageCol(i, LeftPnt.ErrCol, 3, 1))
			{
				PointA.Row = LeftPnt.ErrRow;
				PointA.Col = LeftPnt.ErrCol;
				LeftPnt.ErrRow = i;
				LeftPnt.ErrType = 10;
				return 1;
			}
		}
	}
	else if (FIND_RIGHT == type)
	{
		for (int i = RightPnt.ErrRow; i < LeftPnt.ErrRow; ++i)
		{
			if (IsEageCol(i, RightPnt.ErrCol, 3, 1))
			{
				PointA.Row = RightPnt.ErrRow;
				PointA.Col = RightPnt.ErrCol;
				RightPnt.ErrRow = i;
				RightPnt.ErrType = 10;
				return 2;
			}
		}
	}
	return 0;
}

//================================================================//
//  @brief  :		补环岛总函数
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillCircleIsland(void)
{
	//三点初始化
	PointInit();
	JudgeState();
	LastLeft = 80;
	LastRight = 240;
	LastMiddle = 160;
	str.Format("\r\n CircleState = %d \r\n", CircleState); PrintDebug(str);
	switch (CircleState)
	{
	case 0:
		LeftPnt.ErrType = 0;
		RightPnt.ErrType = 0;
		return;
	case 1:
		CircleOne();
		break;
	case 2:
		CircleTwo();
		break;
	case 3:
		CircleThree();
		break;
	case 4:
		CircleFour();
		break;
	case 5:
		CircleFive();
		str.Format("\r\n CircleLeftPnt = %d \r\n", 1); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", LeftPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", LeftPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", LeftPnt.ErrType); PrintDebug(str);
		str.Format("\r\n CircleRightPnt = %d \r\n", 0); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", RightPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", RightPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", RightPnt.ErrType); PrintDebug(str);
		break;
	case 6:
		CircleSix();
		break;
	default:
		break;

	}
	LastLeft = LL[0];
	LastRight = RL[0];
	LastMiddle = (LastLeft + LastRight) >> 1;

}

//================================================================//
//  @brief  :		判断状态,更改状态
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void JudgeState(void)
{
	//前十行初步判断
	if (LEFT_EAGE == ArrayMin(LL, 10))
	{
		CircleLeftLost = 1;
		LeftPntTre.ErrType = 7;
	}
	else CircleLeftLost = 0;
	if (RIGHT_EAGE == ArrayMax(RL, 10))
	{
		CircleRightLost = 1;
		RightPntTre.ErrType = 7;
	}
	else CircleRightLost = 0;

	if (!CircleLeftLost && !CircleRightLost)
	{
		LeftPnt.ErrRow = RightPnt.ErrCol = 0;
		FindLineNormal();
	}
	else if (CircleLeftLost && CircleRightLost)
		;
	else;
		//FindLineLost();

	switch (CircleState)
	{
	case 0:
		CircleState = 1;
		break;
	case 1:
		if (1 == CircleFlag)	//左环岛
		{
			if (CircleLeftLost)
				CircleState = 2;
		}
		else if (2 == CircleFlag)  //右环岛
		{
			if (CircleRightLost)
				CircleState = 2;
		}
		break;
	case 2:
		if (1 == CircleFlag)	//左环岛
		{
			if (!CircleLeftLost)
				CircleState = 3;
		}
		else if (2 == CircleFlag)	//右环岛
		{
			if (!CircleRightLost)
				CircleState = 3;
		}
		break;
	case 3:
		if (1 == CircleFlag)	//左环岛
		{
			if (-1 == trend(LL, MIN(THREE_TO_FOUR_TREND_TH, LeftPnt.ErrRow)))
				CircleState = 4;
		}
		else if (2 == CircleFlag)	//右环岛
		{
			if (1 == trend(RL, MIN(THREE_TO_FOUR_TREND_TH, RightPnt.ErrRow)))
				CircleState = 4;
		}
		break;
	case 4: 
		if (1 == CircleFlag)		//左环岛
		{
			if (CircleRightLost)
				TwoState = 1;
			if (TwoState && !CircleRightLost)
			{
				CircleState = 5;
				TwoState = 0;
			}
		}
		else if (2 == CircleFlag)		//右环岛
		{
			if (CircleLeftLost)
				TwoState = 1;
			if (TwoState && !CircleLeftLost)
			{
				CircleState = 5;
				TwoState = 0;
			}
		}
		break;
	case 5:
		if (1 == CircleFlag)	//左环岛
		{
			if (CircleRightLost)
				OneState = 1;
			if (OneState && !CircleRightLost)
			{
				CircleState = 6;
				OneState = 0;
			}
		}
		else if (2 == CircleFlag)	//右环岛
		{
			if (CircleLeftLost)
				OneState = 1;
			if (OneState && !CircleLeftLost)
			{
				CircleState = 6;
				OneState = 0;
			}
		}
		break;
	case 6:
		if (!CircleLeftLost && !CircleRightLost)
		{
			CircleState = 0;
			CircleFlag = 0;
		}
	default:
		break;
	}

}


//================================================================//
//  @brief  :		状态1补线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleOne(void)
{
	//LeftPnt.ErrRow = RightPnt.ErrRow = 0;
	//FindLineNormal();
	if (1 == CircleFlag)		//左环岛
	{
		PointA.Row = LeftPnt.ErrRow;
		PointA.Col = LeftPnt.ErrCol;
		//搜B点
		GetPointB();
		str.Format("\r\n PointB = %d \r\n", PointB.Row); PrintDebug(str);
		str.Format("\r\n PointB = %d \r\n", PointB.Col); PrintDebug(str);
		//搜C点
		GetPointC();
		//搜D点
		GetPointD();
		//连接AB，CD
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;
		LL[PointA.Row] = PointA.Col;
		LL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_LEFT, PointA.Row, PointB.Row);
		//补齐左线
		for (int i = PointB.Row + 1; i < PointC.Row; ++i)
		{
			if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
			{
				LeftPnt.ErrRow = --i;
				LeftPnt.ErrCol = LL[i];
				break;
			}
		}
		RightPnt.ErrRow = PointC.Row;
		RightPnt.ErrCol = PointC.Col;
		LeftPnt.ErrType = 2;
		RightPnt.ErrType = 1;

	}
	else if (2 == CircleFlag)		//右环岛
	{
		PointA.Row = RightPnt.ErrRow;
		PointA.Col = RightPnt.ErrCol;
		//搜B点
		GetPointB();
		//搜C点
		GetPointC();
		//搜D点
		GetPointD();
		//连接AB，CD
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;
		RL[PointA.Row] = PointA.Col;
		RL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_RIGHT, PointA.Row, PointB.Row);
		//补齐右线
		for (int i = PointB.Row + 1; i < PointC.Row; ++i)
		{
			if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
			{
				RightPnt.ErrRow = --i;
				RightPnt.ErrCol = RL[i];
				break;
			}
		}
		LeftPnt.ErrRow = PointC.Row;
		LeftPnt.ErrCol = PointC.Col;
		LeftPnt.ErrType = 1;
		RightPnt.ErrType = 2;
	}
}

//================================================================//
//  @brief  :		状态2补线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleTwo(void)
{
	if (1 == CircleFlag)	//左环岛
	{
		////补右线
		int k = 0;
		for (int k = 1; k < IMG_ROWS; ++k)
			if (1 != FindL(FIND_RIGHT, k, k - 1, AutoValue(k)))
			{
				RightPnt.ErrRow = --k;
				RightPnt.ErrCol = RL[k];
				break;
			}
		if (IMG_ROWS == k)
		{
			RightPnt.ErrRow = --k;
			RightPnt.ErrCol = RL[k];
		}
		//左线
		PointA.Row = 0;
		PointA.Col = LastLeft;
		//搜B点
		int Eage_1 = 0;
		for (int i = PointA.Row; i < RightPnt.ErrRow; ++i)
		{
			if (IsEageCol(i, PointA.Col, 3, 1))
			{
				Eage_1 = i;
				break;
			}
		}
		for (int j = PointA.Col + 1; j < RL[0]; ++j)
		{
			int i;
			for (i = Eage_1; i < Eage_1 + 10; ++i)
			{
				if (IsEageCol(i, j, 3, 1))
				{
					Eage_1 = i;
					break;
				}

			}
			if (10 + Eage_1 == i)		//找到B点
			{
				PointB.Row = Eage_1;
				PointB.Col = j - 1;
				break;
			}
		}
		str.Format("\r\n PointB = %d \r\n", PointB.Row); PrintDebug(str);
		str.Format("\r\n PointB = %d \r\n", PointB.Col); PrintDebug(str);
		//搜C点
		GetPointC();
		//搜D点
		GetPointD();
		//连接AB，CD
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;
		LL[PointA.Row] = PointA.Col;
		LL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_LEFT, PointA.Row, PointB.Row);
		//补齐左线
		for (int i = PointB.Row + 1; i < PointC.Row; ++i)
		{
			if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
			{
				LeftPnt.ErrRow = --i;
				LeftPnt.ErrCol = LL[i];
				break;
			}
		}
		RightPnt.ErrRow = PointC.Row;
		RightPnt.ErrCol = PointC.Col;
		LeftPnt.ErrType = 2;
		RightPnt.ErrType = 1;
	}
	else if (2 == CircleFlag)	//右环岛
	{
		//补左线
		int k = 0;
		for (k = 1; k < IMG_ROWS; ++k)
			if (1 != FindL(FIND_LEFT, k, k - 1, AutoValue(k)))
			{
				LeftPnt.ErrRow = --k;
				LeftPnt.ErrCol = LL[k];
				break;
			}
		if (IMG_ROWS == k)
		{
			LeftPnt.ErrRow = --k;
			LeftPnt.ErrCol = LL[k];
		}
		str.Format("\r\n LeftPnt = %d \r\n", LeftPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n LeftPnt = %d \r\n", LeftPnt.ErrCol); PrintDebug(str);
		//右线
		PointA.Row = 0;
		PointA.Col = LastRight;
		//搜B点
		int Eage_1 = 0;
		for (int i = PointA.Row; i < LeftPnt.ErrRow; ++i)
		{
			if (IsEageCol(i, PointA.Col, 3, 1))
			{
				Eage_1 = i;
				break;
			}
		}
		for (int j = PointA.Col - 1; j > LL[0]; --j)
		{
			int i;
			for (i = Eage_1; i < Eage_1 + 10; ++i)
			{
				if (IsEageCol(i, j, 3, 1))
				{
					Eage_1 = i;
					break;
				}

			}
			if (10 + Eage_1 == i)		//找到B点
			{
				PointB.Row = Eage_1;
				PointB.Col = j + 1;
				break;
			}
		}
		str.Format("\r\n PointB = %d \r\n", PointB.Row); PrintDebug(str);
		str.Format("\r\n PointB = %d \r\n", PointB.Col); PrintDebug(str);
		//搜C点
		GetPointC();
		//搜D点
		GetPointD();
		//连接AB，CD
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;
		RL[PointA.Row] = PointA.Col;
		RL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_RIGHT, PointA.Row, PointB.Row);
		//补齐右线
		for (int i = PointB.Row + 1; i < PointC.Row; ++i)
		{
			if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
			{
				RightPnt.ErrRow = --i;
				RightPnt.ErrCol = RL[i];
				break;
			}
		}
		LeftPnt.ErrRow = PointC.Row;
		LeftPnt.ErrCol = PointC.Col;
		LeftPnt.ErrType = 1;
		RightPnt.ErrType = 2;
	}
}

//================================================================//
//  @brief  :		状态3补线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleThree(void)
{
	//FindLineNormal();
	if (1 == CircleFlag)	//	左环岛
	{
		PointB.Col = ArrayMax(LL, LeftPnt.ErrRow);
		PointB.Row = LeftPnt.ErrRow;
		//搜C点

		GetPointC();
		//搜D点
		GetPointD();
		//连接AB，CD
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;
		FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		RightPnt.ErrRow = PointC.Row;
		RightPnt.ErrCol = PointC.Col;
		RightPnt.ErrType = 1;
		LeftPnt.ErrType = 2;
	}
	else if (2 == CircleFlag)	//右环岛
	{
		PointB.Col = ArrayMin(RL, RightPnt.ErrRow);
		PointB.Row = RightPnt.ErrRow;
		//搜C点
		GetPointC();
		//搜D点
		GetPointD();
		//连接AB，CD
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;
		FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		LeftPnt.ErrRow = PointC.Row;
		LeftPnt.ErrCol = PointC.Col;
		LeftPnt.ErrType = 1;
		RightPnt.ErrType = 2;
	}
}

//================================================================//
//  @brief  :		状态4补线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFour(void)
{
	if (1 == CircleFlag)	//左环岛
	{
		if (!CircleLeftLost)		//左不丢线
		{
			//补左线
			for (int i = 1; i < IMG_ROWS; ++i)
			{
				if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
				{
					LeftPnt.ErrRow = --i;
					LeftPnt.ErrCol = LL[i];
					break;
				}
			}
		}
		else
		{
			LeftPnt.ErrRow = 0;
			LL[0] = LeftPnt.ErrCol = LEFT_EAGE;
		}
		PointB.Row = LeftPnt.ErrRow + 5;
		PointB.Col = LL[0] + STATE_FOUR_PB_EAGE_OFFSET;
		//搜C点
		GetPointC();
		str.Format("\r\n PointC = %d \r\n", PointC.Row); PrintDebug(str);
		str.Format("\r\n PointC = %d \r\n", PointC.Col); PrintDebug(str);
		//连接CD
		PointD.Row = 0;
		PointD.Col = LL[0] + STATE_FOUR_WIDTH;
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;

		//补齐右线
		for (int i = PointC.Row + 1; i < IMG_ROWS; ++i)
		{
			if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
			{
				RightPnt.ErrRow = --i;
				RightPnt.ErrCol = RL[i];
				break;
			}
		}
		if (PointC.Row > STATE_FOUR_FILLLINE_TYPE_TH)
			FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		else
			FillLineDown(FIND_RIGHT, PointC.Row);
		LeftPnt.ErrType = 2;
		RightPnt.ErrType = 1;
	}
	else if (2 == CircleFlag)		//右环岛
	{
		if (!CircleRightLost)		//右不丢线
		{
			//补右线
			for (int i = 1; i < IMG_ROWS; ++i)
			{
				if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
				{
					RightPnt.ErrRow = --i;
					RightPnt.ErrCol = RL[i];
					break;
				}
			}
		}
		else
		{
			RightPnt.ErrRow = 0;
			RL[0] = RightPnt.ErrCol = RIGHT_EAGE;
		}
		PointB.Row = RightPnt.ErrRow + 5;
		PointB.Col = RL[0] - STATE_FOUR_PB_EAGE_OFFSET;
		//搜C点
		GetPointC();
		str.Format("\r\n PointC = %d \r\n", PointC.Row); PrintDebug(str);
		str.Format("\r\n PointC = %d \r\n", PointC.Col); PrintDebug(str);
		//连接CD
		PointD.Row = 0;
		PointD.Col = RL[0] - STATE_FOUR_WIDTH;
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;

		//补齐左线
		for (int i = PointC.Row + 1; i < IMG_ROWS; ++i)
		{
			if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
			{
				LeftPnt.ErrRow = --i;
				LeftPnt.ErrCol = LL[i];
				break;
			}
		}
		if (PointC.Row > STATE_FOUR_FILLLINE_TYPE_TH)			//选择向下补线与两点连线 取决于上部分线的质量
			FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		else
			FillLineDown(FIND_LEFT, PointC.Row);
		LeftPnt.ErrType = 1;
		RightPnt.ErrType = 2;
	}
}

//================================================================//
//  @brief  :		状态5补线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFive(void)
{
	if (1 == CircleFlag)	//左环岛
	{
		if (CircleLeftLost)
		{
			LeftPnt.ErrType = 7;
			FindLineLost();
		}
		else FindLineNormal();
		str.Format("\r\n FiveCircleLeftPnt = %d \r\n", 1); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", LeftPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", LeftPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", LeftPnt.ErrType); PrintDebug(str);
		str.Format("\r\n FiveCircleRightPnt = %d \r\n", 0); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", RightPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", RightPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", RightPnt.ErrType); PrintDebug(str);
		//发现外跳
		if (2 == RightPnt.ErrType)
		{
			int middle = (LL[0] + RL[0]) >> 1;
			int i = 0;
			for (i = 0;i < IMG_ROWS; ++i)
			{
				if (IsEageCol(i, middle, 3, 1))
				{
					RL[--i] = middle;
					break;
				}
			}
			FillLinePnt(FIND_RIGHT, RightPnt.ErrRow, i);
			RightPnt.ErrRow = i;
			RightPnt.ErrCol = middle;
			RightPnt.ErrType = 1;
			LeftPnt.ErrType = 2;
		}
	}
	else if (2 == CircleFlag)
	{
		if (CircleRightLost)
		{
			str.Format("\r\n you = %d \r\n", 2); PrintDebug(str);
			RightPnt.ErrType = 7;
			FindLineLost();
		}
		else FindLineNormal();
		str.Format("\r\n FiveCircleLeftPnt = %d \r\n", 1); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", LeftPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", LeftPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", LeftPnt.ErrType); PrintDebug(str);
		str.Format("\r\n FiveCircleRightPnt = %d \r\n", 0); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", RightPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", RightPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", RightPnt.ErrType); PrintDebug(str);
		//发现外跳
		if (2 == LeftPnt.ErrType)
		{
			int middle = (LL[0] + RL[0]) >> 1;
			int i = 0;
			for ( i = 0; i < IMG_ROWS; ++i)
			{
				if (IsEageCol(i, middle, 3, 1))
				{
					LL[--i] = middle;
					break;
				}
			}
			FillLinePnt(FIND_LEFT, LeftPnt.ErrRow, i);
			LeftPnt.ErrRow = i;
			LeftPnt.ErrCol = middle;
			LeftPnt.ErrType = 1;
			RightPnt.ErrType = 2;
		}
	}
}

//================================================================//
//  @brief  :		状态6补线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleSix(void)
{
	if (1 == CircleFlag)	//左环岛
	{
		if (CircleLeftLost)
		{
			int i;
			for (i = 0; i < STATE_SIX_FINDLINE_TYPE_TH; ++i)
			{
				if (IsEageCol(i, LEFT_EAGE, 3, 1))
				{
					GetPointE(--i, LEFT_EAGE);
					break;
				}
			}
			if (STATE_SIX_FINDLINE_TYPE_TH == i)	//未找到E点
			{
				LeftPnt.ErrType = 7;
				FindLineLost();
			}
			else			//找到E点
			{
				LL[PointE.Row] = PointE.Col;
				for (int i = PointE.Row + 1; i < IMG_ROWS; ++i)
				{
					if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
					{
						LeftPnt.ErrRow = --i;
						LeftPnt.ErrCol = LL[i];
						LeftPnt.ErrType = 2;
						FillLineDown(FIND_LEFT, PointE.Row);
						break;
					}
				}
				for (int i = 1; i < IMG_ROWS; ++i)
				{
					if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
					{
						RightPnt.ErrRow = --i;
						RightPnt.ErrCol = RL[i];
						RightPnt.ErrType = 1;
						break;
					}
				}
			}
		}
		else FindLineNormal();
	}
	else if (2 == CircleFlag)	//右环岛
	{
		if (CircleRightLost)
		{
			int i;
			for (i = 0; i < STATE_SIX_FINDLINE_TYPE_TH; ++i)
			{
				if (IsEageCol(i, RIGHT_EAGE, 3, 1))
				{
					str.Format("\r\n I = %d \r\n", i); PrintDebug(str);
					GetPointE(--i, RIGHT_EAGE);
					str.Format("\r\n PointE = %d \r\n", PointE.Row); PrintDebug(str);
					str.Format("\r\n PointE = %d \r\n", PointE.Col); PrintDebug(str);
					break;
				}
			}
			if (STATE_SIX_FINDLINE_TYPE_TH == i)	//未找到E点
			{
				RightPnt.ErrType = 7;
				FindLineLost();
			}
			else			//找到E点
			{
				RL[PointE.Row] = PointE.Col;
				for (int i = PointE.Row + 1; i < IMG_ROWS; ++i)
				{
					if (1 != FindL(FIND_RIGHT, i, i - 1, AutoValue(i)))
					{
						RightPnt.ErrRow = --i;
						RightPnt.ErrCol = RL[i];
						RightPnt.ErrType = 2;
						FillLineDown(FIND_RIGHT, PointE.Row);
						break;
					}
				}
				for (int i = 1; i < IMG_ROWS; ++i)
				{
					if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
					{
						LeftPnt.ErrRow = --i;
						LeftPnt.ErrCol = LL[i];
						LeftPnt.ErrType = 1;
						break;
					}
				}
			}
		}
		else FindLineNormal();
	}
}

//================================================================//
//  @brief  :		获取B点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointB(void)
{
	if (1 == CircleFlag)	//左环岛
	{
		int Eage_1 = 0;
		for (int i = PointA.Row; i < RightPnt.ErrRow; ++i)
		{
			if (IsEageCol(i, PointA.Col, 3, 1))
			{
				Eage_1 = i;
				break;
			}
		}
		for (int j = PointA.Col - 1; j < RL[0]; ++j)
		{
			int i;
			for (i = Eage_1; i < Eage_1 + GETPOINTB_TH; ++i)
			{
				if (IsEageCol(i, j, 3, 1))
				{
					Eage_1 = i;
					break;
				}

			}
			if (GETPOINTB_TH + Eage_1 == i)		//找到B点
			{
				PointB.Row = Eage_1;
				PointB.Col = j - 1;
				break;
			}
		}
	}
	else if (2 == CircleFlag)	//右环岛
	{
		int Eage_1 = 0;
		for (int i = PointA.Row; i < LeftPnt.ErrRow; ++i)
		{
			if (IsEageCol(i, PointA.Col, 3, 1))
			{
				Eage_1 = i;
				break;
			}
		}
		for (int j = PointA.Col - 1; j > LL[0]; --j)
		{
			int i;
			for (i = Eage_1; i < Eage_1 + GETPOINTB_TH; ++i)
			{
				if (IsEageCol(i, j, 3, 1))
				{
					Eage_1 = i;
					break;
				}

			}
			if (GETPOINTB_TH + Eage_1 == i)		//找到B点
			{
				PointB.Row = Eage_1;
				PointB.Col = j + 1;
				break;
			}
		}
	}
}

//================================================================//
//  @brief  :		获取C点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointC(void)
{
	int Eage_1 = 0, Eage_2 = PointB.Col;
	for (int i = PointB.Row + 3; i < IMG_ROWS; ++i)
	{
		if (IsEageCol(i, PointB.Col, 3, 1))
		{
			Eage_1 = i - 1;
			break;
		}
	}
	int k;
	if (1 == CircleFlag)
	{
		for (k = Eage_2; k < RL[Eage_1]; ++k)
			if (IsEage(FIND_RIGHT, Eage_1, k, 3, 1))
			{
				Eage_2 = k;
				break;
			}
		if (RL[Eage_1] == k)		//找到C点
		{
			PointC.Row = Eage_1;
			PointC.Col = Eage_2;
			return;
		}
		else
		{
			for (int i = Eage_1 - 1; i >= 0; --i)
			{
				int j = 0;
				for (j = Eage_2; j < Eage_2 + GETPOINTC_TH; ++j)
				{
					if (IsEage(FIND_RIGHT, i, j, 3, 1))
					{
						Eage_2 = j;
						break;
					}
				}
				if (Eage_2 + GETPOINTC_TH == j)		//找到C点
				{
					PointC.Row = i;
					PointC.Col = Eage_2;
					return;
				}
			}
		}
	}
	else if (2 == CircleFlag)
	{
		for (k = Eage_2; k > LL[Eage_1]; --k)
			if (IsEage(FIND_LEFT, Eage_1, k, 3, 1))
			{
				Eage_2 = k;
				break;
			}
		if (LL[Eage_1] == k)		//找到C点
		{
			PointC.Row = Eage_1;
			PointC.Col = Eage_2;
			return;
		}
		else
		{
			for (int i = Eage_1 - 1; i >= 0; --i)
			{
				int j = 0;
				for (j = Eage_2; j > Eage_2 - GETPOINTC_TH; --j)
				{
					if (IsEage(FIND_LEFT, i, j, 3, 1))
					{
						Eage_2 = j;
						break;
					}
				}
				if (Eage_2 - GETPOINTC_TH == j)		//找到C点
				{
					PointC.Row = i;
					PointC.Col = Eage_2;
					return;
				}
			}
		}
	}
}

//================================================================//
//  @brief  :		获取D点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointD(void)
{
	if (1 == CircleFlag)
	{
		for (int j = PointB.Col; j < RL[0]; ++j)
		{
			if (IsEage(FIND_RIGHT, PointB.Row, j, 3, 1))
			{
				PointD.Row = PointB.Row;
				PointD.Col = j;
				return;
			}
		}
	}
	else if (2 == CircleFlag)
	{
		for (int j = PointB.Col; j > LL[0]; --j)
		{
			if (IsEage(FIND_LEFT, PointB.Row, j, 3, 1))
			{
				PointD.Row = PointB.Row;
				PointD.Col = j;
				return;
			}
		}
	}
}

//================================================================//
//  @brief  :		获取E点
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointE(int row, int col)
{
	if (1 == CircleFlag)	//左环岛
	{
		int Eage = row - 1;
		int i = Eage, j = col;
		while (i < Eage + GETPOINTE_TH)
		{
			if (IsEage45(FIND_LEFT, i, j))
			{
				PointE.Row = i;
				PointE.Col = j;
				Eage = --i;
				continue;
			}
			++i;++j;
		}
		if (GETPOINTE_TH + Eage == i)		//找到E点
		{
			return;
		}
	}
	else if (2 == CircleFlag)	//右环岛
	{
		int Eage = row - 1;
		int i = Eage, j = col;
		while (i < Eage + GETPOINTE_TH)
		{
			if (IsEage45(FIND_RIGHT, i, j))
			{
				PointE.Row = i;
				PointE.Col = j;
				Eage = --i;
				continue;
			}
			++i;--j;
		}
		if (GETPOINTE_TH + Eage == i)		//找到E点
		{
			return;
		}
	}
}

//================================================================//
//  @brief  :		0-1-0状态变换
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int StateChange(int a)
{
	static int c = 0;
	static int LastState = -1;

	if (-1 == LastState)
	{
		LastState = a;
	}
	else if (a != LastState)
	{
		++c;
	}
	else return 0;

	if (2 == c)
	{
		c = 0;
		LastState = -1;
		return 1;
	}
}

//================================================================//
//  @brief  :		斜45度搜边界
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int IsEage45(int type, int row, int col)
{
	if (FIND_LEFT == type)		//向右上
	{
		if ((ImgBuff[(127 - row + FRONT_LINE) * 40 + (col >> 3)] & Bit[col & 7]) == 0x00
			&& (ImgBuff[(126 - row + FRONT_LINE) * 40 + ((col + 1) >> 3)] & Bit[(col + 1) & 7]) == Bit[(col + 1) & 7])
			return 1;
		else return 0;
	}
	else if (FIND_RIGHT == type)	//向左上搜
	{
		if ((ImgBuff[(127 - row + FRONT_LINE) * 40 + (col >> 3)] & Bit[col & 7]) == 0x00
			&& (ImgBuff[(126 - row + FRONT_LINE) * 40 + ((col - 1) >> 3)] & Bit[(col - 1) & 7]) == Bit[(col - 1) & 7])
			return 1;
		else return 0;
	}
}

//================================================================//
//  @brief  :		记录点初始化
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void PointInit(void)
{
	//PointA.Row = PointA.Col = 0;
	PointB.Row = PointB.Col = 0;
	PointC.Row = PointC.Col = 0;
	PointD.Row = PointD.Col = 0;
	PointE.Row = PointE.Col = 0;
}