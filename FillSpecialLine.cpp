#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"



#define UP45_TH 3
//================================================================//
//  @brief  :		������˫��ʮ��
//  @param  :		LeftPnt RightPnt
//  @return :		LeftPnt RightPnt
//  @note   :		���߽��ܿ�ʼ��������
//================================================================//
void FillFourCross(void)
{
	//����B�� //�����ĺ��Լ��ҵ�
	int StartCol;
	if (LeftPnt.ErrCol < 30)
		StartCol = 30;
	else StartCol = LeftPnt.ErrCol;
	Point PointNew = FindLowPoint(LeftPnt.ErrRow, StartCol + 1, StartCol + 31, 15);
	Point PointOld = PointNew;
	while (1)				//��ʼб45����
	{
		PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
		if (UP_EAGE >= PointNew.Row || RIGHT_EAGE <= PointNew.Col
			|| PointOld.Row - PointNew.Row > UP45_TH)		//�ҵ�Ϊ�߽�� �����ҵ����
		{
			if (PointOld.Row < 3 + UP_EAGE)
			{
				ErrorFlag = 1;
				return;
			}
			//������5��
			LL[PointOld.Row] = PointOld.Col;
			for (int i = PointOld.Row - 1; i >= PointOld.Row - 5; --i)
			{
				LL[i] = GetLL(i, LL[i + 1]);
				if (LL[i] - LL[i + 1] > FINDLINE_TH || LL[i + 1] - LL[i] > FINDLINE_TH)
					ErrorFlag = 5;
			}
			//��������			
			float k = LeastSquare(LL, PointOld.Row, PointOld.Row - 5);
			string.Format("\r\n k = %f \r\n", k); PrintDebug(string);
			if (k > 0)
			{
				ErrorFlag = 5;
				break;
			}
			else
			{
				FillLineDown(LL, PointOld.Row, PointOld.Row - 5);
				LeftPnt.ErrRow = PointOld.Row - 3;
				LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
				LeftPnt.Type = 0;
				break;
			}
		}
		else if (DOWN_EAGE <= PointNew.Row)
		{
			LeftPnt.Type = 2;
			break;
		}
		else
		{
			PointOld = PointNew;
		}
	}
	//����B��
	if (RightPnt.ErrCol > RIGHT_EAGE - 30)
		StartCol = RIGHT_EAGE - 30;
	else StartCol = RightPnt.ErrCol;
	PointNew = FindLowPoint(RightPnt.ErrRow, StartCol - 31, StartCol - 1, 15);
	PointOld = PointNew;
	while (1)				//��ʼб45����
	{
		PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
		if (UP_EAGE >= PointNew.Row || LEFT_EAGE >= PointNew.Col
			|| PointOld.Row - PointNew.Row > UP45_TH)		//�ҵ�Ϊ�߽�� �����ҵ��ҵ�
		{
			if (PointOld.Row < 3 + UP_EAGE)
			{
				ErrorFlag = 1;
				return;
			}
			//������5��
			RL[PointOld.Row] = PointOld.Col;
			for (int i = PointOld.Row - 1; i >= PointOld.Row - 5; --i)
			{
				RL[i] = GetRL(i, RL[i + 1]);
				if (RL[i] - RL[i + 1] > FINDLINE_TH || RL[i + 1] - RL[i] > FINDLINE_TH)
					ErrorFlag = 5;
			}
			//��������
			float k = LeastSquare(RL, PointOld.Row, PointOld.Row - 5);
			string.Format("\r\n k = %f \r\n", k); PrintDebug(string);
			if (k < 0)
			{
				ErrorFlag = 5;
				break;
			}
			else
			{
				FillLineDown(RL, PointOld.Row, PointOld.Row - 5);
				RightPnt.ErrRow = PointOld.Row - 3;
				RightPnt.ErrCol = RL[RightPnt.ErrRow];
				RightPnt.Type = 0;
				break;
			}
		}
		else if (DOWN_EAGE <= PointNew.Row)
		{
			RightPnt.Type = 2;
			break;
		}
		else
		{
			PointOld = PointNew;
		}
	}
}

