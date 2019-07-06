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
#define MAXNUM 30
#define _type int
#include "queue.h"


//===========================以下为可直接调用的元素识别函数======================//

//================================================================//
//  @brief  :		环岛判断
//  @param  :		0判断状态1 1判断状态2 
//  @return :		void
//  @note   :		void
//================================================================//
int ImgJudgeCircle(int type)
{
	if (0 == type)
	{
		if (Img_CircleOpen && !Img_SpecialElemFlag
			&& LeftPnt.Type == 2 && LeftPnt.ErrRow > UP_EAGE + 20 && RightPnt.ErrRow < UP_EAGE + 10
			&& LeftPnt.ErrCol < MIDDLE && RightPnt.ErrCol > MIDDLE - 7 && IsCircleIsland(CL))
			return CL;
		else if (Img_CircleOpen && !Img_SpecialElemFlag
			&& RightPnt.Type == 2 && RightPnt.ErrRow > UP_EAGE + 20 && LeftPnt.ErrRow < UP_EAGE + 10
			&& RightPnt.ErrCol > MIDDLE && LeftPnt.ErrCol < MIDDLE + 7 && IsCircleIsland(CR))
			return CR;
		else return CN;
	}
	else
	{
		if (Img_CircleOpen && !Img_SpecialElemFlag
			&& LL[DOWN_EAGE] <= LEFT_EAGE + 3 && RightPnt.ErrRow < UP_EAGE + 10 && RightPnt.ErrCol > MIDDLE - 7
			&& RightPnt.ErrCol < RIGHT_EAGE - 30)
			return CL;
		else if (Img_CircleOpen && !Img_SpecialElemFlag
			&& RL[DOWN_EAGE] >= RIGHT_EAGE - 3 && LeftPnt.ErrRow < UP_EAGE + 10 && LeftPnt.ErrCol < MIDDLE + 7
			&& LeftPnt.ErrCol > LEFT_EAGE + 30)
			return CR;
		else return CN;
	}
}

//================================================================//
//  @brief  :		识别停车
//  @param  :		void 
//  @return :		void
//  @note   :		void
//================================================================//
void ImgJudgeStopLine(void)
{
#if STOPLINE
	if (Img_StopOpen && !Img_SpecialElemFlag
		&& LeftIntLine < UP_EAGE + 15 && RightIntLine < UP_EAGE + 15
		&& LeftPnt.ErrRow <= UP_EAGE + 15 && RightPnt.ErrRow <= UP_EAGE + 15
		&& !Img_StopLineFlag && DistStopLine(UP_EAGE + 15))
	{
		Img_StopLineFlag = 1;
		Img_SpecialElemFlag = 1;
	}
#endif
}


//================================================================//
//  @brief  :		识别弯道断路
//  @param  :		void 
//  @return :		void
//  @note   :		void
//================================================================//
void ImgJudgeCurveBroken(void)
{
#if CURVE_BROKEN	
	int UpRow = MAX(LeftPnt.ErrRow, RightPnt.ErrRow);
	if (!Img_SpecialElemFlag && Img_CurveBrokenOpen
		&& LeftPnt.ErrRow - RightPnt.ErrRow <= 3 && RightPnt.ErrRow - LeftPnt.ErrRow <= 3 && UpRow > 35
		&& ImgJudgeSpecialLine(LeftPnt.ErrRow, LeftPnt.ErrCol, RightPnt.ErrRow, RightPnt.ErrCol, 1))
	{
		int RoadWidth[IMG_ROW] = { 0 };                 //路宽
		int RoadWidthChange = 0;						//路宽变化率
		int DownRow = DOWN_EAGE;
		for (int i = UpRow; i <= DOWN_EAGE; i++)
		{
			RoadWidth[i] = RL[i] - LL[i];
			if (LL[i] - LEFT_EAGE < 10 || RIGHT_EAGE - RL[i] < 10)
			{
				DownRow = i;
				break;
			}
		}
		if (DownRow - UpRow < 5 || JudgeCurveEage(UpRow, DownRow, 1) || JudgeCurveEage(UpRow, DownRow, 2))
			RoadWidthChange = 0;
		else
			RoadWidthChange = (RoadWidth[DownRow] - (RightPnt.ErrCol - LeftPnt.ErrCol)) / (DownRow - UpRow);

		if (RoadWidthChange >= 3)
		{
			Img_BrokenFlag = 3;
			Img_SpecialElemFlag = 1;
			if ((LL[DownRow] - LEFT_EAGE) < 15)//左弯断路
			{
				Img_BrokenFlag = 4;
				//string.Format("\r\n LeftBroke \r\n"); PrintDebug(string);
				LeftPnt.ErrRow = DOWN_EAGE;
				LeftPnt.ErrCol = LEFT_EAGE;
				LeftPnt.Type = 2;
				RightPnt.Type = 1;
			}
			else if ((RIGHT_EAGE - RL[DownRow]) < 15)//右弯断路
			{
				Img_BrokenFlag = 5;
				//string.Format("\r\n RightBroke \r\n"); PrintDebug(string);
				RightPnt.ErrRow = DOWN_EAGE;
				RightPnt.ErrCol = RIGHT_EAGE;
				RightPnt.Type = 2;
				LeftPnt.Type = 1;
			}
		}
	}
#endif
}

