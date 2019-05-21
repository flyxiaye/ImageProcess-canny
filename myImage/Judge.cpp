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

unsigned char JudgeArray[11][11] =
{
	//列为左 行为右
	0,	 0,	  0,   0,   0,   0,   0,   1,   2, 255,   7,
	0,   8,  11,  11,  11,  11,  11,   1,   2, 255,   7,
	0,  11,   9,  10,  11,  10, 255,   1,   2, 255,   7,
	0,  11,  10, 255, 255,  10,  11,   1,   2, 255,   7,
	0,  11,  11, 255,  11,  11, 255,   1,   2, 255,   7,
	0,  11,  11, 255, 255,  11, 255,   1,   2, 255,   7,
	0,  11,  10,  10,  11,   2,  11,   1,   2, 255,   7,
	1,   1,   1,   1,   1,   1,   1,   1,   2, 255,   7,
	2,   2,   2,   2,   2,   2,   2,   2,   2, 255,   7,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   7,
	7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7
};
unsigned char LastType = 0xff;
int NumberI;

//================================================================//
//  @brief  :		main
//  @param  :		void
//  @return :		-1错误
//  @note   :		void
//================================================================//
int FindML(void)
{
	for (int i = 0; i < IMG_ROWS; i++) {
		for (int j = 0; j < IMG_COLS; j++) {

			ImageData[i][j] = ImageData[i][j] > 50 ? 255 : 0;

		}
	}

	//二值化
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 24; j++) {
			ImgBuff[i * 24 + j] = 0;
			for (int k = 0; k < 8; k++) {
				ImgBuff[i * 24 + j] += (ImageData[i + 25][j * 8 + k] > 0) ? WHITE : BLACK;
				if (k != 7)
					ImgBuff[i * 24 + j] = ImgBuff[i * 24 + j] << 1;
			}
		}
	}

	//寻线失败返回0 函数异常返回-1；
	//返回0做个标记 采用另一套补线

	VarInit();
	FindTenLine();
	TenType();
	//调试=============================//
	//int tmpc;
	//FillLevelCross_4();
	////FindLineLost();
	//FindLineNormal();
	//tmpc = FillLevelCross();
	/////*FillBevelCross();
	////FindLineNormal();
	////FillBevelCross();
	//FillLineUp(FIND_LEFT, LeftPnt.ErrRow);
	//FillLineUp(FIND_RIGHT, RightPnt.ErrRow);
	//str.Format("\r\n tmpc = %d \r\n", tmpc); PrintDebug(str);
	//str.Format("\r\n LeftPnt = %d \r\n", 1); PrintDebug(str);
	//str.Format("\r\n	Row = %d \r\n", LeftPnt.ErrRow); PrintDebug(str);
	//str.Format("\r\n	Col = %d \r\n", LeftPnt.ErrCol); PrintDebug(str);
	//str.Format("\r\n	Type = %d \r\n", LeftPnt.ErrType); PrintDebug(str);
	//str.Format("\r\n RightPnt = %d \r\n", 0); PrintDebug(str);
	//str.Format("\r\n	Row = %d \r\n", RightPnt.ErrRow); PrintDebug(str);
	//str.Format("\r\n	Col = %d \r\n", RightPnt.ErrCol); PrintDebug(str);
	//str.Format("\r\n	Type = %d \r\n", RightPnt.ErrType); PrintDebug(str);
	//===========================================================================//
	LastType = 0xff;
	int N = 0;
	NumberI = 0;
	while (1)
	{
		++NumberI;				//进入循环次数
		int Tmp = 0;
		unsigned char SideType = Judge();

		if (0xff == SideType)
			break;
		if (LastType == SideType)
			++N;
		if (N >= 5)
		{
			ErrorFlag = 1;
			break;
		}

		str.Format("\r\n SideType = %d \r\n", SideType); PrintDebug(str);
		switch (SideType & 0x0f)
		{
		case 0x00:
			FindLineNormal();
			break;
		case 0x01:
			FindLineLost();
			break;
		case 0x02:
			if (-1 == FillLevelCross_4())
				ErrorFlag = 7;
			break;
		case 0x07:					//环岛
			FillCircleIsland();
			break;
		case 0x08:
			if (-1 == FillLineUp(FIND_LEFT, LeftPnt.ErrRow))
				ErrorFlag = 2;
			if (-1 == FillLineUp(FIND_RIGHT, RightPnt.ErrRow))
				ErrorFlag = 1;
			break;
		case 0x09:
			Tmp = FillLevelCross();
			if (-1 == Tmp)
				ErrorFlag = 3;
			else if (0 == Tmp)
				FillFlag = 1;
			break;
		case 0x0a:
			BevelCrossFlag = 1;
			if (-1 == FillBevelCross())
				ErrorFlag = 4;
			break;
		case 0x0b:
			FillMiddleLine();
			break;
		case 0x0c:
			if (-1 == FillLineUp(FIND_LEFT, LeftPnt.ErrRow))
				ErrorFlag = 5;
			break;
		case 0x0d:
			if (-1 == FillLineUp(FIND_RIGHT, RightPnt.ErrRow))
				ErrorFlag = 6;
			break;
		case 0x0e:
			StopFill();
			break;
		default:
			break;
		}
		str.Format("\r\n LeftPnt = %d \r\n", 1); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", LeftPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", LeftPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", LeftPnt.ErrType); PrintDebug(str);
		str.Format("\r\n RightPnt = %d \r\n", 0); PrintDebug(str);
		str.Format("\r\n	Row = %d \r\n", RightPnt.ErrRow); PrintDebug(str);
		str.Format("\r\n	Col = %d \r\n", RightPnt.ErrCol); PrintDebug(str);
		str.Format("\r\n	Type = %d \r\n", RightPnt.ErrType); PrintDebug(str);
		//break;
		if (ErrorFlag)
		{
			str.Format("\r\n ErrorFlag = %d \r\n", ErrorFlag); PrintDebug(str);
			break;
		}
		LastType = SideType;
		//高速标志
		if (1 == NumberI)
			if (120 < LeftPnt.ErrRow && 120 < RightPnt.ErrRow)
				MaxSpeedFlag = 1;
			else MaxSpeedFlag = 0;
	}


	int max = MAX(LeftPnt.ErrRow, RightPnt.ErrRow);
	Mid_Filtering(ML, max);			//中线滤波
	////判断赛道类型
	//int cur = Curvature(ML, max);
	//int kLeast = LeastError(ML, 75);
	//str.Format("\r\n curva = %d \r\n", cur); PrintDebug(str);
	//str.Format("\r\n k = %d \r\n", kLeast); PrintDebug(str);

	//=========================控制==================================//
	str.Format("\r\n FT = %d \r\n", FirstRoadType); PrintDebug(str);
	int Error = WeightAve(95, max, ML);
	int max_row = SpeedC_3(LL, RL);
	int ErrorAll = WeightAveAll(ML, max);
	int ErrorSome = WeightAveSome(ML, 95, max);
	str.Format("\r\n ErrorAll = %d \r\n", ErrorAll); PrintDebug(str);
	str.Format("\r\n ErrorSome = %d \r\n", ErrorSome); PrintDebug(str);
	str.Format("\r\n Error = %d \r\n", Error); PrintDebug(str);
	str.Format("\r\n Max_row = %d \r\n", max_row); PrintDebug(str);
	//显示图像
	for (int i = 85; i >= 25; i--)
	{
		if (LL[i] < RIGHT_EAGE && LL[i] > LEFT_EAGE)
		{
			ImageData[i][LL[i]] = 100;
		}
		if (RL[i] < RIGHT_EAGE && RL[i] > LEFT_EAGE)
		{
			ImageData[i][RL[i]] = 128;
		}
		if (ML[i] < RIGHT_EAGE && ML[i] > LEFT_EAGE)
		{
			ImageData[i][ML[i]] = 254;
		}
	}
}