//================================================================//
//  @brief  :		��бʮ��
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillBevelCross(void)
{
	if (LeftPnt.ErrRow < RightPnt.ErrRow)		//��յ� ������ ��бʮ��
	{
		//�յ������ҵ�
		Point PointNew;
		PointNew.Row = SearchUpEage(LeftPnt.ErrRow - 1, LeftPnt.ErrCol + 1) + 1;
		PointNew.Col = LeftPnt.ErrCol + 1;
		Point PointOld = PointNew;
		if (UP_EAGE == PointNew.Row)			//�ҵ�Ϊ�ϱ߽�
			LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 4, LeftPnt.ErrRow + 2);
		else
		{
			while (1)				//��ʼб45����
			{
				PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
				if (PointOld.Row - PointNew.Row > UP45_TH)	//�ҵ���B�� ����PointOld��
				{
					PointNew.Row = LeftPnt.ErrRow;		//����LPERֵ��Ϊ�������ж�
					LL[PointOld.Row] = PointOld.Col;
					FillLinePoint(LL, LeftPnt.ErrRow + 2, PointOld.Row);
					LeftPnt.ErrRow = PointOld.Row;
					LeftPnt.ErrCol = PointOld.Col;
					LeftPnt.Type = 0;
					break;
				}
				else if (UP_EAGE >= PointNew.Row || RIGHT_EAGE <= PointNew.Col || UP_EAGE + UP45_TH > PointOld.Row)		//�ҵ�Ϊ�߽�� ���ϲ���
				{
					LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 5, LeftPnt.ErrRow + 2);
					break;
				}
				else if (DOWN_EAGE <= PointNew.Row)
				{
					LeftPnt.Type = 2;
					break;
				}
				else
				{
					PointOld = PointNew;
				}
			}
		}
		if (1 != g_RoadType)
		{
			//����������ҵ�
			PointNew.Row = SearchUpEage(RightPnt.ErrRow - 1, RightPnt.ErrCol - 1) + 1;
			PointNew.Col = RightPnt.ErrCol - 1;
			PointOld = PointNew;
			if (RightPnt.ErrCol > RL[DOWN_EAGE])		//��������
			{
				RightPnt = FillLineUp(RL, MIN(RightPnt.ErrRow + 10, DOWN_EAGE), RightPnt.ErrRow + 2);
			}
			else if (UP_EAGE == PointNew.Row || PointNew.Row < LeftPnt.ErrRow)		//���������ϱ߽� ���ҵ������߽���� ��Ϊ�����
				RightPnt = FillLineUp(RL, RightPnt.ErrRow + 4, RightPnt.ErrRow + 2);
			else
			{

				while (1)				//��ʼб65����
				{
					PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
					if (PointOld.Row - PointNew.Row > UP45_TH)	//�ҵ���B�� ����PointOld��
					{
						RL[PointOld.Row] = PointOld.Col;
						FillLinePoint(RL, RightPnt.ErrRow + 2, PointOld.Row);
						RightPnt.ErrRow = PointOld.Row;
						RightPnt.ErrCol = PointOld.Col;
						RightPnt.Type = 0;
						break;
					}
					else if (UP_EAGE >= PointNew.Row || LEFT_EAGE >= PointNew.Col || UP_EAGE + UP45_TH > PointOld.Row)		//�ҵ�Ϊ�߽�� ���ϲ���
					{
						RightPnt = FillLineUp(RL, RightPnt.ErrRow + 5, RightPnt.ErrRow + 2);
						break;
					}
					else if (DOWN_EAGE <= PointNew.Row)
					{
						RightPnt.Type = 2;
						break;
					}
					else
					{
						PointOld = PointNew;
					}
				}
			}
		}
		else			//����бʮ��
		{
			if (UP_EAGE == PointOld.Row)		//����޲���
				;
			else
			{
				PointOld.Col = SearchRightEage(PointOld.Row, PointOld.Col + 1) - 1;
				if (RIGHT_EAGE == PointOld.Col)		//�߽�
					;
				else
				{
					while (1)
					{
						PointNew = SearchRightDownEage(PointOld.Row, PointOld.Col - 1);
						if (PointNew.Row - PointOld.Row > UP45_TH || PointNew.Col >= RIGHT_EAGE || PointNew.Row >= DOWN_EAGE)
						{
							if (PointOld.Col > RIGHT_EAGE - 10)		//���ڿ����߽�
								break;
							else
							{
								RL[PointOld.Row] = PointOld.Col;
								//������5����
								for (int i = PointOld.Row - 1; i > PointOld.Row - 5; --i)
								{
									RL[i] = GetRL(i, RL[i + 1]);
								}
								float k = LeastSquare(RL, PointOld.Row, PointOld.Row - 4);
								if (RL[PointOld.Row - 1] < MIDDLE + 20 || k < 0)
								{
									ErrorFlag = 5;
									break;
								}
								else
								{
									FillLineDown(RL, PointOld.Row, PointOld.Row - 4);
									RightPnt.Type = 0;
									RightPnt.ErrRow = PointOld.Row - 4;;
									RightPnt.ErrCol = RL[RightPnt.ErrRow];
									break;
								}
							}
						}
						else if (DOWN_EAGE <= PointNew.Row)
						{
							RightPnt.Type = 2;
							break;
						}
						else
						{
							PointOld = PointNew;
						}
					}
				}
			}
		}

	}
	else if (RightPnt.ErrRow < LeftPnt.ErrRow)		//�ҹյ� ������ ��бʮ��
	{
		//�յ������ҵ�
		Point PointNew;
		PointNew.Row = SearchUpEage(RightPnt.ErrRow - 1, RightPnt.ErrCol - 1) + 1;
		PointNew.Col = RightPnt.ErrCol - 1;
		Point PointOld = PointNew;
		if (UP_EAGE >= PointNew.Row)			//�ҵ�Ϊ�ϱ߽�
			RightPnt = FillLineUp(RL, RightPnt.ErrRow + 4, RightPnt.ErrRow + 2);
		else
		{
			while (1)				//��ʼб65����
			{
				PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
				if (PointOld.Row - PointNew.Row > UP45_TH)	//�ҵ���B�� ����PointOld��
				{
					PointNew.Row = RightPnt.ErrRow;		//����RPERֵ��Ϊ�������ж�
					RL[PointOld.Row] = PointOld.Col;
					FillLinePoint(RL, RightPnt.ErrRow + 2, PointOld.Row);
					RightPnt.ErrRow = PointOld.Row;
					RightPnt.ErrCol = PointOld.Col;
					RightPnt.Type = 0;
					break;
				}
				else if (UP_EAGE >= PointNew.Row || LEFT_EAGE >= PointNew.Col || UP_EAGE + UP45_TH > PointOld.Row)		//�ҵ�Ϊ�߽�� ���ϲ���
				{
					RightPnt = FillLineUp(RL, RightPnt.ErrRow + 5, RightPnt.ErrRow + 2);
					break;
				}
				else if (DOWN_EAGE <= PointNew.Row)
				{
					RightPnt.Type = 2;
					break;
				}
				else
				{
					PointOld = PointNew;
				}
			}
		}
		if (1 != g_RoadType)
		{
			//����������ҵ�
			PointNew.Row = SearchUpEage(LeftPnt.ErrRow - 1, LeftPnt.ErrCol + 1) + 1;
			PointNew.Col = LeftPnt.ErrCol + 1;
			PointOld = PointNew;
			if (LeftPnt.ErrCol < LL[DOWN_EAGE])			//��������
			{
				LeftPnt = FillLineUp(LL, MIN(LeftPnt.ErrRow + 10, DOWN_EAGE), LeftPnt.ErrRow + 2);
			}
			else if (UP_EAGE == PointNew.Row || PointNew.Row < RightPnt.ErrRow)		//���������ϱ߽� ���ҵ�����ұ߽���� ��Ϊ�����
				LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 4, LeftPnt.ErrRow + 2);
			else
			{
				while (1)				//��ʼб45����
				{
					PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
					if (PointOld.Row - PointNew.Row > UP45_TH)	//�ҵ���B�� ����PointOld��
					{
						LL[PointOld.Row] = PointOld.Col;
						FillLinePoint(LL, LeftPnt.ErrRow + 2, PointOld.Row);
						LeftPnt.ErrRow = PointOld.Row;
						LeftPnt.ErrCol = PointOld.Col;
						LeftPnt.Type = 0;
						break;
					}
					else if (UP_EAGE >= PointNew.Row || RIGHT_EAGE <= PointNew.Col)		//�ҵ�Ϊ�߽�� ���ϲ���
					{
						LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 5, LeftPnt.ErrRow + 2);
						break;
					}
					else if (DOWN_EAGE <= PointNew.Row)
					{
						LeftPnt.Type = 2;
						break;
					}
					else
					{
						PointOld = PointNew;
					}
				}
			}
		}
		else			//��߶���ʮ��
		{
			if (UP_EAGE == PointOld.Row)		//�ұ��޲��� 
				;
			else
			{
				PointOld.Col = SearchLeftEage(PointOld.Row, PointOld.Col - 1) + 1;
				if (LEFT_EAGE == PointOld.Col)		//�߽�
					;
				else
				{
					while (1)
					{
						PointNew = SearchLeftDownEage(PointOld.Row, PointOld.Col + 1);
						if (PointNew.Row - PointOld.Row > UP45_TH || PointNew.Col <= LEFT_EAGE || PointNew.Row >= DOWN_EAGE)
						{
							if (PointOld.Col < LEFT_EAGE + 10)		//���ڿ����߽�
								break;
							else
							{
								LL[PointOld.Row] = PointOld.Col;
								//������5����
								for (int i = PointOld.Row - 1; i > PointOld.Row - 5; --i)
								{
									LL[i] = GetLL(i, LL[i + 1]);
								}
								float k = LeastSquare(LL, PointOld.Row, PointOld.Row - 4);
								if (LL[PointOld.Row - 1] > MIDDLE - 20 || k > 0)
								{
									ErrorFlag = 5;
									break;
								}
								else
								{
									FillLineDown(LL, PointOld.Row, PointOld.Row - 4);
									LeftPnt.Type = 0;
									LeftPnt.ErrRow = PointOld.Row - 4;
									LeftPnt.ErrCol = LL[LeftPnt.ErrRow];
									break;
								}
							}
						}
						else if (DOWN_EAGE <= PointNew.Row)
						{
							LeftPnt.Type = 2;
							break;
						}
						else
						{
							PointOld = PointNew;
						}
					}
				}
			}
		}
	}
	else ErrorFlag = 2;
}