//================================================================//
//  @brief  :		识别直道断路
//  @param  :		void 
//  @return :		void
//  @note   :		void
//================================================================//
void ImgJudgeStraightBroken(void)
{
#if STRAIGHT_BROKEN
	if (Img_StraightBrokenOpen && !Img_SpecialElemFlag)
	{
		if (ImgJudgeSpecialLine(LeftIntLine, LL[LeftIntLine], RightIntLine, RL[RightIntLine], 0))
		{
			Img_BrokenFlag = 1;
			Img_SpecialElemFlag = 1;
		}
	}
#endif
}


//================================================================//
//  @brief  :		识别坡道/路障/直道断路
//  @param  :		void 
//  @return :		void
//  @note   :		void
//================================================================//
void ImgJudgeObstacle(void)
{
#if INF
	if (g_inf > block_inf && AngleMutationFlag)
	{
		if (LeftPnt.ErrRow - RightPnt.ErrRow <= 2 && RightPnt.ErrRow - LeftPnt.ErrRow <= 2 
			&& RightPnt.ErrCol -  LeftPnt.ErrCol > 20)
		{
			int Front = MIN(LeftPnt.ErrRow, RightPnt.ErrRow);
			int FrontRompGray = RegionAveGray(Front - 10, LeftPnt.ErrCol + 5, RightPnt.ErrCol - 5);
			int FrontBlockGray = RegionAveGray(Front - 8, LeftPnt.ErrCol, RightPnt.ErrCol);
			int FrontBlockGray2 = RegionAveGray(Front - 2, LeftPnt.ErrCol, RightPnt.ErrCol);
			int DownGray = RegionAveGray(DOWN_EAGE - 2, LL[DOWN_EAGE - 2], RL[DOWN_EAGE - 2]);
			string.Format("\r\n RompGray = %d \r\n", FrontRompGray); PrintDebug(string);
			string.Format("\r\n BlockGray = %d \r\n", FrontBlockGray); PrintDebug(string);
			string.Format("\r\n DownGray = %d \r\n", DownGray); PrintDebug(string);
			if (DownGray - FrontBlockGray > DarkThreshold 
				&& FrontBlockGray2 - FrontBlockGray < 15 && FrontBlockGray - FrontBlockGray2 < 15)
			{
				Img_BlockFlag = 1;//路障
				Img_SpecialElemFlag = 1;
			}
			else if (g_inf > ramp_inf && DownGray - FrontRompGray < DownGray / 10 
				&& FrontRompGray - DownGray < DownGray / 10)
			{
				Img_RampFlag = 1;//坡道
				Img_SpecialElemFlag = 1;
			}
		}
	}
	else
#endif 	
		ImgJudgeStraightBroken();//直道断路
}

//===========================以上为可直接调用的元素识别函数======================//