//================================================================//
//  @brief  :		判断函数
//  @param  :		void
//  @return :		SideType
//  @note   :		通过给与点类型判断采取动作
//================================================================//
unsigned char Judge(void)
{
	//SpecialPntDeal();

	unsigned char SideType = 0x00;
	SideType = JudgeArray[RightPnt.ErrType][LeftPnt.ErrType];

	//特殊标志位
	if (StopFillFlag)				//停车标志
	{
		SideType = 14;
		StopFillFlag = 0;
	}
	else if (2 == NumberI && 0 == LastType)			//环岛判断
	{
		str.Format("\r\n you = %d \r\n", 123); PrintDebug(str);
		if (LeftPnt.ErrRow - RightPnt.ErrRow > 30 && RightPnt.ErrRow > 55 && RightPnt.ErrCol < RIGHT_EAGE - 80)	//疑似右环岛
		{
			CircleFlag = IsCircleIsland(FIND_RIGHT);
			if (2 == CircleFlag)
			{
				SideType = 7;
				CircleState = 1;
			}
		}
		else if (RightPnt.ErrRow - LeftPnt.ErrRow > 30 && LeftPnt.ErrRow > 55 && LeftPnt.ErrCol > LEFT_EAGE + 80)	//疑似左环岛
		{
			CircleFlag = IsCircleIsland(FIND_LEFT);
			if (1 == CircleFlag)
			{
				SideType = 7;
				CircleState = 1;
			}
		}
		else;
	}
	else if (FillFlag)
	{
		SideType = 8;
		FillFlag = 0;
	}
	else if (BevelCrossFlag && 10 == SideType)
	{
		SideType = 8;
	}
	else if (8 == LastType)
	{
		SideType = 11;
	}

	//0普通寻线 1丢边寻线 2四类十字寻线
	//8向上补线 9平十字补线 10斜十字补线 11画中线 12左向上补线 13 右向上补线
	//0xff 错误标志

	return SideType;
}

//================================================================//
//  @brief  :		特殊点处理
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SpecialPntDeal(void)
{
	//左点
	if (LeftPnt.ErrCol < LEFT_EAGE + MaxValue)
		LeftPnt.ErrType = 5;
	if (LeftPnt.ErrCol == LEFT_EAGE && LeftPnt.ErrRow == 0)
		LeftPnt.ErrType = 7;

	//右点
	if (RightPnt.ErrCol > RIGHT_EAGE - MaxValue)
		RightPnt.ErrType = 6;
	if (RightPnt.ErrCol == RIGHT_EAGE && RightPnt.ErrRow == 0)
		RightPnt.ErrType = 7;
	return;
}

//================================================================//
//  @brief  :		识别刹车线
//  @param  :		n k的值
//  @return :		1是 0不是
//  @note   :		void
//================================================================//
int IsStopLine(int *array)
{
	int n = 0;		//记录有左右跳变情况的行数
	for (int i = 0; i < 10; ++i)
	{
		if (*(array + i) > 3)
			n++;
	}
	if (n > 1)
		return 1;
	else return 0;

}