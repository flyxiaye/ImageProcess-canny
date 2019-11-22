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
//  @brief  :		�������߲��ú���
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void SelectFirstLine(void)
{
	FirstLineV4();
}
//================================================================//
//  @brief  :		��ͨ�����ͼ������
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void MainFill(void)
{
	VarInit();			//������ʼ��
	SelectFirstLine();	
	g_RoadType = FirstRowProcess();	//�������ͳ����ж�
	string.Format("\r\n RoadType = %d \r\n", g_RoadType); PrintDebug(string);

	if (0 == g_RoadType)
	{
		FindLineNormal(1);
		string.Format("\r\n 0x01LeftPnt = %d %d %d \r\n", LeftPnt.Type, LeftPnt.ErrRow, LeftPnt.ErrCol); PrintDebug(string);
		string.Format("\r\n 0x01RightPnt = %d %d %d \r\n", RightPnt.Type, RightPnt.ErrRow, RightPnt.ErrCol); PrintDebug(string);

		ImgJudgeStopLine();		//ʶ��ͣ��
		ImgJudgeObstacle();     //ʶ���µ�·��ֱ����·					
		ImgJudgeCurveBroken();	//�����·

#if CIRCLE == 2					//�����ж�
		CircleFlag = Img_JudgeCircleIsland(0);
		if (CL == CircleFlag)
		{
			int tmp_row = LeftPnt.ErrRow;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			if (tmp_row > DOWN_EAGE - 10)
				;
			else CircleFlag = CN;
		}
		else if (CR == CircleFlag)
		{
			int tmp_row = RightPnt.ErrRow;
			GetPointA();
			GetPointB();
			GetPointC();
			GetPointD();
			FillLineAB();
			FillLineCD();
			FillAllEage();
			if (tmp_row > DOWN_EAGE - 10)
				;
			else CircleFlag = CN;
		}
		else
#endif // CIRCLE
			if (LeftPnt.Type == 2 && RightPnt.Type == 2)		//ʮ�ֲ�ͼ
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow > 10 || RightPnt.ErrRow - LeftPnt.ErrRow > 10)
					FillBevelCross();			//бʮ��
				else
					FillLevelCross();			//ƽʮ��
				FindLineNormal(0);

			}
	}
	if (1 == g_RoadType)
	{
		FindLineLost_1();
#if CIRCLE == 2
		CircleFlag = Img_JudgeCircleIsland(1);
		if (CL == CircleFlag)
		{
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
		CircleFill();		//��������
	}
	else
#endif // CIRCLE
	{
		if (Img_SpecialElemFlag)
			SpecialElemFill();		//����Ԫ�ش���
		if (!Img_SpecialElemFlag)
			MainFill();				//��ͨ�������
	}

	//����У����ƣ�����У��
	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] <= 40 || ML_Count > DOWN_EAGE - 20		//�±߽��С����Ч��������
		|| RightPnt.ErrCol - LeftPnt.ErrCol > 100 || LeftPnt.ErrCol - RightPnt.ErrCol > 30)									//�ϱ߽粻����
	{
		ErrorFlag = 4;
	}
	if (!ErrorFlag)
		SpeedRow = GetSpeedRow(ML[DOWN_EAGE], LeftPnt.ErrRow, RightPnt.ErrRow);		//�ٿ�������Զ��



	string.Format("\r\n ErrorFlag = %d \r\n", ErrorFlag); PrintDebug(string);
	//string.Format("\r\n StopLine = %d \r\n", StopLineFlag); PrintDebug(string);
	//string.Format("\r\n SpeedRow = %d \r\n", SpeedRow); PrintDebug(string);
	string.Format("\r\n CircleFlag = %d \r\n", CircleFlag); PrintDebug(string);
	string.Format("\r\n CircleState = %d \r\n", CircleState); PrintDebug(string);
	string.Format("\r\n BrokenFlag = %d \r\n", Img_BrokenFlag); PrintDebug(string);
	string.Format("\r\n BlockFlag = %d \r\n", Img_BlockFlag); PrintDebug(string);
	string.Format("\r\n RampFlag = %d \r\n", Img_RampFlag); PrintDebug(string);
	string.Format("\r\n SpecElemFlag = %d \r\n", Img_SpecialElemFlag); PrintDebug(string);
	string.Format("\r\n ML_count = %d \r\n", ML_Count); PrintDebug(string);
	string.Format("\r\n ML->count = %d \r\n", ML[ML_Count]); PrintDebug(string);

}

//================================================================//
//  @brief  :		��ͼ�����򣨵�ţ�
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetML_Ind(void)
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
		VarInit();
		SelectFirstLine();
		if (2 == Img_BrokenFlag)
		{
			if (ImgJudgeOutBroken())
			{
				Img_BrokenFlag = 0;
			}
		}
		else if (Img_BrokenFlag)
		{
			if (ImgJudgeOutBroken())
			{
				Img_BrokenFlag = 2;
			}
		}
		else if (Img_StopLineFlag)
			;
		else if (Img_RampFlag)
			;
		else
		{
			Img_SpecialElemFlag = 0;		//	����Ԫ�ظ�λ
			FindLineNormal(0);
#if INF
			if (g_inf > stop_inf)
			{
				if (LeftPnt.ErrRow - RightPnt.ErrRow <= 2 && RightPnt.ErrRow - LeftPnt.ErrRow <= 2
					&& RightPnt.ErrCol - LeftPnt.ErrCol > 20)
				{
					int Front = MIN(LeftPnt.ErrRow, RightPnt.ErrRow);
					int FrontRompGray = RegionAveGray(Front - 10, LeftPnt.ErrCol + 5, RightPnt.ErrCol - 5);
					int FrontBlockGray = RegionAveGray(Front - 10, LeftPnt.ErrCol, RightPnt.ErrCol);
					int FrontBlockGray2 = RegionAveGray(Front - 2, LeftPnt.ErrCol, RightPnt.ErrCol);
					int DownGray = RegionAveGray(DOWN_EAGE - 2, LL[DOWN_EAGE - 2], RL[DOWN_EAGE - 2]);
					if (Img_BlockOpen && !Img_SpecialElemFlag
						&& DownGray - FrontBlockGray > DarkThreshold && DownGray - FrontBlockGray2 > DarkThreshold
						&& FrontRompGray - FrontBlockGray < 6 && FrontBlockGray - FrontRompGray < 6)
					{
						Img_BlockFlag = 1;//·��
						Img_SpecialElemFlag = 1;
	}
					else if (Img_RampOpen && !Img_SpecialElemFlag
						&& UP_EAGE + 1 == Front && DownGray - FrontRompGray < BrightThreshold && FrontRompGray - DownGray < BrightThreshold
						&& FrontRompGray - FrontBlockGray > 8)
					{
						Img_RampFlag = 1;//�µ�
						Img_SpecialElemFlag = 1;
					}
}
	}
			else
#endif 	
			if (Img_StraightBrokenOpen && !Img_SpecialElemFlag)		//ʶ��ֱ����·
			{
				if (ImgJudgeSpecialLine(LeftIntLine, LL[LeftIntLine], RightIntLine, RL[RightIntLine], 0, 45))
				{
					Img_BrokenFlag = 1;
					Img_SpecialElemFlag = 1;
				}
			}		
			ImgJudgeCurveBroken();			//ʶ�������·
			ImgJudgeStopLine();				//ʶ��ͣ����
		}
	}
}