//================================================================//
//  @brief  :		特殊元素
//  @param  :		坡道 路障 断路 停车线
//  @return :		void
//  @note   :		坡道：补图开 循环清其他标志
//					路障：补图关 补图开启后清其他标志一次
//					断路：补图开 断路补图模式
//					停车线：补图开 循环清其他标志
//================================================================//
void SpecialElemFill(void)
{
	VarInit();
	LeftPnt.Type = RightPnt.Type = 0;
	SelectFirstLine();
	FindLineNormal(0);
	if (1 == Img_BrokenFlag)
	{
		if (ImgJudgeOutBroken())
		{
			Img_BrokenFlag = 2;			//断路
		}
		LeftPnt.ErrRow = MAX(LeftPnt.ErrRow, LeftIntLine);
		RightPnt.ErrRow = MAX(RightPnt.ErrRow, RightIntLine);
		FillMiddleLine();
	}
	else if (2 == Img_BrokenFlag)
	{
		if (ImgJudgeOutBroken())
		{
			Img_BrokenFlag = 0;
			Img_SpecialElemFlag = 0;
		}
	}
	else if (3 == Img_BrokenFlag)
	{
		if (ImgJudgeOutBroken())
		{
			Img_BrokenFlag = 2;			//断路
		}
	}
	else if (4 == Img_BrokenFlag)//左弯断路
	{
		LeftPnt.ErrRow = DOWN_EAGE;
		LeftPnt.ErrCol = LEFT_EAGE;
		LeftPnt.Type = 2;
		RightPnt.Type = 1;
		for (int i = DOWN_EAGE; i > UP_EAGE; --i)
		{
			LL[i] = LEFT_EAGE;
		}
		FillMiddleLine();
		if (ImgJudgeOutBroken())
		{
			Img_BrokenFlag = 2;
		}
	}
	else if (5 == Img_BrokenFlag)//右弯断路
	{
		RightPnt.ErrRow = DOWN_EAGE;
		RightPnt.ErrCol = RIGHT_EAGE;
		RightPnt.Type = 2;
		LeftPnt.Type = 1;
		for (int i = DOWN_EAGE; i > UP_EAGE; --i)
		{
			RL[i] = RIGHT_EAGE;
		}
		FillMiddleLine();
		if (ImgJudgeOutBroken())
		{
			Img_BrokenFlag = 2;
		}
	}
	else if (Img_BlockFlag)
	{
		LeftPnt.ErrRow = MAX(LeftPnt.ErrRow, LeftIntLine);
		RightPnt.ErrRow = MAX(RightPnt.ErrRow, RightIntLine);
		FillMiddleLine();
	}
	else if (Img_RampFlag)
	{
		FillMiddleLine();
	}
	else if (Img_StopLineFlag)
	{
		FillMiddleLine();
	}
	else
	{
		Img_SpecialElemFlag = 0;
	}
}


//================================================================//
//  @brief  :		内跳线判断封闭曲线
//  @param  :		左线行数 右线行数 type 0为直道断路 1位弯道断路
//  @return :		1 确认有
//  @note   :		全局变量LL RL
//================================================================//
int ImgJudgeSpecialLine(int left_row, int left_col, int right_row, int right_col, int type)
{
	const int StartLine = 35;
	if (!type &&
		(left_row < StartLine && right_row < StartLine
			|| left_row - right_row > 8 || right_row - left_row > 8
			|| left_col > MIDDLE || right_col < MIDDLE))
		return 0;
	if (type)
	{
		if (left_col > right_col)
		{
			int tmp = left_col;
			left_col = right_col; right_col = tmp;
			tmp = left_row;
			left_row = right_row; right_row = tmp;
		}
	}
	const int diff = 2;
	int NewRow = left_row;
	int NewRow2 = right_row;
	int OldRow = NewRow;
	int OldRow2 = NewRow2;
	int Middle = (left_col + right_col) >> 1;
	for (int i = left_col + 1; i <= Middle; i++)
	{
		NewRow = SearchUpEage(OldRow + 2, i);
		if (NewRow - OldRow > diff || OldRow - NewRow > diff)
			return 0;
		OldRow = NewRow;
	}
	for (int i = right_col - 1; i >= Middle; i--)
	{
		NewRow2 = SearchUpEage(OldRow2 + 2, i);
		if (NewRow2 - OldRow2 > diff || OldRow2 - NewRow2 > diff)
			return 0;
		OldRow2 = NewRow2;
	}
	if (NewRow == NewRow2)return 1;
	else return 0;
}