//================================================================//
//  @brief  :		��ƽʮ��
//  @param  :		LeftPnt RightPnt
//  @return :		LeftPnt RightPnt
//  @note   :		void
//================================================================//
void FillLevelCross(void)
{
	//������������ҵ�
	Point PointNew;
	PointNew.Row = SearchUpEage(LeftPnt.ErrRow - 1, LeftPnt.ErrCol + 1) + 1;
	PointNew.Col = LeftPnt.ErrCol + 1;
	Point PointOld = PointNew;
	if (UP_EAGE == PointNew.Row)			//�ҵ�Ϊ�ϱ߽�
		LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 5, LeftPnt.ErrRow + 2);
	else
	{
		while (1)				//��ʼб65����
		{
			PointNew = SearchRightUpEage(PointOld.Row + 1, PointOld.Col);
			if (PointOld.Row - PointNew.Row > UP45_TH)	//�ҵ���B�� ����TmpB��
			{
				//��B���ж���ȷ��
				for (int i = PointOld.Row - 1; i > PointOld.Row - 5; i--)
				{
					LL[i] = GetLL(i, LL[i + 1]);
					if (LL[i + 1] - LL[i] > 0 || LL[i] - LL[i + 1] > FINDLINE_TH) ErrorFlag = 6;
				}
				LL[PointOld.Row] = PointOld.Col;
				FillLinePoint(LL, LeftPnt.ErrRow + 2, PointOld.Row);
				LeftPnt.ErrRow = PointOld.Row;
				LeftPnt.ErrCol = PointOld.Col;
				LeftPnt.Type = 0;
				break;
			}
			else if (UP_EAGE >= PointNew.Row || RIGHT_EAGE <= PointNew.Col)		//�ҵ�Ϊ�߽�� ���ϲ���
			{
				LeftPnt = FillLineUp(LL, LeftPnt.ErrRow + 5, LeftPnt.ErrRow + 2);
				break;
			}
			else if (DOWN_EAGE <= PointNew.Row || LEFT_EAGE >= PointNew.Col)
			{
				LeftPnt.Type = 2;
				break;
			}
			else
			{
				PointOld = PointNew;
			}
		}
	}
	//������������ҵ�
	PointNew.Row = SearchUpEage(RightPnt.ErrRow - 1, RightPnt.ErrCol - 1) + 1;
	PointNew.Col = RightPnt.ErrCol - 1;
	PointOld = PointNew;
	if (UP_EAGE == PointNew.Row)				//���������ϱ߽�
		RightPnt = FillLineUp(RL, RightPnt.ErrRow + 5, RightPnt.ErrRow + 2);
	else
	{
		while (1)				//��ʼб65����
		{
			PointNew = SearchLeftUpEage(PointOld.Row + 1, PointOld.Col);
			if (PointOld.Row - PointNew.Row > UP45_TH)	//�ҵ���B�� ����PointOld��
			{
				//��B����ȷ��
				for (int i = PointOld.Row - 1; i > PointOld.Row - 5; i--)
				{
					if (RL[i] - RL[i + 1] > 0 || RL[i + 1] - RL[i] > FINDLINE_TH)ErrorFlag = 6;
				}
				RL[PointOld.Row] = PointOld.Col;
				FillLinePoint(RL, RightPnt.ErrRow + 2, PointOld.Row);
				RightPnt.ErrRow = PointOld.Row;
				RightPnt.ErrCol = PointOld.Col;
				RightPnt.Type = 0;
				break;
			}
			else if (UP_EAGE >= PointNew.Row || LEFT_EAGE >= PointNew.Col)		//�ҵ�Ϊ�߽�� ���ϲ���
			{
				RightPnt = FillLineUp(RL, RightPnt.ErrRow + 5, RightPnt.ErrRow + 2);
				break;
			}
			else if (DOWN_EAGE <= PointNew.Row || RIGHT_EAGE <= PointNew.Col)
			{
				RightPnt.Type = 2;
				break;
			}
			else
			{
				PointOld = PointNew;
			}
		}
	}
}

