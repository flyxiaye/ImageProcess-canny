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
//  @brief  :		���в��ú���
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	FirstLineV4();
}
//================================================================//
//  @brief  :		��ͨ��ͼ������
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
#if STOPLINE
		//ʶ��ͣ��
		if (Img_StopOpen && LeftIntLine < UP_EAGE + 15 && RightIntLine < UP_EAGE + 15
			&& LeftPnt.ErrRow <= UP_EAGE + 15 && RightPnt.ErrRow <= UP_EAGE + 15
			&& !StopLineFlag && DistStopLine(UP_EAGE + 15))
		{
			StopLineFlag = 1;
			SpecialElemFlag = 1;
		}
#endif
#if RAMP
		//ʶ���µ�
		if (Img_RampOpen && !SpecialElemFlag
			&& LeftPnt.ErrRow <= UP_EAGE + 1 && RightPnt.ErrRow <= UP_EAGE + 1)
		{
			if (IsRamp())
			{
				RampFlag = 1;
				SpecialElemFlag = 1;
			}
		}
#endif
#if CURVE_BROKEN
		//�����·
		if (LeftPnt.ErrRow - RightPnt.ErrRow < 5 && RightPnt.ErrRow - LeftPnt.ErrRow < 5
			&& LeftPnt.ErrCol - RightPnt.ErrCol < 10 && RightPnt.ErrCol - LeftPnt.ErrCol < 10)
		{
			if (JudgeSpecialLine(LeftPnt.ErrRow, RightPnt.ErrRow, 1))
			{
				BrokenFlag = 3;
				SpecialElemFlag = 1;
			}
			else BrokenFlag = 0;
		}
		else BrokenFlag = 0;
#endif

#if CIRCLE == 1
		if (RightPnt.Type == 2 && RightPnt.ErrRow - LeftPnt.ErrRow >= CIRCLEUP_TH && LeftPnt.ErrRow <= UP_EAGE + 20
			&& RightPnt.ErrCol < RIGHT_EAGE - 25)	//�һ����ж�
		{
			CircleFlag = IsCircleIsland(CR);
			string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
			if (CircleFlag)		//�ǻ��� ������ͼ
			{
				CircleState = 1;
				GetPointA();
				GetPointB();
				GetPointC();
				GetPointD();
				FillLineAB();
				FillLineCD();
				FillAllEage();
			}
			else;
		}
		else if (LeftPnt.Type == 2 && LeftPnt.ErrRow - RightPnt.ErrRow >= CIRCLEUP_TH && RightPnt.ErrRow <= UP_EAGE + 20
			&& LeftPnt.ErrCol > LEFT_EAGE + 25)	//�󻷵��ж�
		{
			CircleFlag = IsCircleIsland(CL);
			string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
			if (CircleFlag)		//�ǻ��� ������ͼ
			{
				CircleState = 1;
				GetPointA();
				GetPointB();
				GetPointC();
				GetPointD();
				FillLineAB();
				FillLineCD();
				FillAllEage();
			}
			else;
		}
		else
#elif CIRCLE == 2
		if (Img_CircleOpen && !SpecialElemFlag
			&& LeftPnt.Type == 2 && LeftPnt.ErrRow > UP_EAGE + 20 && RightPnt.ErrRow < UP_EAGE + 10
			&& LeftPnt.ErrCol < MIDDLE && RightPnt.ErrCol > MIDDLE - 7 && IsCircleIsland(CL))
		{
			//CircleFlag = CL;
			CircleState = 1;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else if (Img_CircleOpen && !SpecialElemFlag
			&& RightPnt.Type == 2 && RightPnt.ErrRow > UP_EAGE + 20 && LeftPnt.ErrRow < UP_EAGE + 10
			&& RightPnt.ErrCol > MIDDLE && LeftPnt.ErrCol < MIDDLE + 7 && IsCircleIsland(CR))
		{
			//CircleFlag = CR;
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
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)		//ʮ�ֲ�ͼ
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow > 10 || RightPnt.ErrRow - LeftPnt.ErrRow > 10)
				{
					FillBevelCross();
				}
				else
				{
					FillLevelCross();
				}
				FindLineNormal(0);

			}
#if BLOCK_BROKEN
		//ʶ������Ԫ��
		if ((Img_BlockOpen || Img_BrokenOpen) && !SpecialElemFlag)		//��··���ж�
		{
			int flag = JudgeSpecialElem(LeftIntLine, RightIntLine);
			if (1 == Img_BlockOpen && 1 == flag)
			{
				BlockFlag = 1;
				SpecialElemFlag = 1;
			}
			else if (2 == Img_BlockOpen && flag)			//����ʶ��·��
			{
				BrokenFlag = 1;
				SpecialElemFlag = 1;
#if INF
				if (g_inf > stop_inf)
				{
					BlockFlag = 1;
					BrokenFlag = 0;
				}
#endif 
			}
			else if (Img_BrokenOpen && 2 == flag)
			{
				BrokenFlag = 1;
				SpecialElemFlag = 1;
			}
			else;
		}
#endif
	}
	if (1 == g_RoadType)
	{
		FindLineLost();
#if CIRCLE == 2
		if (Img_CircleOpen && !SpecialElemFlag
			&& LL[DOWN_EAGE] == LEFT_EAGE && RightPnt.ErrRow < UP_EAGE + 10 && RightPnt.ErrCol > MIDDLE)
		{
			CircleFlag = CL;
			CircleState = 2;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
		}
		else if (Img_CircleOpen && !SpecialElemFlag
			&& RL[DOWN_EAGE] == RIGHT_EAGE && LeftPnt.ErrRow < UP_EAGE + 10 && LeftPnt.ErrCol < MIDDLE)
		{
			CircleFlag = CR;
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
//  @brief  :		��ͼ������
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
	//����У��
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40 || ML_Count > DOWN_EAGE - 20		//�±߽��С����Ч��������
		|| RightPnt.ErrCol - LeftPnt.ErrCol > 100)									//�ϱ߽粻����
	{
		ErrorFlag = 4;
	}
	if (!ErrorFlag)
		SpeedRow = GetSpeedRow(ML[DOWN_EAGE], LeftPnt.ErrRow, RightPnt.ErrRow);

	string.Format("\r\n ErrorFlag = %d \r\n", ErrorFlag); PrintDebug(string);
	/*string.Format("\r\n StopLine = %d \r\n", StopLineFlag); PrintDebug(string);
	string.Format("\r\n SpeedRow = %d \r\n", SpeedRow); PrintDebug(string);
	string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
	string.Format("\r\n BrokenFlag = %d \r\n", BrokenFlag); PrintDebug(string);
	string.Format("\r\n BlockFlag = %d \r\n", BlockFlag); PrintDebug(string);
	string.Format("\r\n RampFlag = %d \r\n", RampFlag); PrintDebug(string);
	string.Format("\r\n SpecElemFlag = %d \r\n", SpecialElemFlag); PrintDebug(string);*/
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