//================================================================//
//  @brief  :		判断出断路
//  @param  :		void
//  @return :		1还处于断路  0已经出了断路
//  @note   :		void
//================================================================//
int ImgJudgeOutBroken(void)
{
	if (2 == Img_BrokenFlag)
	{
		if (LightThreshold > 50
			&& RL[DOWN_EAGE] - LL[DOWN_EAGE] > 94
			&& RL[DOWN_EAGE - 1] - LL[DOWN_EAGE - 1] > 94
			&& RL[DOWN_EAGE - 2] - LL[DOWN_EAGE - 2] > 94
			&& RL[DOWN_EAGE] >= RL[DOWN_EAGE - 1] && RL[DOWN_EAGE - 1] >= RL[DOWN_EAGE - 2]
			&& LL[DOWN_EAGE] <= LL[DOWN_EAGE - 1] && LL[DOWN_EAGE - 1] <= LL[DOWN_EAGE - 2])
			return 1;
		else return 0;
	}
	else
	{
		static SeqQueue qLight;
		static unsigned char init_flag = 0;
		if (!init_flag)
		{
			init_flag = 1;
			initQueue(&qLight);
		}
		qUpdateQueue(&qLight, LightThreshold);
		int max = qGetMax(&qLight);
		int min = qGetMin(&qLight);
		if (max - min > 35)
		{
			initQueue(&qLight);
			return 1;
		}
		else return 0;
	}
	//static int Num_i = 0;
	//static int BrokenAve[10] = { 0 };
	//if (2 == Img_BrokenFlag)
	//{
	//	if (Num_i < 5)
	//	{
	//		BrokenAve[Num_i++] = LightThreshold;
	//		if (Num_i > 1)
	//		{
	//			for (int i = 0; i < Num_i - 1; i++)
	//			{
	//				for (int j = i; j < Num_i; j++)
	//				{
	//					if (BrokenAve[j] - BrokenAve[i] > 30)
	//					{
	//						Num_i = 0;
	//						return 2;
	//					}
	//				}
	//			}
	//		}

	//		return 0;
	//	}
	//	else
	//	{
	//		for (int i = 0; i < 4; i++)		//更新数组元素
	//			BrokenAve[i] = BrokenAve[i + 1];
	//		BrokenAve[4] = LightThreshold;
	//		//判断条件
	//		for (int i = 0; i < 4; i++)
	//		{
	//			for (int j = i; j < 5; j++)
	//			{
	//				if (BrokenAve[j] - BrokenAve[i] > 30)
	//				{
	//					Num_i = 0;
	//					return 2;
	//				}
	//			}
	//		}
	//		return 0;
	//	}
	//	////FindLineNormal(0);
	//	//if (LeftPnt.ErrRow < DOWN_EAGE - 20 && RightPnt.ErrRow < DOWN_EAGE - 20)
	//	//{
	//	//	if (RL[DOWN_EAGE] - LL[DOWN_EAGE] > 94 && RL[DOWN_EAGE - 1] - LL[DOWN_EAGE - 1] > 94
	//	//		&& RL[DOWN_EAGE - 2] - LL[DOWN_EAGE - 2] > 94 && RL[DOWN_EAGE - 3] - LL[DOWN_EAGE - 3] > 94)
	//	//	{
	//	//		BrokenLastAve = 0;
	//	//		return 0;
	//	//	}
	//	//	else return 2;
	//	//}
	//	//else
	//	//{
	//	//	return 2;
	//	//}
	//}
	//else
	//{
	//	if (Num_i < 10)
	//	{
	//		BrokenAve[Num_i++] = LightThreshold;
	//		if (Num_i > 1)
	//		{
	//			for (int i = 0; i < Num_i - 1; i++)
	//			{
	//				for (int j = i; j < Num_i; j++)
	//				{
	//					if (BrokenAve[i] - BrokenAve[j] > 30)
	//					{
	//						Num_i = 0;
	//						return 1;
	//					}
	//				}
	//			}
	//		}
	//		/*if (Num_i > 0 && BrokenAve[Num_i - 1] - LightThreshold > 30)
	//		{
	//			Num_i = 0;
	//			return 1;
	//		}
	//		BrokenAve[Num_i++] = LightThreshold;
	//		return 0;*/
	//	}
	//	else
	//	{
	//		for (int i = 0; i < 9; i++)		//更新数组元素
	//			BrokenAve[i] = BrokenAve[i + 1];
	//		BrokenAve[9] = LightThreshold;
	//		//判断条件
	//		for (int i = 0; i < 9; i++)
	//		{
	//			for (int j = i; j < 10; j++)
	//			{
	//				if (BrokenAve[i] - BrokenAve[j] > 30)
	//				{
	//					Num_i = 0;
	//					return 1;
	//				}
	//			}
	//		}
	//		return 0;
	//	}

	//	//		if (BrokenLastAve == 0)
	//	//		{
	//	//			BrokenLastAve = LightThreshold;
	//	//			return 0;
	//	//		}
	//	//		else
	//	//		{
	//	//			if (BrokenLastAve - LightThreshold > 30)
	//	//			{
	//	//                          BrokenLastAve = LightThreshold;
	//	//				return 1;
	//	//			}
	//	//			else 
	//	//                        {
	//	//                          BrokenLastAve = LightThreshold;
	//	//                          return 0;
	//	//                        }
	//	//		}
	//}
	//return 0;
}

