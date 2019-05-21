#ifndef GlobalVar_H
#define GlobalVar_H

#endif // !GlobalVar_H
#pragma once
#define FIND_LEFT 0
#define FIND_RIGHT 1
#define MaxValue 12								//Ѱ����ֵ
#define MaxValueCol 25
#define MaxValue1 60
#define MAX(x, y) x > y ? x : y
#define MIN(x, y) x < y ? x : y
#define FRONT_LINE 0
#define LEFT_EAGE 0
#define RIGHT_EAGE 187
//#define IMG_ROWS 128
//#define IMG_COLS 320

#define WHITE 0
#define BLACK 1

typedef struct
{
	int ErrType; //0��ͨ��� 1���� 2���� 3�յ� 4��ֱ����ͼ���ϱ߽磩5��߽� 6�ұ߽� 7���ж��� 8����ʮ�� 9����ѭ�� 10����
	int ErrRow;
	int ErrCol;
}SpecialPoint;

typedef struct
{
	float k;
	float b;
}Line;

typedef struct
{
	int x1;
	int x2;
}Vector;

extern SpecialPoint LeftPnt, RightPnt, LeftPntTre, RightPntTre;

extern const unsigned char Bit[];
//1��·��־λ		2���ұ�־λ
//unsigned char ImgBuff[IMG_COLS * IMG_ROWS];
extern unsigned char FirstRoadType;					//����������� 1 ��ȷ������ 2 ֱ��ϸ�� 3 ���߶��������������� бʮ�֣� 4 ������ʮ������
extern int LeftLost, RightLost;                       //���Ϊ����ǰ��һ�� ������
extern int FindLine;						//��ǵ�ǰ��
extern int LL[IMG_ROWS];								//����������
extern int RL[IMG_ROWS];
extern int ML[IMG_ROWS];
extern const int Mid;
extern int LeftFindFlag, RightFindFlag;
extern int g_trend_flag;			//1���������� -1����������
extern int row_n;
extern int ThreeFlag;				//1Ϊ�� 2Ϊ��

extern int FillFlag;						//��ͼʧ�ܱ�־
extern int ErrorFlag;						//�쳣������
extern int BevelCrossFlag;					//бʮ�ֱ�־
extern int LevelCrossFlag;					//ƽʮ�ֱ�־
extern int StopFillFlag;					//ͣ����־

//===================================����======================//
extern int CircleFlag;			//0���ǻ��� 1�󻷵� 2�һ���
extern int CircleState;

void VarInit(void);

//globalvar
extern int g_stop_line;
extern int g_stop_dis;
extern int MaxSpeedFlag;