//================================================================//
//  @brief  :		��С���˷����²���
//  @param  :		void
//  @return :		void
//  @note   :		row1 > row2
//================================================================//
void FillLineDown(int* LinePoint, int row1, int row2)
{
	if (row1 <= row2)return;
	float k = LeastSquare(LinePoint, row1, row2);
	int b = *(LinePoint + row1);
	LinePoint = LinePoint + DOWN_EAGE;
	for (int i = DOWN_EAGE; i > row1; --i)
		* (LinePoint--) = (int)((i - row1) * k + b + 0.5f);
}

//================================================================//
//  @brief  :		��С���˷����ϲ���
//  @param  :		void
//  @return :		void
//  @note   :		row1 > row2
//================================================================//
SpecialPoint FillLineUp(int* LinePoint, int row1, int row2)
{
	SpecialPoint TmpP;
	if (row1 <= row2)
	{
		TmpP.Type = 9;
		TmpP.ErrRow = TmpP.ErrCol = 0;
		return TmpP;
	}
	float k = LeastSquare(LinePoint, row1, row2);
	int b = *(LinePoint + row2);
	SpecialPoint PointUp;
	LinePoint = LinePoint + row2 - 1;
	for (int i = row2 - 1; i > UP_EAGE; --i)
	{
		*(LinePoint) = (int)(0.5f + (i - row2) * k + b);
		if (*(LinePoint) < LEFT_EAGE)
		{
			PointUp.ErrRow = ++i;
			PointUp.ErrCol = *(++LinePoint);
			PointUp.Type = 5;
			return PointUp;
		}
		else if (*(LinePoint) > RIGHT_EAGE)
		{
			PointUp.ErrRow = ++i;
			PointUp.ErrCol = *(++LinePoint);
			PointUp.Type = 6;
			return PointUp;
		}
		--LinePoint;
	}
	TmpP.Type = 4;
	TmpP.ErrRow = UP_EAGE + 1;
	TmpP.ErrCol = *(++LinePoint);
	return TmpP;
}

