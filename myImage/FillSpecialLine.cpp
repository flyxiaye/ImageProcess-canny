#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"
#include "Judge.h"



//================================================================//
//  @brief  :		补斜十字
//  @param  :		void
//  @return :		-1错误 1补线成功
//  @note   :		void
//================================================================//
int FillBevelCross(void)
{
	if (3 == RightPnt.ErrType || 2 == RightPnt.ErrType && 5 == LeftPnt.ErrType)			//右拐点 左斜十字
	{
		int errorflag = 0;
		//寻线
		FindLine = FindCross_2(FIND_LEFT);
		str.Format("\r\n FindLine = %d \r\n", FindLine); PrintDebug(str);
		if (FindLine)
		{
			if (Connect((RightPnt.ErrRow + LeftPnt.ErrRow) >> 1, (RightPnt.ErrCol + LeftPnt.ErrCol) >> 1, FindLine, RL[FindLine]))	//连通检测是否正确
			{
				//补线
				int row1 = CutLine(&RL[FindLine], FindLine);
				int row2 = CutLineDown(&RL[RightPnt.ErrRow], RightPnt.ErrRow);
				if (-1 == FillLinePnt(FIND_RIGHT, row2, row1))
					errorflag = 1;
				else                                              //补左线
				{
					RightPnt.ErrType = 0;
					RightPnt.ErrRow = row1;
					RightPnt.ErrCol = RL[row1];
					int j;
					for (j = RL[row1]; j >= LEFT_EAGE; --j)
					{
						int a = IsEage(FIND_LEFT, row1, j, 4, 2);
						if (1 == a)
						{
							LL[row1] = j;
							if (2 == LeftPnt.ErrType)
							{
								if (j - LeftPnt.ErrCol < 80 && LeftPnt.ErrCol - j < 80)
								{
									int row3 = CutLineDown(&LL[LeftPnt.ErrRow], LeftPnt.ErrRow);
									if (-1 == FillLinePnt(FIND_LEFT, row3, row1))
									{
										errorflag += 2;
										break;
									}
									else
									{
										LeftPnt.ErrType = 0;
										LeftPnt.ErrRow = row1;
										LeftPnt.ErrCol = j;
										break;
									}
								}
								else
								{
									int k;
									for (k = 0; k < 10; ++k)
									{
										if (1 != FindL(FIND_LEFT, row1 + k + 1, row1 + k, AutoValue(row1 + k + 1)))
											break;
									}
									if (10 == k)
									{
										FillLineDown(FIND_LEFT, row1);
										LeftPnt.ErrType = 0;
										LeftPnt.ErrRow = row1;
										LeftPnt.ErrCol = j;
										break;
									}
									else
									{
										//errorflag = 3;//new
										break;
									}
								}

							}
							else if (5 == LeftPnt.ErrType)
							{
								int k;
								for (k = 0; k < 10; ++k)
								{
									if (1 != FindL(FIND_LEFT, row1 + k + 1, row1 + k, AutoValue(row1 + k + 1)))
										break;
								}
								if (10 == k)
								{
									LeftPnt.ErrType = 0;
									LeftPnt.ErrRow = row1;
									LeftPnt.ErrCol = j;
									FillLineDown(FIND_LEFT, row1);
									break;
								}
								else
									break;
							}
						}
						else if (2 == a)
						{
							if (2 == LeftPnt.ErrType)
							{
								j = LEFT_EAGE - 1;
								break;
							}
							else if (5 == LeftPnt.ErrType)
								break;
						}
					}
					if (LEFT_EAGE - 1 == j)
					{
						errorflag += 2;
					}
				}
			}
			else
			{
				errorflag = 1;
			}
		}
		else errorflag = 1;
		if (errorflag)
		{
			int a = 0, b = 0;
			if (2 == LeftPnt.ErrType)
				a = FillLineUp(FIND_LEFT, LeftPnt.ErrRow);
			if (1 == errorflag)
				b = FillLineUp(FIND_RIGHT, RightPnt.ErrRow);
			if (-1 == a || -1 == b)
				return -1;
			else
				return 1;
		}
		else return 1;
	}
	else if (3 == LeftPnt.ErrType || 2 == LeftPnt.ErrType && 6 == RightPnt.ErrType)				//左拐点或左跳变点 右斜十字
	{
		int errorflag = 0; //错误1 两边补线 错误2 右边补线
		//寻线
		FindLine = FindCross_2(FIND_RIGHT);
		if (FindLine)
		{
			str.Format("\r\n you = %d \r\n", 1); PrintDebug(str);
			str.Format("\r\n FindLine = %d \r\n", FindLine); PrintDebug(str);
			if (Connect((RightPnt.ErrRow + LeftPnt.ErrRow) >> 1, (RightPnt.ErrCol + LeftPnt.ErrCol) >> 1, FindLine, LL[FindLine]))	//连通检测是否正确
			{
				//补线
				int row1 = CutLine(&LL[FindLine], FindLine);
				int row2 = CutLineDown(&LL[LeftPnt.ErrRow], LeftPnt.ErrRow);
				if (-1 == FillLinePnt(FIND_LEFT, row2, row1))
					errorflag = 1;
				else
				{
					LeftPnt.ErrType = 0;
					LeftPnt.ErrRow = row1;
					LeftPnt.ErrCol = LL[row1];
					int j;
					for (j = LL[row1]; j <= RIGHT_EAGE; ++j)
					{
						int a = IsEage(FIND_RIGHT, row1, j, 4, 2);
						if (1 == a)
						{
							RL[row1] = j;
							if (2 == RightPnt.ErrType)
							{
								if (j - RightPnt.ErrCol < 80 && RightPnt.ErrCol - j < 80)
								{
									int row3 = CutLineDown(&RL[RightPnt.ErrRow], RightPnt.ErrRow);
									if (-1 == FillLinePnt(FIND_RIGHT, row3, FindLine))
									{
										errorflag += 2;
										break;
									}
									else
									{
										RightPnt.ErrType = 0;
										RightPnt.ErrRow = row1;
										RightPnt.ErrCol = j;
										break;
									}
								}
								else
								{
									int k;
									for (k = 0; k < 10; ++k)
									{
										if (1 != FindL(FIND_RIGHT, row1 + k + 1, row1 + k, AutoValue(row1 + k + 1)))
											break;
									}
									if (10 == k)
									{
										RightPnt.ErrType = 0;
										RightPnt.ErrRow = row1;
										RightPnt.ErrCol = j;
										FillLineDown(FIND_RIGHT, row1);
										break;
									}
									else break;
								}
							}
							else if (6 == RightPnt.ErrType)
							{
								int k;
								for (k = 0; k < 10; ++k)
								{
									if (1 != FindL(FIND_RIGHT, row1 + k + 1, row1 + k, AutoValue(row1 + k + 1)))
										break;
								}
								if (10 == k)
								{
									RightPnt.ErrType = 0;
									RightPnt.ErrRow = row1;
									RightPnt.ErrCol = j;
									FillLineDown(FIND_RIGHT, row1);
									break;
								}
								else break;
							}
						}
						else if (2 == a)
						{
							if (2 == RightPnt.ErrType)
							{
								j = RIGHT_EAGE + 1;
								break;
							}
							else if (6 == RightPnt.ErrType)
								break;
						}
					}
					if (RIGHT_EAGE + 1 == j)
					{
						errorflag += 2;
					}
				}
			}
			else errorflag = 1;
		}
		else errorflag = 1;
		if (errorflag)
		{
			//判断右边类型
			int a = 0, b = 0;
			if (2 == RightPnt.ErrType)
				a = FillLineUp(FIND_RIGHT, RightPnt.ErrRow);
			if (1 == errorflag)
				b = FillLineUp(FIND_LEFT, LeftPnt.ErrRow);
			if (-1 == a || -1 == b)
				return -1;
			else return 1;
		}
		else return 1;
	}
	else return -1;
	return 1;
}

