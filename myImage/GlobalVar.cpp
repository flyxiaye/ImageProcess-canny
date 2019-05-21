#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"


SpecialPoint LeftPnt, RightPnt, LeftPntTre, RightPntTre;

const unsigned char Bit[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
//0������־λ	1��·��־λ		2���ұ�־λ
//unsigned char ImgBuff[IMG_COLS * IMG_ROWS];

int LeftLost = -1, RightLost = -1;                       //���Ϊ����ǰ��һ�� ������

int LL[IMG_ROWS] = { 0 };								//����������
int RL[IMG_ROWS] = { 0 };
int ML[IMG_ROWS] = { 0 };
const int Mid = (LEFT_EAGE + RIGHT_EAGE) >> 1;



//��־λ
unsigned char FirstRoadType;			//����������� 1 ��ȷ������ 2 ֱ��ϸ�� 3 ���߶��������������� бʮ�֣� 4 ������ʮ������
int FindLine = 0;						//��ǵ�ǰ��
int LeftFindFlag = 0, RightFindFlag = 0;  //����Ѱ��������
int ThreeFlag = 0;						//3��������־ 1Ϊ�� 2Ϊ��

int g_trend_flag = 0;				//1���������� -1����������
int row_n = 0;

int FillFlag = 0;
int ErrorFlag = 0;						//�쳣������
//int CircleFlag = 0;				//������־
int CutFlag = 0;				//��·��־
int BevelCrossFlag = 0;			//бʮ�ֱ�־ tips:��бʮ�ֺ���ֻʹ��һ��
int LevelCrossFlag = 0;			//ƽʮ�ֱ�־
int StopFillFlag = 0;			//ɲ�����߱�־

//===================================����======================//
int CircleFlag = 0;
int CircleState = 0;			//	4������ 5������ 6��������

void VarInit(void)
{
	//ȫ�ֱ�����ʼ��
	for (int i = IMG_ROWS - 1; i >= 0; --i)
	{
		LL[i] = LEFT_EAGE;
		RL[i] = RIGHT_EAGE;
		ML[i] = Mid;
	}
	IsCross3(0, 0);
	FindLine = 0;
	FirstRoadType = 0;
	LeftLost = RightLost = -1;
	LeftFindFlag = RightFindFlag = 0;
	g_trend_flag = 0;
	FillFlag = 0;
	BevelCrossFlag = 0;
	LevelCrossFlag = 0;
	LeftPnt.ErrType = 0;
	LeftPnt.ErrRow = 0;
	LeftPnt.ErrCol = LEFT_EAGE;
	RightPnt.ErrType = 0;
	RightPnt.ErrRow = 0;
	RightPnt.ErrCol = RIGHT_EAGE;
	StopFillFlag = 0;
	ErrorFlag = 0;
}

//globalvar
int g_stop_line = 0;		//ͣ����־
int g_stop_dis = 0;			//�Ӿ�ͣ����־
int MaxSpeedFlag = 0;     //���ٱ�־