//================================================================//
//  @brief  :		���㲹��
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillLinePoint(int* LinePoint, int row1, int row2)
{
	if (row1 <= row2) return;
	float k = (float)(LinePoint[row1] - LinePoint[row2]) / (float)(row1 - row2);
	int b = *(LinePoint + row1);
	LinePoint = LinePoint + row1 - 1;
	for (int i = row1 - 1; i > row2; --i)
	{
		*(LinePoint--) = (int)(0.5 + (i - row1) * k + b);
	}
}


//================================================================//
//  @brief  :		������
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillMiddleLine(void)
{
#define MIDDLE_MODE 2
#if 1 == MIDDLE_MODE
	if (LeftPnt.ErrRow > RightPnt.ErrRow)
	{
		ML_Count = RightPnt.ErrRow;
		for (int i = DOWN_EAGE; i >= LeftPnt.ErrRow; --i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
		int Offset = (RL[LeftPnt.ErrRow] - LL[LeftPnt.ErrRow]) >> 1;
		for (int i = LeftPnt.ErrRow - 1; i >= RightPnt.ErrRow; --i)
		{
			ML[i] = RL[i] - Offset;
			if (LEFT_EAGE > ML[i] || RIGHT_EAGE < ML[i])
			{
				ML_Count = ++i;
				return;
			}
		}
	}
	else if (LeftPnt.ErrRow < RightPnt.ErrRow)
	{
		ML_Count = LeftPnt.ErrRow;
		for (int i = DOWN_EAGE; i >= RightPnt.ErrRow; --i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
		int Offset = (RL[RightPnt.ErrRow] - LL[RightPnt.ErrRow]) >> 1;
		for (int i = RightPnt.ErrRow - 1; i >= LeftPnt.ErrRow; --i)
		{
			ML[i] = LL[i] + Offset;
			if (LEFT_EAGE > ML[i] || RIGHT_EAGE < ML[i])
			{
				ML_Count = ++i;
				return;
			}

		}
	}
	else
	{
		ML_Count = LeftPnt.ErrRow;
		for (int i = DOWN_EAGE; i >= LeftPnt.ErrRow; --i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;

		}
	}
	LeftPnt.Type = RightPnt.Type = 9;
#elif 2 == MIDDLE_MODE			//����У��ƽ��
	if (LeftPnt.ErrRow > RightPnt.ErrRow)
	{
		ML_Count = RightPnt.ErrRow;
		for (int i = DOWN_EAGE; i >= LeftPnt.ErrRow; --i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
		int Offset = ((RL[LeftPnt.ErrRow] - LL[LeftPnt.ErrRow]) >> 1) - MidOffset[LeftPnt.ErrRow];
		for (int i = LeftPnt.ErrRow - 1; i >= RightPnt.ErrRow; --i)
		{
			ML[i] = RL[i] - Offset - MidOffset[i];
			if (LEFT_EAGE > ML[i] || RIGHT_EAGE < ML[i])
			{
				ML_Count = ++i;
				return;
			}
		}
	}
	else if (LeftPnt.ErrRow < RightPnt.ErrRow)
	{
		ML_Count = LeftPnt.ErrRow;
		for (int i = DOWN_EAGE; i >= RightPnt.ErrRow; --i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
		int Offset = ((RL[RightPnt.ErrRow] - LL[RightPnt.ErrRow]) >> 1) - MidOffset[RightPnt.ErrRow];
		for (int i = RightPnt.ErrRow - 1; i >= LeftPnt.ErrRow; --i)
		{
			ML[i] = LL[i] + Offset + MidOffset[i];
			if (LEFT_EAGE > ML[i] || RIGHT_EAGE < ML[i])
			{
				ML_Count = ++i;
				return;
			}

		}
	}
	else
	{
		ML_Count = LeftPnt.ErrRow;
		for (int i = DOWN_EAGE; i >= LeftPnt.ErrRow; --i)
		{
			ML[i] = (LL[i] + RL[i]) >> 1;
		}
	}
	LeftPnt.Type = RightPnt.Type = 9;
#endif
}