//================================================================//
//  @brief  :		补平十字
//  @param  :		void
//  @return :		-1错误 0补图失败
//  @note   :		void
//================================================================//
int FillLevelCross(void)
{
	int StartLine = FindCrossLine_1(MIN(LeftPnt.ErrRow, RightPnt.ErrRow), LeftPnt.ErrCol, RightPnt.ErrCol, 0);
	//int StartLine = FindCrossLine_1(FindLine, LeftPnt.ErrCol, RightPnt.ErrCol, 0);
	str.Format("\r\n StartLine = %d \r\n", StartLine); PrintDebug(str);
	FindLine = FindCross_1(StartLine, (MAX(LeftPnt.ErrCol - 20, LEFT_EAGE)), MIN(RightPnt.ErrCol + 20, RIGHT_EAGE));
	if (-1 == FindLine || 0 == FindLine)
		return FindLine;
	else
	{
		int row1 = CutLine(&LL[FindLine], FindLine);
		int row2 = CutLineDown(&LL[LeftPnt.ErrRow], LeftPnt.ErrRow);
		int FlagLeft = FillLinePnt(FIND_LEFT, row2, row1);
		int row3 = CutLine(&RL[FindLine], FindLine);
		int row4 = CutLineDown(&RL[RightPnt.ErrRow], RightPnt.ErrRow);
		int FlagRight = FillLinePnt(FIND_RIGHT, row4, row3);
		if (1 == FlagLeft && 1 == FlagRight)
		{
			LeftPnt.ErrType = 0;
			LeftPnt.ErrRow = FindLine;
			LeftPnt.ErrCol = LL[FindLine];
			RightPnt.ErrType = 0;
			RightPnt.ErrRow = FindLine;
			RightPnt.ErrCol = RL[FindLine];
			return FindLine;
		}
		else return 0;
	}
}

