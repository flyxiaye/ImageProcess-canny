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

//==============ȫ�ֱ���=========================//
#define THREE_TO_FOUR_TREND_TH 30		//״̬3ת״̬4���Ʋ�����Ч��
#define STATE_FOUR_WIDTH 165			//״̬4�������
#define STATE_FOUR_PB_EAGE_OFFSET 40	//״̬4��B��������߽�ƫ����
#define STATE_FOUR_FILLLINE_TYPE_TH 50	//״̬4���²��߷�ʽ��ֵ
#define STATE_SIX_FINDLINE_TYPE_TH	70	//״̬6�Ƿ���E����ֵ
#define GETPOINTB_TH 10					//��B����ֵ
#define GETPOINTC_TH 30					//��C����ֵ
#define GETPOINTE_TH 20					//��E����ֵ
typedef struct
{
	int Row;
	int Col;
}Point;
Point PointA, PointB, PointC, PointD, PointE;
int LastLeft, LastRight, LastMiddle;
int CircleLeftLost = 0, CircleRightLost = 0;  //���Ҷ������
int SwitchFlag = 0;								//״̬�任��־

int OneState = 0, TwoState = 0;					//״̬5 0-1-0�ж�


//================================================================//
//  @brief  :		�ж��Ƿ�Ϊ����
//  @param  :		void
//  @return :		1���󻷵� 2���һ��� 0����
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
//  @brief  :		�������ܺ���
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FillCircleIsland(void)
{
	//�����ʼ��
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
//  @brief  :		�ж�״̬,����״̬
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void JudgeState(void)
{
	//ǰʮ�г����ж�
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
		if (1 == CircleFlag)	//�󻷵�
		{
			if (CircleLeftLost)
				CircleState = 2;
		}
		else if (2 == CircleFlag)  //�һ���
		{
			if (CircleRightLost)
				CircleState = 2;
		}
		break;
	case 2:
		if (1 == CircleFlag)	//�󻷵�
		{
			if (!CircleLeftLost)
				CircleState = 3;
		}
		else if (2 == CircleFlag)	//�һ���
		{
			if (!CircleRightLost)
				CircleState = 3;
		}
		break;
	case 3:
		if (1 == CircleFlag)	//�󻷵�
		{
			if (-1 == trend(LL, MIN(THREE_TO_FOUR_TREND_TH, LeftPnt.ErrRow)))
				CircleState = 4;
		}
		else if (2 == CircleFlag)	//�һ���
		{
			if (1 == trend(RL, MIN(THREE_TO_FOUR_TREND_TH, RightPnt.ErrRow)))
				CircleState = 4;
		}
		break;
	case 4: 
		if (1 == CircleFlag)		//�󻷵�
		{
			if (CircleRightLost)
				TwoState = 1;
			if (TwoState && !CircleRightLost)
			{
				CircleState = 5;
				TwoState = 0;
			}
		}
		else if (2 == CircleFlag)		//�һ���
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
		if (1 == CircleFlag)	//�󻷵�
		{
			if (CircleRightLost)
				OneState = 1;
			if (OneState && !CircleRightLost)
			{
				CircleState = 6;
				OneState = 0;
			}
		}
		else if (2 == CircleFlag)	//�һ���
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
//  @brief  :		״̬1����
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleOne(void)
{
	//LeftPnt.ErrRow = RightPnt.ErrRow = 0;
	//FindLineNormal();
	if (1 == CircleFlag)		//�󻷵�
	{
		PointA.Row = LeftPnt.ErrRow;
		PointA.Col = LeftPnt.ErrCol;
		//��B��
		GetPointB();
		str.Format("\r\n PointB = %d \r\n", PointB.Row); PrintDebug(str);
		str.Format("\r\n PointB = %d \r\n", PointB.Col); PrintDebug(str);
		//��C��
		GetPointC();
		//��D��
		GetPointD();
		//����AB��CD
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;
		LL[PointA.Row] = PointA.Col;
		LL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_LEFT, PointA.Row, PointB.Row);
		//��������
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
	else if (2 == CircleFlag)		//�һ���
	{
		PointA.Row = RightPnt.ErrRow;
		PointA.Col = RightPnt.ErrCol;
		//��B��
		GetPointB();
		//��C��
		GetPointC();
		//��D��
		GetPointD();
		//����AB��CD
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;
		RL[PointA.Row] = PointA.Col;
		RL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_RIGHT, PointA.Row, PointB.Row);
		//��������
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
//  @brief  :		״̬2����
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleTwo(void)
{
	if (1 == CircleFlag)	//�󻷵�
	{
		////������
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
		//����
		PointA.Row = 0;
		PointA.Col = LastLeft;
		//��B��
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
			if (10 + Eage_1 == i)		//�ҵ�B��
			{
				PointB.Row = Eage_1;
				PointB.Col = j - 1;
				break;
			}
		}
		str.Format("\r\n PointB = %d \r\n", PointB.Row); PrintDebug(str);
		str.Format("\r\n PointB = %d \r\n", PointB.Col); PrintDebug(str);
		//��C��
		GetPointC();
		//��D��
		GetPointD();
		//����AB��CD
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;
		LL[PointA.Row] = PointA.Col;
		LL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_LEFT, PointA.Row, PointB.Row);
		//��������
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
	else if (2 == CircleFlag)	//�һ���
	{
		//������
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
		//����
		PointA.Row = 0;
		PointA.Col = LastRight;
		//��B��
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
			if (10 + Eage_1 == i)		//�ҵ�B��
			{
				PointB.Row = Eage_1;
				PointB.Col = j + 1;
				break;
			}
		}
		str.Format("\r\n PointB = %d \r\n", PointB.Row); PrintDebug(str);
		str.Format("\r\n PointB = %d \r\n", PointB.Col); PrintDebug(str);
		//��C��
		GetPointC();
		//��D��
		GetPointD();
		//����AB��CD
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;
		RL[PointA.Row] = PointA.Col;
		RL[PointB.Row] = PointB.Col;
		FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		FillLinePnt(FIND_RIGHT, PointA.Row, PointB.Row);
		//��������
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
//  @brief  :		״̬3����
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleThree(void)
{
	//FindLineNormal();
	if (1 == CircleFlag)	//	�󻷵�
	{
		PointB.Col = ArrayMax(LL, LeftPnt.ErrRow);
		PointB.Row = LeftPnt.ErrRow;
		//��C��

		GetPointC();
		//��D��
		GetPointD();
		//����AB��CD
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;
		FillLinePnt(FIND_RIGHT, PointD.Row, PointC.Row);
		RightPnt.ErrRow = PointC.Row;
		RightPnt.ErrCol = PointC.Col;
		RightPnt.ErrType = 1;
		LeftPnt.ErrType = 2;
	}
	else if (2 == CircleFlag)	//�һ���
	{
		PointB.Col = ArrayMin(RL, RightPnt.ErrRow);
		PointB.Row = RightPnt.ErrRow;
		//��C��
		GetPointC();
		//��D��
		GetPointD();
		//����AB��CD
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
//  @brief  :		״̬4����
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFour(void)
{
	if (1 == CircleFlag)	//�󻷵�
	{
		if (!CircleLeftLost)		//�󲻶���
		{
			//������
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
		//��C��
		GetPointC();
		str.Format("\r\n PointC = %d \r\n", PointC.Row); PrintDebug(str);
		str.Format("\r\n PointC = %d \r\n", PointC.Col); PrintDebug(str);
		//����CD
		PointD.Row = 0;
		PointD.Col = LL[0] + STATE_FOUR_WIDTH;
		RL[PointD.Row] = PointD.Col;
		RL[PointC.Row] = PointC.Col;

		//��������
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
	else if (2 == CircleFlag)		//�һ���
	{
		if (!CircleRightLost)		//�Ҳ�����
		{
			//������
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
		//��C��
		GetPointC();
		str.Format("\r\n PointC = %d \r\n", PointC.Row); PrintDebug(str);
		str.Format("\r\n PointC = %d \r\n", PointC.Col); PrintDebug(str);
		//����CD
		PointD.Row = 0;
		PointD.Col = RL[0] - STATE_FOUR_WIDTH;
		LL[PointD.Row] = PointD.Col;
		LL[PointC.Row] = PointC.Col;

		//��������
		for (int i = PointC.Row + 1; i < IMG_ROWS; ++i)
		{
			if (1 != FindL(FIND_LEFT, i, i - 1, AutoValue(i)))
			{
				LeftPnt.ErrRow = --i;
				LeftPnt.ErrCol = LL[i];
				break;
			}
		}
		if (PointC.Row > STATE_FOUR_FILLLINE_TYPE_TH)			//ѡ�����²������������� ȡ�����ϲ����ߵ�����
			FillLinePnt(FIND_LEFT, PointD.Row, PointC.Row);
		else
			FillLineDown(FIND_LEFT, PointC.Row);
		LeftPnt.ErrType = 1;
		RightPnt.ErrType = 2;
	}
}

//================================================================//
//  @brief  :		״̬5����
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleFive(void)
{
	if (1 == CircleFlag)	//�󻷵�
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
		//��������
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
		//��������
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
//  @brief  :		״̬6����
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void CircleSix(void)
{
	if (1 == CircleFlag)	//�󻷵�
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
			if (STATE_SIX_FINDLINE_TYPE_TH == i)	//δ�ҵ�E��
			{
				LeftPnt.ErrType = 7;
				FindLineLost();
			}
			else			//�ҵ�E��
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
	else if (2 == CircleFlag)	//�һ���
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
			if (STATE_SIX_FINDLINE_TYPE_TH == i)	//δ�ҵ�E��
			{
				RightPnt.ErrType = 7;
				FindLineLost();
			}
			else			//�ҵ�E��
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
//  @brief  :		��ȡB��
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointB(void)
{
	if (1 == CircleFlag)	//�󻷵�
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
			if (GETPOINTB_TH + Eage_1 == i)		//�ҵ�B��
			{
				PointB.Row = Eage_1;
				PointB.Col = j - 1;
				break;
			}
		}
	}
	else if (2 == CircleFlag)	//�һ���
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
			if (GETPOINTB_TH + Eage_1 == i)		//�ҵ�B��
			{
				PointB.Row = Eage_1;
				PointB.Col = j + 1;
				break;
			}
		}
	}
}

//================================================================//
//  @brief  :		��ȡC��
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
		if (RL[Eage_1] == k)		//�ҵ�C��
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
				if (Eage_2 + GETPOINTC_TH == j)		//�ҵ�C��
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
		if (LL[Eage_1] == k)		//�ҵ�C��
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
				if (Eage_2 - GETPOINTC_TH == j)		//�ҵ�C��
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
//  @brief  :		��ȡD��
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
//  @brief  :		��ȡE��
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void GetPointE(int row, int col)
{
	if (1 == CircleFlag)	//�󻷵�
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
		if (GETPOINTE_TH + Eage == i)		//�ҵ�E��
		{
			return;
		}
	}
	else if (2 == CircleFlag)	//�һ���
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
		if (GETPOINTE_TH + Eage == i)		//�ҵ�E��
		{
			return;
		}
	}
}

//================================================================//
//  @brief  :		0-1-0״̬�任
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
//  @brief  :		б45���ѱ߽�
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
int IsEage45(int type, int row, int col)
{
	if (FIND_LEFT == type)		//������
	{
		if ((ImgBuff[(127 - row + FRONT_LINE) * 40 + (col >> 3)] & Bit[col & 7]) == 0x00
			&& (ImgBuff[(126 - row + FRONT_LINE) * 40 + ((col + 1) >> 3)] & Bit[(col + 1) & 7]) == Bit[(col + 1) & 7])
			return 1;
		else return 0;
	}
	else if (FIND_RIGHT == type)	//��������
	{
		if ((ImgBuff[(127 - row + FRONT_LINE) * 40 + (col >> 3)] & Bit[col & 7]) == 0x00
			&& (ImgBuff[(126 - row + FRONT_LINE) * 40 + ((col - 1) >> 3)] & Bit[(col - 1) & 7]) == Bit[(col - 1) & 7])
			return 1;
		else return 0;
	}
}

//================================================================//
//  @brief  :		��¼���ʼ��
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