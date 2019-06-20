#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "MainProcess.h"
#include "CircleIsland.h"
#include "FirstLineProcess.h"
#include "canny.h"
#include "SpecialElem.h"

//================================================================//
//  @brief  :		首行搜线采用函数
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	FirstLineV4();
}
//================================================================//
//  @brief  :		普通补图主程序
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void MainFill(void)
{
	VarInit();
	SelectFirstLine();
	g_RoadType = FirstRowProcess();
	string.Format("\r\n RoadType = %d \r\n", g_RoadType); PrintDebug(string);
	if (0 == g_RoadType)
	{
		FindLineNormal(1);
		string.Format("\r\n 0x01LeftPnt = %d %d %d \r\n", LeftPnt.Type, LeftPnt.ErrRow, LeftPnt.ErrCol); PrintDebug(string);
		string.Format("\r\n 0x01RightPnt = %d %d %d \r\n", RightPnt.Type, RightPnt.ErrRow, RightPnt.ErrCol); PrintDebug(string);
		PointE.Row = LeftPnt.ErrRow; PointE.Col = LeftPnt.ErrCol;
		PointF.Row = RightPnt.ErrRow; PointF.Col = RightPnt.ErrCol;

		ImgJudgeStopLine();		//识别停车
		ImgJudgeRamp();			//识别坡道
		ImgJudgeCurveBroken();	//弯道断路
#if CIRCLE == 2
		CircleFlag = ImgJudgeCircle(0);
		if (CL == CircleFlag)
		{
			//CircleFlag = CN;
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else if (CR == CircleFlag)
		{
			//CircleFlag = CN;
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else
#endif // CIRCLE
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)		//十字补图
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow > 10 || RightPnt.ErrRow - LeftPnt.ErrRow > 10)
					FillBevelCross();
				else
					FillLevelCross();
				FindLineNormal(0);

			}
		ImgJudgeBlock();		//识别路障
	}
	if (1 == g_RoadType)
	{
		FindLineLost();
#if CIRCLE == 2
		CircleFlag = ImgJudgeCircle(1);
		if (CL == CircleFlag)
		{
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else if (CR == CircleFlag)
		{
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else
#endif
			if (1 == g_RoadType && 2 == LeftPnt.Type && 2 == RightPnt.Type)
			{
				FillBevelCross();
				FindLineNormal(0);
			}
	}
	if (2 == g_RoadType)
	{
		FillFourCross();
		FindLineNormal(0);
	}
	FillMiddleLine();
}


//================================================================//
//  @brief  :		补图主程序
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetML(void)
{
	CannyEage();
#if CIRCLE
	if (CircleFlag)		//is CircleIsland 
	{
		CircleFill();
	}
	else
#endif // CIRCLE
	{
		if (SpecialElemFlag)
			SpecialElemFill();
		if (!SpecialElemFlag)
			MainFill();
	}

	//中线校验
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40 || ML_Count > DOWN_EAGE - 20		//下边界过小，有效行数过低
		|| RightPnt.ErrCol - LeftPnt.ErrCol > 100)									//上边界不收敛
	{
		ErrorFlag = 4;
	}
	if (!ErrorFlag)
		SpeedRow = GetSpeedRow(ML[DOWN_EAGE], LeftPnt.ErrRow, RightPnt.ErrRow);



	string.Format("\r\n ErrorFlag = %d \r\n", ErrorFlag); PrintDebug(string);
	//string.Format("\r\n StopLine = %d \r\n", StopLineFlag); PrintDebug(string);
	//string.Format("\r\n SpeedRow = %d \r\n", SpeedRow); PrintDebug(string);
	string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
	string.Format("\r\n CircleState = %d \r\n", CircleState); PrintDebug(string);
	//string.Format("\r\n BrokenFlag = %d \r\n", BrokenFlag); PrintDebug(string);
	//string.Format("\r\n BlockFlag = %d \r\n", BlockFlag); PrintDebug(string);
	//string.Format("\r\n RampFlag = %d \r\n", RampFlag); PrintDebug(string);
	//string.Format("\r\n SpecElemFlag = %d \r\n", SpecialElemFlag); PrintDebug(string);
	string.Format("\r\n ML_count = %d \r\n", ML_Count); PrintDebug(string);
	string.Format("\r\n ML->count = %d \r\n", ML[ML_Count]); PrintDebug(string);

	for (int i = 0; i < 120; i++)
	{
		ImageData[i][LL[i]] = 254;
		ImageData[i][ML[i]] = 255;
		ImageData[i][RL[i]] = 254;
	}
	for (int i = 0; i < 30; i++)
		ImageData[LeftIntLine][i] = 100;
	for (int i = 187; i > 158; i--)
		ImageData[RightIntLine][i] = 128;
}