//================================================================//
//  @brief  :		识别停车线
//  @param  :		void
//  @return :		1停车线  0非停车线
//  @note   :		void
//================================================================//
int ImgIsStopLine(int line, int left, int right)
{
	int count = 0;
	for (int i = left + 1; i < right; )
	{
		i = SearchRightEage(line, i);
		i = SearchRightNoEage(line, i);
		count++;
		if (count > 6) return 1;
	}
	return 0;
}

//================================================================//
//  @brief  :		识别停车线以及距离
//  @param  :		void
//  @return :		1停车线  0非停车线
//  @note   :		void
//================================================================//
int DistStopLine(int line)
{
	int a = ImgIsStopLine(line, LL[line], RL[line]);
	line += 2;
	int b = ImgIsStopLine(line, LL[line], RL[line]);
	line += 2;
	int c = ImgIsStopLine(line, LL[line], RL[line]);
	if (a || b || c)return 1;
	else return 0;
}


//================================================================//
//  @brief  :		期望速度控制
//  @param  :		middle 车辆中值
//  @return :		void
//  @note   :		void
//================================================================//
int GetSpeedRow(int middle, int left_row, int right_row)
{
	Point P[3] = {
		{DOWN_EAGE, middle - 5},
	{DOWN_EAGE, middle},
	{DOWN_EAGE, middle + 5} };
	int min_row = MIN(left_row, right_row);
	for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	{
		if ((LL[i] - P[0].Col) * (LL[i - 1] - P[0].Col) <= 0
			|| (RL[i] - P[0].Col) * (RL[i - 1] - P[0].Col) <= 0
			|| UP_EAGE + 1 == i)
		{
			P[0].Row = MAX(i, min_row);
			break;
		}
	}
	for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	{
		if ((LL[i] - P[1].Col) * (LL[i - 1] - P[1].Col) <= 0
			|| (RL[i] - P[1].Col) * (RL[i - 1] - P[1].Col) <= 0
			|| UP_EAGE + 1 == i)
		{
			P[1].Row = MAX(i, min_row);
			break;
		}
	}
	for (int i = DOWN_EAGE; i > UP_EAGE; i--)
	{
		if ((LL[i] - P[2].Col) * (LL[i - 1] - P[2].Col) <= 0
			|| (RL[i] - P[2].Col) * (RL[i - 1] - P[2].Col) <= 0
			|| UP_EAGE + 1 == i)
		{
			P[2].Row = MAX(i, min_row);
			break;
		}
	}
	return (P[1].Row + P[0].Row + P[2].Row) / 3;
}

//================================================================//
//  @brief  :		期望速度计算
//  @param  :		row 赛道边缘行数
//  @return :		void
//  @note   :		void
//================================================================//
int GetSpeedCount(int row, int speed_max, int speed_min, float a)
{
	if (50 < row) return speed_min;
	int temp = (int)(a * (50 - row) * (50 - row) + speed_min + 0.5f);
	return MIN(speed_max, temp);
}