//================================================================//
//  @brief  :		补平十字（4类）
//  @param  :		void
//  @return :		-1错误
//  @note   :		void
//================================================================//
int FillLevelCross_4(void)
{
	int LeftEage = ArrayMax(LL, 10), RightEage = ArrayMin(RL, 10);
	int FinalLine = FindCrossLine_1(0, LeftEage, RightEage, 40);
	str.Format("\r\n FinalLine = %d \r\n", FinalLine); PrintDebug(str);
	FindLine = FindCross_1(FinalLine, LeftEage, RightEage);
	show(FindLine);
	str.Format("\r\n 4_FindLine = %d \r\n", FindLine); PrintDebug(str);
	if (FindLine)
	{
		//向下补线
		FillLineDown(FIND_LEFT, FindLine);
		FillLineDown(FIND_RIGHT, FindLine);
		LeftPnt.ErrType = 0;
		LeftPnt.ErrRow = FindLine;
		LeftPnt.ErrCol = LL[FindLine];
		RightPnt.ErrType = 0;
		RightPnt.ErrRow = FindLine;
		RightPnt.ErrCol = RL[FindLine];
		return 1;
	}
	else
	{
		return -1;
	}
}

//================================================================//
//  @brief  :		补中线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillMiddleLine(void)
{
	if (LeftPnt.ErrRow > RightPnt.ErrRow)
	{
		int i;
		for (i = 0; i <= RightPnt.ErrRow; ++i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
		int Offset = (RL[i - 1] - LL[i - 1]) >> 1;
		for (; i <= LeftPnt.ErrRow; ++i)
		{
			ML[i] = LL[i] + Offset;
		}
	}
	else if (LeftPnt.ErrRow < RightPnt.ErrRow)
	{
		int i;
		for (i = 0; i <= LeftPnt.ErrRow; ++i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
		int Offset = (RL[i - 1] - LL[i - 1]) >> 1;
		for (; i <= RightPnt.ErrRow; ++i)
		{
			ML[i] = RL[i] - Offset;
		}
	}
	else
	{
		for (int i = 0; i < LeftPnt.ErrRow; ++i)
			ML[i] = (LL[i] + RL[i]) >> 1;
	}
	LeftPnt.ErrType = 9;
	RightPnt.ErrType = 9;
}

//================================================================//
//  @brief  :		向上补线
//  @param  :		类型（type) 起始行（line)
//  @return :		-1错误
//  @note   :		void
//================================================================//
int FillLineUp(int type, int line)
{
	if (0 > line || IMG_ROWS <= line)
		return -1;
	else if (0 == line)
		return 1;

	int *LinePoint;
	if (FIND_LEFT == type)
		LinePoint = LL;
	else if (FIND_RIGHT == type)
		LinePoint = RL;
	else LinePoint = ML;

	//直线修剪
	line = CutLineDown(LinePoint + line, line);

	Line TmpLine = LeastSquare(LinePoint, MAX(line - 10, 0), line);
	if (0 == TmpLine.k && 0 == TmpLine.b)
		return -1;

	int *p = LinePoint + line, i;
	for (i = line; i < IMG_ROWS; ++i, ++p)
	{
		*p = (int)(TmpLine.k * i + TmpLine.b + 0.5);
		if (*p < LEFT_EAGE || *p > RIGHT_EAGE)
			break;
	}

	if (type == FIND_LEFT)
	{
		LeftPnt.ErrType = 5;
		LeftPnt.ErrRow = i - 1;
		LeftPnt.ErrCol = *(--p);

		if (IMG_ROWS == i)
		{
			LeftPnt.ErrType = 4;
		}
		else if (RIGHT_EAGE - MaxValue <= *p)
			LeftPnt.ErrType = 6;
	}
	else if (type == FIND_RIGHT)
	{
		RightPnt.ErrType = 6;
		RightPnt.ErrRow = i - 1;
		RightPnt.ErrCol = *(--p);
		if (IMG_ROWS == i)
		{
			RightPnt.ErrType = 4;
		}
		else if (LEFT_EAGE + MaxValue >= *p)
			RightPnt.ErrType = 5;
	}
	else;
	return 1;
}

//================================================================//
//  @brief  :		向下补线
//  @param  :		类型（type) 起始行（line)
//  @return :		-1错误
//  @note   :		void
//================================================================//
int FillLineDown(int type, int line)
{
	if (line <= 0 || line >= IMG_ROWS)
		return -1;
	int TmpLine;
	if (FIND_LEFT == type)
	{
		TmpLine = CutLine(&LL[line], line);
		Line LftLine = LeastSquare(LL, TmpLine, MIN(line + 10, IMG_ROWS - 1));
		if (0 == LftLine.b && 0 == LftLine.k)
			return -1;
		for (int i = 0; i < TmpLine; ++i)
		{
			*(LL + i) = (int)(i * LftLine.k + LftLine.b + 0.5);
		}
	}
	else if (FIND_RIGHT == type)
	{
		TmpLine = CutLine(&RL[line], line);
		Line RhtLine = LeastSquare(RL, TmpLine, MIN(line + 10, IMG_ROWS - 1));
		if (0 == RhtLine.k && 0 == RhtLine.b)
			return -1;
		for (int i = 0; i < TmpLine; ++i)
		{
			*(RL + i) = (int)(i * RhtLine.k + RhtLine.b + 0.5);
		}
	}
	return 1;
}

//================================================================//
//  @brief  :		两点补线
//  @param  :		类型（type) 起始行（line) 结束行（end_line）
//  @return :		-1错误
//  @note   :		void
//================================================================//
int FillLinePnt(int type, int line, int end_line)
{
	if (line >= end_line)
		return -1;
	//line 和 endline均有数据
	int *LinePoint;
	if (FIND_LEFT == type)
		LinePoint = LL;
	else if (FIND_RIGHT == type)
		LinePoint = RL;
	else LinePoint = ML;

	float k = (float)(*(LinePoint + end_line) - *(LinePoint + line)) / (float)(end_line - line);

	int *p = LinePoint + line;
	for (int i = line + 1; i <= end_line; ++i)
	{
		*(++p) = (int)(k * (i - line) + *(LinePoint + line) + 0.5);
	}
	return 1;
}

//================================================================//
//  @brief  :		向下补线（斜率）
//  @param  :		类型（type) 起始行（line) 结束行（end_line)
//  @return :		-1错误
//  @note   :		void
//================================================================//
int FillLineDownK(int type, int line, int end_line)
{
	if (line == end_line)
		return -1;
	int *LinePoint;
	if (FIND_LEFT == type)
		LinePoint = LL;
	else if (FIND_RIGHT == type)
		LinePoint = RL;
	else LinePoint = ML;

	float k = (float)(*(LinePoint + end_line) - *(LinePoint + line)) / (float)(end_line - line);

	for (int i = 0; i < line; ++i)
	{
		*(LinePoint + i) = (int)((k * (i - line) + *(LinePoint + line)) + 0.5);
	}
	return 1;
}

//================================================================//
//  @brief  :		刹车补线
//  @param  :		类型（type) 起始行（line) 结束行（end_line)
//  @return :		-1错误
//  @note   :		void
//================================================================//
void StopFill(void)
{
	for (int i = 0; i < IMG_ROWS; ++i)
	{
		int j, LeftFindFlag = 0, RightFindFlag = 0;
		for (j = LEFT_EAGE; j <= RIGHT_EAGE; ++j)
		{
			if (1 == IsEage(FIND_LEFT, i, j, 2, 13))
			{
				LL[i] = j;
				LeftPnt.ErrRow = i;
				LeftPnt.ErrCol = j;
				LeftFindFlag = 1;
				break;
			}
		}
		for (; j <= RIGHT_EAGE; ++j)
			if (1 == IsEage(FIND_RIGHT, i, j, 2, 13))
			{
				RL[i] = j;
				RightPnt.ErrRow = i;
				RightPnt.ErrCol = j;
				RightFindFlag = 1;
				break;
			}
		if (LeftFindFlag && RightFindFlag)
		{
			LeftPnt.ErrType = 0;
			RightPnt.ErrType = 0;
			//向下寻线
			for (int k = i - 1; k >= 0; --k)
			{
				FindL(FIND_LEFT, k, k + 1, AutoValue(k));
				FindL(FIND_RIGHT, k, k + 1, AutoValue(k));
			}
			return;
		}
		else
		{
			continue;
		}
	}
}