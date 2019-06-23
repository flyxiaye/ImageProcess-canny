#ifndef GlobalVar_H
#define GlobalVar_H
#include "windows.h"
#include "Exp.h"
#define LEFT_EAGE 0			//ͼ����߽�
#define RIGHT_EAGE 187		//ͼ���ұ߽�
#define MIDDLE 94			//ͼ����ֵ
#define UP_EAGE 25			//ͼ���ϱ߽�
#define DOWN_EAGE 70		//ͼ���±߽�
#define IMG_ROW 120			//ͼ������
#define IMG_COL 188			//ͼ������
//#define FIND_LEFT 0
//#define FIND_RIGHT 1

#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#define MIN(x, y) ((x) < (y)) ? (x) : (y)

#define FINDLINE_TH 7		//������ֵ
#define IN_OFFSET 2 //����ƫ��
//#define OUT_OFFSET 6 //����ƫ��
#define CIRCLEUP_TH 15		//���뻷�������

#define CIRCLE 2			//��������
#define BROKEN 1			//��·����
#define CURVE_BROKEN 1
#define RAMP 1				//�µ�����
#define BLOCK_BROKEN 1		//·�϶�·����
#define STOPLINE 1			//ͣ������
#define CI_IND 0			//��������־
#define INF 0				//����ʶ��

typedef struct
{
	int Row;
	int Col;
}Point;

typedef struct
{
	int Type;		//0��ͨ��� 1���� 2���� 3�յ� 4�ϱ߽� 5��߽� 6�ұ߽� 7���ж��� 8��4��ʮ�ֲ��� 9����ѭ��
	int ErrRow;
	int ErrCol;
}SpecialPoint;

enum CircleType
{
	CN, CL, CR
};

enum EageType
{
	NO, HIGH_TH, LOW_TH
};

extern const int MidOffset[];
extern int ML_Count;									//������Ч��
extern SpecialPoint LeftPnt, RightPnt;					//���������������Ϣ
extern int g_RoadType;
extern int ErrorFlag;								//�����־λ
extern int LastMiddleLine;							//������֡����
extern int DiffThreshold;							//��Ե�����ֵ
extern int LightThreshold;						//ȥ�߹�������ֵ
extern int LightThreshold2;						//ȥ�߹�������ֵ2
extern int FindLineType;							//�Ƿ����߹��˲���־

extern int SpeedRow;			//���ٱ߽�����

extern int CircleFlag;
extern int CircleState;

extern int LeftIntLine;		//��¼����������
extern int RightIntLine;		//��¼����������
extern int Img_SpecialElemFlag;    //����Ԫ�ر�־
extern int Img_BrokenFlag;			//��·��־ 1ֱ����·  3�����·
extern int Img_BlockFlag;			//·�ϱ�־
extern int Img_StopLineFlag;		//ͣ���߱�־
extern int Img_RampFlag;			//�µ���־

extern int BrokenLastAve;
extern int StopLineDist;
		
extern unsigned char Img_CircleOpen;
extern unsigned char Img_BrokenOpen;
extern unsigned char Img_StopOpen;
extern unsigned char Img_BlockOpen;
extern unsigned char Img_RampOpen;

extern int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];
extern unsigned char ImageEage[IMG_ROW][IMG_COL];
void VarInit(void);

extern Point PointE;
extern Point PointF;

//��ʱ
extern LARGE_INTEGER f;
extern double dFreq;
extern LARGE_INTEGER start, end, end2;
#endif // !GlobalVar_H
#pragma once
